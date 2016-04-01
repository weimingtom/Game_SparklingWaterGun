#ifndef __MAP_OBJECT_MAIN_STAGE_H__
#define __MAP_OBJECT_MAIN_STAGE_H__

#include "MapObject.h"

//*************************************************************
// ���C���X�e�[�W�N���X
//*************************************************************
class MapObjectMainStage
	:public MapObjectInterface
{
public:
	MapObjectMainStage(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~MapObjectMainStage();
public:
	// �`��(�ړ��Ȃǂ��Ȃ��̂ōX�V�͖���)
	void	Render();
	void Render_ShadowMap();
};

//*************************************************************
// ���C���X�e�[�W�쐬�N���X
//*************************************************************
class MapObjectMainStageFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __MAP_OBJECT_MAIN_STAGE_H__

// EOF