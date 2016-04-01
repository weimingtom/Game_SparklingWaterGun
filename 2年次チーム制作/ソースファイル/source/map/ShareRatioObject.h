#ifndef __SHARE_RATIO_OBJECT_H__
#define __SHARE_RATIO_OBJECT_H__

#include "ShareRatioObjectAim.h"
#include "MapObject.h"
#include "../DamageHit.h"
#include "MapObjectParam.h"
#include "../ShareObjIcon.h"
#include "MapObjectFactory.h"

//*************************************************************
// シェアオブジェクト基本クラス
//*************************************************************

class ShareRatioObjectBase 
	:public MapObjectInterface
{
protected:
	ShareObjIcon*              m_pShareObjIcon;  // アイコン
	ShareRatioParam          m_ShareRatio;        // シェアパラメータ
	ShareRatioObjectAim    aim_target_;           // エイムターゲット
	float                              collision_size_;        // 判定のサイズ
	Vector3                         damage_pos_;        // 判定の中心座標
public:
	ShareRatioObjectBase(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	virtual~ShareRatioObjectBase();
private:
	void UpdateShareValueCure();
	bool CalcHitDamage(DamageObject* p);
	void UpdateColor();
	// ダメージ判定
	DAMAGEFLAGS HitCheck();
	static bool SetChangeSourEffect(CRVector3 pos, ENTITY_ID parentId);
protected:
	static void SetChangeShareIcon(CRVector3 pos, ENTITY_ID parentId);
	void SetChangeTeamEffect(
		bool set_water_pool = true,
		bool set_smoke = true
		);
	virtual void ChangeTeam(DamageObject* p) = 0;
	virtual void Hit(DamageObject* p) = 0;
public:
	static RATIO GetSeeThroughAlpha();
	// 更新
	virtual void Update()override;
	virtual Vector3 Get_MoveParameter()const{ return VECTOR3ZERO; };
	// ゲッタ
	CRVector3 Get_DamagePos()const{ return damage_pos_; }
	const ShareRatioParam& Get_ShareParam()const;

};

//*************************************************************
// ポスタークラス
//*************************************************************

class PosterShareRatioObject 
	:public ShareRatioObjectBase
{
public:
	PosterShareRatioObject(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~PosterShareRatioObject();
public:
	void Hit(DamageObject* p)override;

	// チーム変更
	void ChangeTeam(DamageObject* p)override;

	// 親の影響を受ける
	void ParentMove(
		LPCMATRIX pParentWorldMat,
		LPCMATRIX pParentWorldMatInv,
		LPCMATRIX pParentMoveMat,
		LPCMATRIX AffineMat);

	// 更新・描画
	void Update()override;
	void Render()override;
	void Render(char* shaderName)override;
	void Render_ShadowMap()override;

	//RayPick
	virtual bool RayPick(
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
// ポスター作成クラス
//*************************************************************
class PosterFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __SHARE_RATIO_OBJECT_H__

// EOF