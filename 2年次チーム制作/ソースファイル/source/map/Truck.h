#ifndef __MAP_TRUCK_OBJECT_H__
#define __MAP_TRUCK_OBJECT_H__

#include "ObjectLoader.h"
#include "MapObject.h"
#include "../DamageHit.h"
#include "MapObjectFactory.h"
#include "WayPointObject.h"
#include "ShareRatioObject.h"

// 角度コントロール
void angleContRoll(CRVector3 MyPos, CRVector3 ViewPos, float *angle, float MaxSpped);

//*************************************************************
// トラッククラス
//*************************************************************
class Truck
	:public ShareRatioObjectBase
{
	Vector3                  target_pos_;           // ターゲットの座標(ここに向かって移動する)
	float                       speed_;                   // 移動速度
	Vector3                  move_;                   // 移動量
	WayPointObject*   target_point_;         // ターゲットのWayPointObject
	DamageObject       damage_object_;
	bool                       is_step_brakes_;     // true : 減速   false : 加速
public:
	Truck(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~Truck();
public:
	// 更新・描画
	void Update()override;
	void Render()override;
	void Render_ShadowMap()override;

	//RayPick
	bool RayPick(
		Vector3* out,
		Vector3* pos,
		Vector3*vec,
		float* dist
		)override;

	// オブジェクトの移動量取得
	Vector3 Get_MoveParameter()const override{ return move_; }
	
	void Hit(DamageObject* p)override;

	// チーム変更
	void ChangeTeam(DamageObject* p)override;
private:
	// 前に味方がいるかどうか
	bool Is_FriendForward();
	// 前にほかのトラックがいるかどうか
	bool Is_TruckForward();
	// 前に何かがあると止まる
	void CheckForward();

	// 速度更新
	void UpdateSpeed();

	void UpdateTarget();
	void UpdateAngle();
};

//*************************************************************
// トラック作成クラス
//*************************************************************
class TruckFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __MAP_TRUCK_OBJECT_H__

// EOF