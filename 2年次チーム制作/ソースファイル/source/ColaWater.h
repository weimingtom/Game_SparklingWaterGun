#ifndef __COLA_WATER_H__
#define __COLA_WATER_H__

#include "ColaBullet.h"
#include "ppSpatialPartitionning.h"
#include "DamageHit.h"

//*****************************************************************
//	コーラ弾丸クラスのヘッダ
//*****************************************************************

//コーラ弾丸の基本的なパラメータ
struct ColaWaterParam
{
	Vector3			pos, move, power;
	Vector3			m_goal_position;//着弾予想地点
	Vector3			m_goal_normal;
	int				m_live_timer;	//消去時間カウント
	float			m_scale;
	COLOR			color;
	ENTITY_ID		HitObjectId;
};

//コーラ弾丸クラス
class ColaWater :public BulletBase
{
private:
	static int			ApperTagCount;
	static int			UpdateTag;
	static const int	UpdateFrame = 10;
	static LVERTEX		lVertex[4];
private:
	EffectClassAnimationNMove*	m_pEffect;
	ColaWaterParam				m_param;		//パラメーター構造体
	bool						m_DeleteFlag;	//消去するかどうかのフラグ
	const int					m_Updatetag;	//更新順序
	const int					m_live_time;	//消去時間
	PolygonEffect::INDEX		m_effect_index;
public:
	DamageObject				m_damage_obj;	//ダメージクラス
public:
	ColaWater(
		const ColaWaterParam&	param,
		int						liveTime,
		LPIEXMESH				pMesh);
	~ColaWater();
private:
	bool HitCheck();
	void GoalPosUpdate();
public:
	static void AddUpdateTag();
	static void SetCameraVector();
	void GetMoveAndPos(Vector3& move, Vector3& pos);
	bool isDelete();
	void Update();
	void Render();
};



#endif