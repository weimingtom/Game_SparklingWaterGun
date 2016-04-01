#ifndef __MAP_CRANE_H__
#define __MAP_CRANE_H__

#include "../ParentAndChild.h"
#include "../GameSystem\GameBase.h"
#include "ObjectLoader.h"
#include "MapObject.h"
#include "MapObjectFactory.h"
#include "ObjectLoader.h"
#include "SwingObject.h"

class CraneSteelFrame;


//*************************************************************
// クレーンのボディクラス
//*************************************************************
class CraneBody 
	:public MapObjectInterface
{
	float                         rotary_speed_;
	Vector3                    arm_position_;
	CraneSteelFrame*   SteelFrame_;
	bool                         initialized_;
	int                            timer_;
public:
	CraneBody(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	virtual~CraneBody();

	// 更新・描画
	void Update()override;
	void Render()override;
	void Render_ShadowMap()override;

	// 初期化
	void Initialize(CraneSteelFrame* pSteelFrame);

	// 揺らす
	void AddForce(CRVector3 power);
	
	// 吊る場所取得
	Vector3 Get_ArmPosition()const { return arm_position_ + parameters_.pos; }
};

//*************************************************************
// クレーンに吊られているものクラス
//*************************************************************
class CraneSteelFrame 
	:public MapObjectInterface
{
	int								ride_timer;
	int                             timer_;
	float                          max_wire_length_;
	CraneBody*              MyBody_;
	D3DXQUATERNION   pose_;
	SwingObject<1>       swing_object_;

	// 現在の設計上、MeshManagerから受け取るときの
	// id がわからないからメッシュをCraneBodyと別にできないので
	// とりあえず自分で作成する
	LPIEXMESH               wire_mesh_;
	Matrix                        wire_scale_;
public:
	const float AddPowerSize = 5.5f;
public:
	CraneSteelFrame(
		CraneBody*             pBody,
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	virtual~CraneSteelFrame();

	// 揺らす
	void AddForce(CRVector3 force);

	// 更新・描画
	void Update()override;
	void Render()override;
	void Render_ShadowMap()override;

	// 自分の姿勢を変えたいときはここでする
	void RotateSelf()override;

	// 親の影響を受ける
	void ParentMove(
		LPCMATRIX pParentWorldMat,
		LPCMATRIX pParentWorldMatInv,
		LPCMATRIX pParentMoveMat,
		LPCMATRIX AffineMat
		)override;

	bool HandleMessage(
		LPGameMessage msg)override;
};


//*************************************************************
// クレーンを作成するクラス
//*************************************************************
class CraneFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __MAP_CRANE_H__

// EOF