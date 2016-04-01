#include "../All.h"
#include "MapObjectFactory.h"


//*************************************************************
// �}�b�v�I�u�W�F�N�g�쐬�N���X
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

	// �쐬�ɕK�v�Ȃ��̂�p�ӂ���

	ObjectParam parameter;
	parameter.pos = data.pos;
	parameter.angle = data.angle;
	parameter.scale = data.scale;

	ENTITY_ID id = 
		(ENTITY_ID)(game_id::id_map_object_min + count);

	LPIEXMESH mesh =
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)(data.mesh_type));

	// �쐬
	MapObjectInterface* obj =
		new MapObjectInterface(
		id,
		parameter,
		mesh,
		(MAP_OBJ_TYPE)(data.type)
		);
	++count;

	// �o�^
	MAP_MANAGER.AddObject(obj);

	return true;
}

// EOF