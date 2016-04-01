#ifndef __ELEVATOR_H__
#define __ELEVATOR_H__

#include "../ParentAndChild.h"
#include "../GameSystem\GameBase.h"
#include "ObjectLoader.h"
#include "MapObject.h"
#include "MapObjectFactory.h"
#include "ObjectLoader.h"


//*************************************************************
// エレベータークラス
//*************************************************************
class Elevator
	:public MapObjectInterface
{
protected:
	enum
	{
		_wait_root,              // k_root_pos_で待つ
		_wait_target,           // k_target_pos_で待つ
		_move_to_target,    // k_root_pos_に向かって動く
		_move_to_root,       // k_target_pos_に向かって動く
	};

	int                    state_;               // 現在の状態
	int                    timer_;               // タイマー
	const Vector3   k_target_pos_;  // 往復先
	const Vector3   k_root_pos_;     // 往復元
	Vector3            move_;               // 移動量
public:
	Elevator(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~Elevator();

public:
	// 更新・描画
	void Update()override;
	void Render()override;
	void Render_ShadowMap()override;

protected:
	// ステート更新
	void UpdateState();
	
	// ステート関数
	void WaitRoot();
	void WaitTarget();
	void MoveToRoot();
	void MoveToTarget();

};

//*************************************************************
// エレベーター作成クラス
//*************************************************************
class ElevatorFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};


#endif // __ELEVATOR_H__

// EOF