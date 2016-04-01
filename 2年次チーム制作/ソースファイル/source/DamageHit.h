#ifndef __DAMAGE_HIT_H__
#define __DAMAGE_HIT_H__

#include "iextreme.h"
#include "ppSpatialPartitionning.h"
#include "GameSystem\GameBase.h"

class DamageObjectManager;
//*******************************************************
//	ダメージクラス
//*******************************************************

//ダメージ識別フラグ
typedef DWORD DAMAGEFLAGS;

//ビットによるタイプ分け
namespace damage_type_bits
{
	enum
	{
		//誰へのダメージか
		all_character_cure =	(1 << 30),	//敵味方問わず回復
		team_cure_damage =		(1 << 29),	//味方を回復させるダメージ
		all_character_damage =	(1 << 28),	//敵味方問わずダメージ
		enemy_damage =			(1 << 27),	//敵へのダメージ

		//オプション
		except_myself =			(1 << 26),	//自分は除く
		apper_crushed_mesh =	(1 << 25),

		//バリエーション
		to_dying_damage =		(1 << 24),	//瀕死状態に(既になっている場合はそのまま)
		blow_off_damage =		(1<<23),	//吹き飛ぶ
	};
}

#define IS_TYPE_DAMAGE(flags,damage_type) ((flags)&(damage_type))

//ダメージパラメータ
struct DamageParam
{
	ENTITY_ID		parent;		//製造者ID
	DAMAGEFLAGS		type;		//ダメージ識別フラグ
	float			val;		//値
	int				hit_count;	//あたった回数
	Vector3			vec, pos;	//いちと向き
	COLOR			color;
};

//ダメージクラス
class DamageObject
{
private:
	DamageObjectManager*	pMgr;
	lp_sp_object			sp_obj;
	Vector3					pos;
	float					size;
public:
	DamageParam				param;
public:
	DamageObject(DamageObjectManager*	pMgr);
	~DamageObject();
public:
	void Apper();	//ダメージ判定を有効にする
	void Exit();	//ダメージ判定を無効にする
public:
	bool isEnable()const;
	CRVector3 GetPos()const;
	float GetSize()const;
	void SetPos(CRVector3 pos);	
	void SetSize(float s);
};

//*******************************************************
//	ダメージマネージャ
//*******************************************************
typedef std::vector<DamageObject*> DAMAGEVECTOR;

class DamageObjectManager:public GameBaseEntity
{
private:
	lp_sp_field m_pField;
public:
	DamageObjectManager();
	~DamageObjectManager();
public:
	void RenderDebugParticle();
	lp_sp_field	GetField()const;
	DAMAGEVECTOR GetInDamage(CRVector3 pos, float size);	//範囲を矩形にとるので注意
	bool HandleMessage(LPGameMessage msg);
};

//ダメージ識別便利関数
namespace damage_check
{
	bool isCureDamage(LPCONTROLLOBJCT me, const DamageParam& param);
	bool isEnemyDamage(LPCONTROLLOBJCT me, const DamageParam& param);
}

#endif