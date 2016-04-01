#ifndef __MAP_OBJECT_SKY_H__
#define __MAP_OBJECT_SKY_H__

#include "MapObject.h"

//*************************************************************
// ��N���X
//*************************************************************
class MapObjectSky 
	:public MapObjectInterface
{
public:
	MapObjectSky(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~MapObjectSky();
public:
	// �X�V�E�`��
	void Update()override;
	void	Render()override;
	void Render_ShadowMap()override;

	//RayPick
	bool RayPick(
		Vector3* out,
		Vector3* pos,
		Vector3*vec,
		float* dist
		)override
	{
		return false;
	}
};

//*************************************************************
// ��쐬�N���X
//*************************************************************
class MapObjectSkyFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __MAP_OBJECT_SKY_H__

// EOF