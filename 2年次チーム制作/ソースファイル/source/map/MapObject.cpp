#include "../All.h"
#include "MapObject.h"
#include "ObjectLoader.h"
#include "MyFunction.h"
#include "MapObjectFactory.h"
#include "MapObjectMainStage.h"
#include "Truck.h"
#include "WayPointObject.h"
#include "Crane.h"
#include "Sky.h"
#include "MapObjectSignboard.h"
#include "Elevator.h"
#include "VendingMachine.h"
#include "AirShip.h"

//*************************************************************
//		オブジェクト基本クラス
//*************************************************************

MapObjectInterface::MapObjectInterface(
	ENTITY_ID               id,
	ObjectParam            param,
	LPIEXMESH              mesh,
	MAP_OBJ_TYPE       type
	) :
	GameBaseEntity(id),
	object_type_(type),
	parameters_(param),
	mesh_(mesh)
{
	D3DXMatrixIdentity(&difference_prev_frame_matrix_);
	D3DXMatrixIdentity(&RT_matrix_);
	D3DXMatrixIdentity(&RT_matrix_inverse_);
	D3DXMatrixIdentity(&S_matrix_);
	D3DXMatrixIdentity(&SRT_matrix_);
	D3DXMatrixIdentity(&SRT_matrix_inverse_);
}


MapObjectInterface::~MapObjectInterface()
{

}


void MapObjectInterface::Update()
{
	//転送行列作成
	SetTransformMatrixXYZ(
		&RT_matrix_,
		parameters_.pos.x,
		parameters_.pos.y,
		parameters_.pos.z,
		parameters_.angle.x,
		parameters_.angle.y,
		parameters_.angle.z);

	// 自身を回転させる
	RotateSelf();

	//移動行列作成
	difference_prev_frame_matrix_ = RT_matrix_ * RT_matrix_inverse_;

#if 0
	MyDebugMgrStr("%2.2f %2.2f %2.2f %2.2f", m_matMove._11, m_matMove._12, m_matMove._13, m_matMove._14);
	MyDebugMgrStr("%2.2f %2.2f %2.2f %2.2f", m_matMove._21, m_matMove._22, m_matMove._23, m_matMove._24);
	MyDebugMgrStr("%2.2f %2.2f %2.2f %2.2f", m_matMove._31, m_matMove._32, m_matMove._33, m_matMove._34);
	MyDebugMgrStr("%2.2f %2.2f %2.2f %2.2f", m_matMove._41, m_matMove._42, m_matMove._43, m_matMove._44);
	MyDebugMgrStr("");
#endif

	//転送行列の逆行列作成
	D3DXMatrixInverse(
		&RT_matrix_inverse_,
		NULL,
		&RT_matrix_);

	//子に適応
	PARENTANDCHILD::UpdateAllChild(
		&RT_matrix_,
		&RT_matrix_inverse_,
		&difference_prev_frame_matrix_);

	//スケーリング行列作成
	D3DXMatrixScaling(
		&S_matrix_,
		parameters_.scale.x,
		parameters_.scale.y,
		parameters_.scale.z);

	//適応
	MulMatrix3x3(SRT_matrix_, &S_matrix_, &RT_matrix_);

	SRT_matrix_._41 = RT_matrix_._41;
	SRT_matrix_._42 = RT_matrix_._42;
	SRT_matrix_._43 = RT_matrix_._43;

	D3DXMatrixInverse(
		&SRT_matrix_inverse_,
		NULL,
		&SRT_matrix_);

	// 転送行列の適応は描画直前にしている
}


void MapObjectInterface::Render()
{
	if (mesh_)
	{
		//転送行列の適応
		mesh_->TransMatrix = SRT_matrix_;
#if LIGHTMAP
		mesh_->Render(shader, "mainstage_lightmap");
#else
		mesh_->Render(shader, "mainstage_realtime_shadow");
#endif
	}
}


void	MapObjectInterface::Render_ShadowMap()
{
	if (mesh_)
	{
		//転送行列の適応
		mesh_->TransMatrix = SRT_matrix_;
		
		mesh_->Render(shader, "ShadowBuf");
	}
}


void MapObjectInterface::Render(char* shaderName)
{
	if (mesh_)
	{
		mesh_->Render(shader, shaderName, SRT_matrix_);
	}
}


bool MapObjectInterface::RayPick(
	Vector3* out,
	Vector3* pos,
	Vector3*vec,
	float* dist
	)
{
	if (mesh_ == nullptr)
	{// メッシュがない
		return false;
	}

	bool result = 
		(mesh_->RayPickPlus(out, pos, vec, dist, SRT_matrix_, SRT_matrix_inverse_) != -1);

	return result;
}

// dataからマップオブジェクトを作成、登録をする
void CreateObjectFromData(const ObjectData& data)
{
	using namespace map_obj;

#if 0
	MapObjectFractory* factory = nullptr;

	switch (data.type)
	{
	case _main_stage:
		factory = new MapObjectMainStageFactory();
		break;
	case _resporn_point_begin:
	case _resporn_point_end:
		factory = new MapObjectFractory();
		break;
	case _flag:
		// 作成しない
		break;
	case _share_obj_begin:
	case _share_obj_end:
		factory = new PosterFactory();
		break;
	case _way_point_obj_begin:
	case _way_point_obj_end:
		factory = new WayPointFactory();
		break;
	case _truck:
		factory = new TruckFactory();
		break;
	case _crane_body:
		factory = new CraneFactory();
		break;
	case _crane_steel_frame:
		// 作成しない
		break;
	case _sky:
		factory = new MapObjectSkyFactory();
		break;
	case _signboard:
		factory = new MapObjectSignboardFactory();
		break;
	case _max:
		// 作成しない
		break;
	default:
		// 作成しない
		break;
	}
	if (factory != nullptr)
	{
		factory->CreateNewObject(data);
	}
#else

	if (data.type == _main_stage)
	{// メインステージ
		MapObjectMainStageFactory factory;
		factory.CreateNewObject(data);
	}
	else if (data.type == _truck)
	{// トラック
		TruckFactory factory;
		factory.CreateNewObject(data);
	}
	else if (data.type == _crane_body)
	{// クレーン
		CraneFactory factory;
		factory.CreateNewObject(data);
	}
	else if (data.type == _sky)
	{// 空
		MapObjectSkyFactory factory;
		factory.CreateNewObject(data);
	}
	else if (data.type == _signboard)
	{// 看板
		MapObjectSignboardFactory factory;
		factory.CreateNewObject(data);
	}
	else if (data.type == _elevator)
	{// エレベーター
		ElevatorFactory factory;
		factory.CreateNewObject(data);
	}
	else if (data.type == _air_ship)
	{// 飛行船
		AirShipFactory factory;
		factory.CreateNewObject(data);
	}
	else if (data.type >= _resporn_point_begin 
		&& data.type <= _resporn_point_end)
	{// 自動販売機
		VendingMachineFactory factory;
		factory.CreateNewObject(data);
	}
	else if (data.type >= _way_point_obj_begin
		&& data.type <= _way_point_obj_end)
	{// WayPoint
		WayPointFactory factory;
		factory.CreateNewObject(data);
	}
	else if (data.type >= _share_obj_begin
		&& data.type <= _share_obj_end)
	{// ポスター
		PosterFactory factory;
		factory.CreateNewObject(data);
	}
	else
	{// その他
		MapObjectFractory factory;
		factory.CreateNewObject(data);
	}
#endif
}

// fileからメッシュ管理クラスにメッシュを作成、登録する
bool LoadMeshFile(char* file, MeshManager** pMeshMgr)
{
	textLoader loader(file);
	char tmp[256];
	char t[512];

	std::list<LPIEXMESH> mesh_list;

	while (loader.EndCheck())
	{
		loader._Loadstring(tmp);
		sprintf_s<512>(t, "DATA\\stage_mesh\\%s", tmp);
		mesh_list.push_back(new IEXMESH(t));
	}
	if (!mesh_list.empty())
	{
		*pMeshMgr = new MeshManager((int)mesh_list.size());

		auto& it = mesh_list.begin();
		auto& end = mesh_list.end();

		for (; it != end; ++it)
		{
			(*pMeshMgr)->MeshRegister(*it);
		}
		return true;
	}
	return false;
}


//************************************************************
//	マップオブジェクトマネージャクラス(Singleton)
//************************************************************

MapObjectManager* MapObjectManager::instance_ = nullptr;


MapObjectManager::MapObjectManager() :
stage_center_(0, 0, 0),
space_partitioning_field_(nullptr),
mesh_manager_(nullptr)
{
	space_partitioning_field_ =
		new sp_field(&stage_center_, 3000.0f, 5);

	WayPointFactory::Initcount_way_point(); 
	MapObjectFractory::InitCount();

}


MapObjectManager::~MapObjectManager()
{
	for (auto& it : map_objects_)
	{
		safe_delete(it);
	}

	safe_delete(space_partitioning_field_);
	safe_delete(mesh_manager_);
}

void MapObjectManager::CreateInstance()
{
	if (!instance_)instance_ = new MapObjectManager();
}

MapObjectManager& MapObjectManager::getInstance()
{
	return *instance_;
}

void MapObjectManager::Release()
{
	delete instance_;
	instance_ = nullptr;
}

//--------------------------------------------------------------------
// レイ判定
// 戻り値 : 一番近くのレイの当たったオブジェクトへのポインタ
//--------------------------------------------------------------------
MapObjectInterface* MapObjectManager::RayPick(
	Vector3* out,
	Vector3* pos,
	Vector3*vec,
	float* dist
	)
{
	const Vector3 fpos(*pos), fvec(*vec);
	const float fdist(*dist);

	Vector3 tempout, temppos(fpos), tempvec(fvec);
	float tempdist(fdist);

	float near_obj = FLT_MAX;

	MapObjectInterface *ret = nullptr;

	for (auto& target : map_objects_)
	{
		if (target->RayPick(&tempout, &temppos, &tempvec, &tempdist))
		{
			VECTOR3POINTTOPOINT(tempdist, tempout, fpos, temppos);

			if (tempdist < near_obj)
			{
				(*out) = tempout;
				(*vec) = tempvec;

				near_obj = tempdist;
				ret = target;
			}
		}
		temppos = fpos;
		tempvec = fvec;
		tempdist = fdist;
	}
	return ret;
}


bool MapObjectManager::LoadObject(char* obj_file, char* mesh_file)
{
	ObjectLoader Loader(obj_file);

	if (Loader.Load() == false)
	{
		return false;
	}

	if (LoadMeshFile(mesh_file, &mesh_manager_) == false)
	{
		return false;
	}

	ObjectDataList& obj_list = Loader.GetList();

	for (auto& it : obj_list)
	{
		CreateObjectFromData(it);
	}
	return true;
}


void MapObjectManager::AddObject(MapObjectInterface* obj)
{
	map_objects_.push_back(obj);
}


void MapObjectManager::Get_TypeObject(MAPOBJ_ARRAY& out, MAP_OBJ_TYPE type)
{
	for (auto& it : map_objects_)
	{
		if (it->Get_Type() == type)
		{
			out.push_back(it);
		}
	}
}


void MapObjectManager::Get_ShareTypeObject(MAPOBJ_ARRAY& out)
{
	MAP_OBJ_TYPE type;

	for (auto& it : map_objects_)
	{
		type = it->Get_Type();

		if (type >= map_obj::_share_obj_begin
			&&type <= map_obj::_share_obj_end)
		{
			out.push_back(it);
		}
		else if (type >= map_obj::_resporn_point_begin
			&& type <= map_obj::_resporn_point_end)
		{// 自販機も含める
			out.push_back(it);
		}
	}
}


void MapObjectManager::Update()
{
	//範囲内のオブジェクト取得
	//m_ObjVector =m_pField->SearchObject(&m_center, 500);

	objects_in_range_.clear();

	for (auto& it : map_objects_)
	{
		objects_in_range_.push_back(it);
		// このマクロは良くないと思う...
		SAFE_EXE(it, Update);
	}
	UpdateShareValue();
}


void MapObjectManager::Render()
{
	for (auto& it : map_objects_)
	{
		SAFE_EXE(it, Render);
	}
}


void	MapObjectManager::Render_ShadowMap()
{
	for (auto& it : map_objects_)
	{
		SAFE_EXE(it, Render_ShadowMap);
	}
}


void MapObjectManager::Render(char* shader_Name)
{
	for (auto& it : map_objects_)
	{
		SAFE_EXE(it, Render, shader_Name);
	}
}


void MapObjectManager::Get_ShareValue(SHAREPARAM_ARRAY& out)
{
	out = share_parameters_;
}


void MapObjectManager::UpdateShareValue()
{
	//テスト用勢力ゲージ
	static const int team_max = 2;
	float share_power[team_max] = { 0 };
	float share_max = 0;

	share_parameters_.clear();

	MAPOBJ_ARRAY Array;
	MAP_MANAGER.Get_ShareTypeObject(Array);

	ShareRatioObjectBase* obj;
	for (auto& it : Array)
	{
		obj = (ShareRatioObjectBase*)(it);
		const ShareRatioParam& param = obj->Get_ShareParam();

		if (param.team_type >= 0)
		{
			share_power[param.team_type] += param.share_power;
			share_max += param.share_power;
		}
	}

	if (share_max <= 0)return;

	ShareRatioParam param;
	for (int i = 0; i < team_max; ++i)
	{
		param.team_type = i;
		param.share_power = share_power[i] / share_max;
		share_parameters_.push_back(param);
	}
}


void MapObjectManager::Get_MeshManager(MeshManager** out)
{
	*out = mesh_manager_;
}

// EOF