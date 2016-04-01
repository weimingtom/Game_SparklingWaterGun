#include "../All.h"
#include "SwingObject.h"
#include "Crane.h"
#include "MyFunction.h"

// この値もファイルに埋め込んでおくべきなのか？
static const Vector3 gravity(0, -0.05f, 0);
#define GRAVITY gravity
#define CRANE_ARM_HEIGHT 23.0f
#define CRANE_ARM_LENGTH 18.0f
#define CRANE_ROTARY_SPEED 90.0f/(60*3)
#define CRANE_WIRE_LENGTH 18.5f
#define CRANE_ROTATE_TIME 60 * 7
#define CRANE_ROTATE_STOP_TIME 60 * 2

static const int kCraneAddForceTime = 60 * 5;  // 一定時間ごとに揺れる
static const float kCraneAddForceValue = 0.5f;  // 一定時間ごとに揺れる力

//*************************************************************
// クレーンのボディクラス
//*************************************************************

CraneBody::CraneBody(
	ENTITY_ID               id,
	ObjectParam            param,
	LPIEXMESH              mesh,
	MAP_OBJ_TYPE       type
	) :
	MapObjectInterface(id, param, mesh, type),
	rotary_speed_(CRANE_ROTARY_SPEED),
	arm_position_(0, CRANE_ARM_HEIGHT, 0),
	SteelFrame_(nullptr),
	initialized_(false),
	timer_(0)
{
	
}

CraneBody::~CraneBody()
{

}


void CraneBody::Initialize(CraneSteelFrame* pSteelFrame)
{
	if (initialized_)
	{
		safe_delete(pSteelFrame);
		MyAssert(false, "初期化済みです！");
		return;
	}
	SteelFrame_ = pSteelFrame;
	AddChild(SteelFrame_);
	initialized_ = true;
}


void CraneBody::AddForce(CRVector3 force)
{
	SteelFrame_->AddForce(force);
}


void CraneBody::Update()
{
	const int stoptime = CRANE_ROTATE_STOP_TIME;
	const int movetime = CRANE_ROTATE_TIME;

	// 一定時間ごとに90°回転
	++timer_;
	if (timer_ < movetime)
	{
		parameters_.angle.y += PI / 2 / (float)movetime;
	}
	else if (timer_ >= stoptime + movetime)
	{
		timer_ = 0;
	}

	// アームの先端の座標はハードコーディング
	arm_position_.x = -sinf(parameters_.angle.y)*CRANE_ARM_LENGTH;
	arm_position_.z = -cosf(parameters_.angle.y)*CRANE_ARM_LENGTH;

	// 更新
	MapObjectInterface::Update();
}


void CraneBody::Render()
{
	if (mesh_)
	{
		mesh_->TransMatrix = SRT_matrix_;
		mesh_->Render(shader, "truck");
	}
}


void CraneBody::Render_ShadowMap()
{
	if (mesh_)
	{
		mesh_->TransMatrix = SRT_matrix_;
		mesh_->Render(shader, "ShadowBuf");
	}
}

//*************************************************************
// クレーンに吊られているものクラス
//*************************************************************
CraneSteelFrame::CraneSteelFrame(
	CraneBody*             pBody,
	ENTITY_ID               id,
	ObjectParam            param,
	LPIEXMESH              mesh,
	MAP_OBJ_TYPE       type
	) :
	MapObjectInterface(id, param, mesh, type),
	timer_(0), 
	max_wire_length_(CRANE_WIRE_LENGTH),
	MyBody_(pBody),
	ride_timer(0)
{
	// 初期座標設定
	Vector3 arm_pos = pBody->Get_ArmPosition();

	swing_object_.Set_OriginalPosition(arm_pos);
	swing_object_.Set_CoefficientOfDrag(0.02f);
	swing_object_.Set_ObjectPosition(arm_pos + Vector3(0, -max_wire_length_, 0));
	swing_object_.Update();

	parameters_.pos = swing_object_.Get_ObjectPosition();

	D3DXQuaternionIdentity(&pose_);

	// 現在の設計上、MeshManagerから受け取るときの
	// id がわからないからメッシュをCraneBodyと別にできないので
	// とりあえず自分で作成する

	mesh_ = new IEXMESH("DATA\\stage_mesh\\StageObj\\Crane_saki.IMO");
	wire_mesh_ = new IEXMESH("DATA\\stage_mesh\\StageObj\\chain.IMO");

	parameters_.scale = Vector3(0.03f, 0.03f, 0.03f);
	D3DXMatrixScaling(&wire_scale_, 0.01f, 0.007f, 0.01f);
}


CraneSteelFrame::~CraneSteelFrame()
{
	safe_delete(mesh_);
	safe_delete(wire_mesh_);
}


void CraneSteelFrame::AddForce(CRVector3 force)
{
	// 揺らす
	swing_object_.AddForce(force);
}


void CraneSteelFrame::Update()
{

#ifdef _DEBUG
	// テスト用
	if (GetKeyState('L') < 0){
		AddForce(Vector3(0.01f, 0, 0));
	}
#endif

	if (ride_timer>0)--ride_timer;

	++timer_;
	if (timer_ >= kCraneAddForceTime)
	{
	//	swing_object_.AddForce(Vector3Rand()*kCraneAddForceValue);
		timer_ = 0;
	}

	// 振り子のように揺れる処理がいまいちうまくいかない。
	// どうするべきか？->揺れものクラスで実装できた。

	const Vector3& arm_pos = MyBody_->Get_ArmPosition();
	swing_object_.Set_OriginalPosition(arm_pos);
	swing_object_.AddForce(GRAVITY);

	swing_object_.Update();
	parameters_.pos = swing_object_.Get_ObjectPosition();

	// 姿勢更新
	MapObjectInterface::Update();
}

void CraneSteelFrame::RotateSelf()
{
	// 吊られている支点に上を向ける

	Vector3 arm_vec = MyBody_->Get_ArmPosition() - parameters_.pos;

	D3DXVECTOR3 up(0, 1, 0), vec(arm_vec.x, arm_vec.y, arm_vec.z);
	D3DXVec3Normalize(&vec, &vec);

	D3DXVECTOR3 axis;
	D3DXVec3Cross(&axis, &up, &vec);

	float rotate_angle =
		acosf(D3DXVec3Dot(&up, &vec));

	
	if (rotate_angle > 0.001f)
	{
		// ※小さすぎる場合は回転しない
		D3DXQuaternionRotationAxis(&pose_, &axis, rotate_angle);
	}

	// 行列を使って回転させる
	Matrix R,T;
	D3DXMatrixRotationQuaternion(&R, &pose_);
	D3DXMatrixTranslation(&T, parameters_.pos.x, parameters_.pos.y, parameters_.pos.z);

	RT_matrix_ = R*T;
}

void CraneSteelFrame::Render()
{
	if (mesh_)
	{
		mesh_->TransMatrix = SRT_matrix_;
		mesh_->Render(shader, "truck");
	}
	if (wire_mesh_)
	{
		wire_mesh_->TransMatrix = wire_scale_ * RT_matrix_;
		wire_mesh_->Render(shader, "truck");
	}
}

void CraneSteelFrame::Render_ShadowMap()
{
	if (mesh_)
	{
		mesh_->TransMatrix = SRT_matrix_;
		mesh_->Render(shader, "ShadowBuf");
	}
	if (wire_mesh_)
	{
		wire_mesh_->TransMatrix = wire_scale_ * RT_matrix_;
		wire_mesh_->Render(shader, "ShadowBuf");
	}
}


void CraneSteelFrame::ParentMove(
	LPCMATRIX pParentWorldMat,
	LPCMATRIX pParentWorldMatInv,
	LPCMATRIX pParentMoveMat,
	LPCMATRIX AffineMat)
{
	// CraneBodyクラスとは親子関係を結んでおくべき？
	// おそらくなくても大丈夫
}

bool CraneSteelFrame::HandleMessage(LPGameMessage msg)
{
	if (msg->type == msg_t::ride_obj&&
		ride_timer == 0)
	{
		LPCONTROLLOBJCT pObj = (LPCONTROLLOBJCT)msg->exinfo;
		Vector3 v = parameters_.pos - pObj->GetBodyCenter();
		v /= 15;
		swing_object_.AddForce(v);
		ride_timer = 15;
		return true;
	}

	return false;
}

//*************************************************************
// クレーンを作成するクラス
//*************************************************************

bool CraneFactory::CreateNewObject(const ObjectData &data)
{
	// MeshManager取得
	MeshManager* mesh_manager;
	MAP_MANAGER.Get_MeshManager(&mesh_manager);
	MyAssert((mesh_manager != nullptr), "MeshManagerの取得に失敗しました。");


	ObjectParam crane_parameter;
	crane_parameter.pos = data.pos;
	crane_parameter.angle = data.angle;
	crane_parameter.scale = data.scale;

	// CraneBody作成

	ENTITY_ID body_id = 
		(ENTITY_ID)(game_id::id_map_object_min + count);

	LPIEXMESH body_mesh =
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)(data.mesh_type));
	
	CraneBody *body =
		new CraneBody(
		body_id,
		crane_parameter,
		body_mesh,
		map_obj::_crane_body
		);

	++count;

	// 吊られているもの作成

	ENTITY_ID steel_frame_id = 
		(ENTITY_ID)(game_id::id_map_object_min + count);

	LPIEXMESH steel_frame_mesh =
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)(data.mesh_type));

	// とりあえず nullptr にしておく
	steel_frame_mesh = nullptr;

	CraneSteelFrame *steel_frame = 
		new CraneSteelFrame(
		body,
		steel_frame_id,
		crane_parameter,
		steel_frame_mesh,
		map_obj::_crane_steel_frame
		);

	++count;

	// 初期化
	body->Initialize(steel_frame);

	// マネージャーに追加
	MAP_MANAGER.AddObject(body);
	MAP_MANAGER.AddObject(steel_frame);

	return true;
}

// EOF