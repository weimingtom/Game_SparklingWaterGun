#include "../All.h"
#include "MapObjectMainStage.h"

//*************************************************************
// メインステージクラス
//*************************************************************
MapObjectMainStage::MapObjectMainStage(
	ENTITY_ID			id,
	ObjectParam		param,
	LPIEXMESH			mesh,
	MAP_OBJ_TYPE	type) :
	MapObjectInterface(id, param, mesh, type)
{

}

MapObjectMainStage::~MapObjectMainStage()
{

}

void MapObjectMainStage::Render()
{
	if (mesh_)
	{
		mesh_->TransMatrix = SRT_matrix_;
#if LIGHTMAP
		mesh_->Render(shader, "mainstage_lightmap");
#else
		mesh_->Render(shader, "mainstage_realtime_shadow");
#endif
	}
}

void	MapObjectMainStage::Render_ShadowMap()
{
	if (mesh_)
	{
		mesh_->TransMatrix = SRT_matrix_;
#if LIGHTMAP
		mesh_->Render(shader, "ShadowBuf_MainStage");
#else
		mesh_->Render(shader, "ShadowBuf");
#endif
	}
}


//*************************************************************
// メインステージ作成クラス
//*************************************************************
bool MapObjectMainStageFactory::CreateNewObject(const ObjectData &data)
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

	LPIEXMESH main_stage_mesh = 
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)(data.mesh_type));

	// 作成
	MapObjectMainStage* main_stage =
		new MapObjectMainStage(
		id,
		parameter,
		main_stage_mesh,
		map_obj::_main_stage
		);
	++count;

	// 登録
	MAP_MANAGER.AddObject(main_stage);

	return true;
}

// EOF