#ifndef __MAP_OBJECT_MAIN_STAGE_H__
#define __MAP_OBJECT_MAIN_STAGE_H__

#include "MapObject.h"

//*************************************************************
// メインステージクラス
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
	// 描画(移動などしないので更新は無し)
	void	Render();
	void Render_ShadowMap();
};

//*************************************************************
// メインステージ作成クラス
//*************************************************************
class MapObjectMainStageFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __MAP_OBJECT_MAIN_STAGE_H__

// EOF