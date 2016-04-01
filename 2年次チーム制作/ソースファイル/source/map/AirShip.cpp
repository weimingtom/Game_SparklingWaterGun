#include "../All.h"
#include "AirShip.h"
#include "Truck.h"

#define AIR_SHIP_ROTATION_CENTER Vector3(0.0f,30.0f,0.0f)
#define AIR_SHIP_ROTATION_RADIUS 40.0f
#define AIR_SHIP_ROTATION_SPEED (1.0f*(PI / 180.0f)) / 60.0f

//*************************************************************
// 飛行船クラス
//*************************************************************
AirShip::AirShip(
	ENTITY_ID               id,
	ObjectParam            param,
	LPIEXMESH              mesh,
	MAP_OBJ_TYPE       type
	) :
	MapObjectInterface(id, param, mesh, type),
	rotation_center_(AIR_SHIP_ROTATION_CENTER),
	rotation_radius_(AIR_SHIP_ROTATION_RADIUS),
	rotation_speed_(AIR_SHIP_ROTATION_SPEED),
	rotation_angle_(0.0f)
{
	parameters_.pos = rotation_center_;
	parameters_.pos.x += sinf(rotation_angle_)*rotation_radius_;
	parameters_.pos.z += cosf(rotation_angle_)*rotation_radius_;
}


AirShip::~AirShip()
{

}


void AirShip::Update()
{

	rotation_angle_ += rotation_speed_;

	Vector3 view_target;
	view_target.y = rotation_center_.y;
	view_target.x = rotation_center_.x + sinf(rotation_angle_)*rotation_radius_;
	view_target.z = rotation_center_.z + cosf(rotation_angle_)*rotation_radius_;


	angleContRoll(parameters_.pos, view_target, &parameters_.angle.y, 5.0f);

	parameters_.pos = view_target;

	MapObjectInterface::Update();
}


void AirShip::Render()
{
	if (mesh_)
	{
		ScoreCalcManager* pScoreMgr = (ScoreCalcManager*)IDMANAGER.GetPointer(game_id::id_score_calc_manager);

		ScoreCalcManager::TeamScoreData::const_iterator cit;
		pScoreMgr->GetNo1Team(&cit);

		mesh_->SetTexure(0, GAMEMATCHINFO.GetTextureMgr().GetAirShipTexturePtr(cit->second.team_type)->GetTexture());
		mesh_->TransMatrix = SRT_matrix_;
		mesh_->Render();
		mesh_->SetTexure(0, nullptr);

	}
}


void AirShip::Render_ShadowMap()
{
	if (mesh_)
	{
		mesh_->TransMatrix = SRT_matrix_;
		mesh_->Render(shader, "ShadowBuf");
	}
}


//*************************************************************
// 飛行船作成クラス
//*************************************************************

bool AirShipFactory::CreateNewObject(const ObjectData &data)
{
	// MeshManager取得
	MeshManager* mesh_manager;
	MAP_MANAGER.Get_MeshManager(&mesh_manager);
	MyAssert((mesh_manager != nullptr), "MeshManagerの取得に失敗しました。");

	ObjectParam parameter;
	parameter.pos = data.pos;
	parameter.angle = data.angle;
	parameter.scale = data.scale;

	// AirShip作成
	ENTITY_ID air_ship_id =
		(ENTITY_ID)(game_id::id_map_object_min + count);

	LPIEXMESH air_ship_mesh =
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)(data.mesh_type));

	AirShip *air_ship =
		new AirShip(
		air_ship_id,
		parameter,
		air_ship_mesh,
		map_obj::_air_ship
		);
	++count;

	// マネージャーに追加
	MAP_MANAGER.AddObject(air_ship);

	return true;
}

// EOF