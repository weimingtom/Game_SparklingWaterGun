#ifndef __MAP_OBJECT_SIGNBOARD_H__
#define __MAP_OBJECT_SIGNBOARD_H__

#include "MapObject.h"

//*************************************************************
// �ŔN���X
//*************************************************************
class MapObjectSignboard
	:public MapObjectInterface
{
	const float rotary_speed_; // ��]���x
public:
	MapObjectSignboard(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~MapObjectSignboard();
public:
	// �X�V�E�`��
	void Update()override;
	void	Render()override;
	void Render_ShadowMap()override;
};

//*************************************************************
// �Ŕ쐬�N���X
//*************************************************************
class MapObjectSignboardFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __MAP_OBJECT_SIGNBOARD_H__

// EOF