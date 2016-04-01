#ifndef __AIR_SHIP_H__
#define __AIR_SHIP_H__

#include "../ParentAndChild.h"
#include "../GameSystem\GameBase.h"
#include "ObjectLoader.h"
#include "MapObject.h"
#include "MapObjectFactory.h"
#include "ObjectLoader.h"


//*************************************************************
// 飛行船クラス
//*************************************************************
class AirShip
	:public MapObjectInterface
{
private:
	const Vector3 rotation_center_;    // 回転の中心
	float                rotation_radius_;    // 回転の半径
	float                rotation_speed_;    // 角速度 ※1フレーム
	float                rotation_angle_;     // 中心からの回転角度
public:
	AirShip(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~AirShip();

public:
	// 更新・描画
	void Update()override;
	void Render()override;
	void Render_ShadowMap()override;
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
// 飛行船作成クラス
//*************************************************************
class AirShipFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __AIR_SHIP_H__

// EOF