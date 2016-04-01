#include "../All.h"
#include "MapObjectSignboard.h"

//*************************************************************
// �ŔN���X
//*************************************************************

MapObjectSignboard::MapObjectSignboard(
	ENTITY_ID               id,
	ObjectParam            param,
	LPIEXMESH              mesh,
	MAP_OBJ_TYPE       type
	) :
	MapObjectInterface(id, param, mesh, type),
	rotary_speed_(0.003f)
{

}


MapObjectSignboard::~MapObjectSignboard()
{

}


void MapObjectSignboard::Update()
{
	parameters_.angle.y += rotary_speed_;

	MapObjectInterface::Update();
}


void	MapObjectSignboard::Render()
{
	return;
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


void	MapObjectSignboard::Render_ShadowMap()
{
	return;
	if (mesh_)
	{
		mesh_->TransMatrix = SRT_matrix_;
		mesh_->Render(shader, "ShadowBuf");

	}
}

//*************************************************************
// �Ŕ쐬�N���X
//*************************************************************

bool MapObjectSignboardFactory::CreateNewObject(const ObjectData &data)
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

	LPIEXMESH signboard_mesh = 
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)(data.mesh_type));

	// �쐬
	MapObjectSignboard* signboard =
		new MapObjectSignboard(
		id,
		parameter,
		signboard_mesh,
		map_obj::_signboard
		);
	++count;

	// �o�^
	MAP_MANAGER.AddObject(signboard);

	return true;
}

