#include "../All.h"
#include "Sky.h"

//*************************************************************
// ��N���X
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
	//�K��
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
// ��쐬�N���X
//*************************************************************

bool MapObjectSkyFactory::CreateNewObject(const ObjectData &data)
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

	LPIEXMESH sky_mesh = 
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)(data.mesh_type));

	// �쐬
	MapObjectSky* sky =
		new MapObjectSky(
		id, 
		parameter, 
		sky_mesh, 
		map_obj::_sky
		);
	++count;

	// �o�^
	MAP_MANAGER.AddObject(sky);

	return true;
}

// EOF