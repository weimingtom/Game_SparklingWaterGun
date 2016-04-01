#include "../All.h"
#include "MapObjectFactory.h"


//*************************************************************
// マップオブジェクト作成クラス
//*************************************************************

int MapObjectFractory::count = 0;

void MapObjectFractory::InitCount()
{
	count = 0;
}

bool MapObjectFractory::CreateNewObject(const ObjectData &data)
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

	LPIEXMESH mesh =
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)(data.mesh_type));

	// 作成
	MapObjectInterface* obj =
		new MapObjectInterface(
		id,
		parameter,
		mesh,
		(MAP_OBJ_TYPE)(data.type)
		);
	++count;

	// 登録
	MAP_MANAGER.AddObject(obj);

	return true;
}

// EOF