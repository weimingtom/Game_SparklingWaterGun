#include "../All.h"
#include "Sky.h"

//*************************************************************
// 空クラス
//*************************************************************

MapObjectSky::MapObjectSky(
	ENTITY_ID               id,
	ObjectParam            param,
	LPIEXMESH              mesh,
	MAP_OBJ_TYPE       type
	) :
	MapObjectInterface(id, param, mesh, type)
{

}


MapObjectSky::~MapObjectSky()
{

}


void MapObjectSky::Update()
{
	//parameters_.pos = Vector3MulMatrix(Vector3(0,0,0), matView);

	MapObjectInterface::Update();
}


void	MapObjectSky::Render()
{
	//適応
	if (mesh_)
	{
		mesh_->TransMatrix = SRT_matrix_;
#if LIGHTMAP
		mesh_->Render(shader, "mainstage_lightmap");
#else
		mesh_->Render(shader, "truck");
#endif
	}
}


void	MapObjectSky::Render_ShadowMap()
{

}


//*************************************************************
// 空作成クラス
//*************************************************************

bool MapObjectSkyFactory::CreateNewObject(const ObjectData &data)
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

	LPIEXMESH sky_mesh = 
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)(data.mesh_type));

	// 作成
	MapObjectSky* sky =
		new MapObjectSky(
		id, 
		parameter, 
		sky_mesh, 
		map_obj::_sky
		);
	++count;

	// 登録
	MAP_MANAGER.AddObject(sky);

	return true;
}

// EOF