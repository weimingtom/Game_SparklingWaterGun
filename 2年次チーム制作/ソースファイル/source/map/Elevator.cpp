#include "../All.h"
#include "SwingObject.h"
#include "Elevator.h"
#include "MyFunction.h"


static const int    kElevatorStopTime = 60*3;            // 到着してから停止するフレーム
static const int    kElevatorMoveTime = 60*3;       // ３秒かけて動く
static const float kElevatorMoveDistance = 4.0f; // 動く幅

//*************************************************************
// エレベータークラス
//*************************************************************


Elevator::Elevator(
	ENTITY_ID               id,
	ObjectParam            param,
	LPIEXMESH              mesh,
	MAP_OBJ_TYPE       type
	) :
	MapObjectInterface(id, param, mesh, type),
	state_(_wait_root),
	timer_(0),
	k_root_pos_(param.pos),
	k_target_pos_(param.pos + Vector3(0, kElevatorMoveDistance, 0)),
	move_(0, 0, 0)
{

}


Elevator::~Elevator()
{

}


void Elevator::Update()
{
	++timer_;

	UpdateState();

	parameters_.pos += move_;

	MapObjectInterface::Update();
}


void Elevator::Render()
{
#if 0
	if (mesh_)
	{
		mesh_->TransMatrix = SRT_matrix_;
		mesh_->Render(shader, "truck");
	}
#else
	MapObjectInterface::Render();
#endif
}


void Elevator::Render_ShadowMap()
{
#if 0
	if (mesh_)
	{
		mesh_->TransMatrix = SRT_matrix_;
		mesh_->Render(shader, "ShadowBuf");
	}
#else
	MapObjectInterface::Render_ShadowMap();
#endif
}


void Elevator::UpdateState()
{

	switch (state_)
	{
	case _wait_root:
		WaitRoot();break;

	case	_wait_target:
		WaitTarget();break;

	case _move_to_target:
		MoveToTarget(); break;

	case _move_to_root:
		MoveToRoot();break;

	default:
		break;
	}
}


void Elevator::WaitRoot()
{
	move_ = Vector3(0, 0, 0);

	if (timer_ < kElevatorStopTime)return;

	// 誰かが乗ったら上がる
	if (GetChildCount() > 0)
	{
		// 移動量設定
		Vector3 vec = k_target_pos_ - k_root_pos_;
		float len = vec.Length();
		float delta = len / (float)kElevatorMoveTime;

		vec.Normalize();

		move_ = vec*delta;

		// ステート変更
		state_ = _move_to_target;
		timer_ = 0;
	}
}


void Elevator::WaitTarget()
{
	move_ = Vector3(0, 0, 0);

	if (timer_ >= kElevatorStopTime)
	{
		// 移動量設定
		Vector3 vec = k_root_pos_ - k_target_pos_;
		float len = vec.Length();
		float delta = len / (float)kElevatorMoveTime;

		vec.Normalize();

		move_ = vec*delta;

		// ステート変更
		state_ = _move_to_root;
		timer_ = 0;
	}
}


void Elevator::MoveToRoot()
{
	if (timer_ >= kElevatorMoveTime)
	{
		state_ = _wait_root;
		timer_ = 0;
	}
}


void Elevator::MoveToTarget()
{
	if (timer_ >= kElevatorMoveTime)
	{
		state_ = _wait_target;
		timer_ = 0;
	}
}

//*************************************************************
// エレベーター作成クラス
//*************************************************************

bool ElevatorFactory::CreateNewObject(const ObjectData &data)
{
	MeshManager* mesh_manager;
	MAP_MANAGER.Get_MeshManager(&mesh_manager);

	// 作成に必要なものを用意する

	ObjectParam parameter;
	parameter.pos = data.pos;
	parameter.angle = data.angle;
	parameter.scale = data.scale;

	ENTITY_ID id =
		(ENTITY_ID)(game_id::id_map_object_min + count);

	LPIEXMESH elevator_mesh =
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)(data.mesh_type));

	// 作成
	Elevator* elevator =
		new Elevator(
		id,
		parameter,
		elevator_mesh,
		map_obj::_sky
		);
	++count;

	// 登録
	MAP_MANAGER.AddObject(elevator);

	return true;
}


// EOF