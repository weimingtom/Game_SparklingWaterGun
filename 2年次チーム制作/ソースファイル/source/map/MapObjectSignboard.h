#ifndef __MAP_OBJECT_SIGNBOARD_H__
#define __MAP_OBJECT_SIGNBOARD_H__

#include "MapObject.h"

//*************************************************************
// 看板クラス
//*************************************************************
class MapObjectSignboard
	:public MapObjectInterface
{
	const float rotary_speed_; // 回転速度
public:
	MapObjectSignboard(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~MapObjectSignboard();
public:
	// 更新・描画
	void Update()override;
	void	Render()override;
	void Render_ShadowMap()override;
};

//*************************************************************
// 看板作成クラス
//*************************************************************
class MapObjectSignboardFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __MAP_OBJECT_SIGNBOARD_H__

// EOF