#ifndef __CHARACTER_MESH_MANAGER_H__
#define __CHARACTER_MESH_MANAGER_H__

#include "iextreme.h"
#include "GameSystem\GameBase.h"
#include "Character.h"
#include "CODBO.h"

//******************************************************
//		キャラクタのモデル管理ヘッダー
//******************************************************

//************************************
//モーションを指定する部位の列挙
//************************************
namespace chr_motion_parts
{
	enum _ENUM_CHARACTER_MOTION_PARTS
	{
		ALL_PARTS,	//全部位
		LOWER_BODY,	//上半身
		UPPER_BODY,	//下半身

		_PARTS_MAX,
	};
}

typedef chr_motion_parts::_ENUM_CHARACTER_MOTION_PARTS	CHR_MOTION_PARTS;	//できればこっちをry

//************************************
//モーションをタイプの列挙
//************************************
namespace chr_motion_type
{
	enum _ENUM_CHARACTER_MOTION_TYPE
	{
		STAND = 1,			//棒立ち
		AIM = 28, //構えモーション

		MOVE_FRONT = 3,			//前歩き移動
		MOVE_RIGHT = 9,			//右歩き移動
		MOVE_LEFT = 7,			//左歩き移動
		MOVE_BACK = 5,			//後ろ歩き移動

		DIE = 22,			//死亡状態
		DYING,				//瀕死状態
		DYING_MOVE_FRONT,	//瀕死状態の前移動
		DYING_MOVE_RIGHT,	//瀕死状態の右移動
		DYING_MOVE_BACK,	//瀕死状態の後ろ移動
		DYING_MOVE_LEFT,	//瀕死状態の左移動

		THROW = 19,				//グレネード投げる
		DAMAGE_BODY,		//ぼこぼこ状態

		JUMP_START = 11,			//ジャンプスタート
		JUMP_MOVE_DOWN = 12,			//ジャンプスタート
		JUMP_END = 13,			//着地

		ROLL_START = 34,
		ROLLING = 35,
		ROLLIMPACTEND = 43,
		ROLLUSUALEND = 36,

		VANISH = 38,
		VANISH_FINISH = 39, 

		NATK_COLA = 19,			//コーラで殴る
		NATK_KICK = 20,			//蹴り

		SHUFFLE = 31,
		HOVER=44,
		HOVER_END = 45,

		APEER_VENDING_MACHINE=41,	//自販機からでてくる
	};
}

typedef chr_motion_type::_ENUM_CHARACTER_MOTION_TYPE CHR_MOTION_TYPE;	//できればこっちをry

namespace chr_bone_type
{
	enum _ENUM_CHARACTER_BONE_TYPE
	{
		HEAD_PULLTAB = 13,
		UPPER_BODY = 11,
		LOWER_BODY = 0,

		EYE_RIGHT = 4,
		EYE_LEFT = 4,

		RIGHT_ARM_1 = 9,
		RIGHT_ARM_2 = 10,
		RIGHT_ARM_3 = 12,
		RIGHT_ARM_WEAPON = 26,

		LEFT_ARM_1 = 14,
		LEFT_ARM_2 = 15,
		LEFT_ARM_3 = 17,
		LEFT_ARM_WEAPON = 31,

		RIGHT_LEG_1 = 19,
		RIGHT_LEG_2 = 20,
		RIGHT_LEG_3 = 3,

		LEFT_LEG_1 = 24,
		LEFT_LEG_2 = 25,
		LEFT_LEG_3 = 8,

		BODY_DAMAGE_1 = 2,
		BODY_DAMAGE_2 = 2,
		BODY_DAMAGE_3 = 2,
		BODY_DAMAGE_4 = 2, 

	};
}

typedef chr_bone_type::_ENUM_CHARACTER_BONE_TYPE CHARACTER_BONE_TYPE;

//************************************
//キャラクタのモデルベースクラス
//************************************
class CharacterModelBase
{
public:
	virtual ~CharacterModelBase(){}
public:
	//モデルを描画
	virtual void Render(LPCHARACTER C) = 0;
	virtual void Render(LPCHARACTER C, char* shader_name) = 0;

	virtual const Matrix& GetMatrix()const = 0;
	virtual void SetMatrix(CRMatrix mat) = 0;

	virtual void AnimationParts(
		CHR_MOTION_PARTS	parts,	//アニメーションする部位
		float				speed,	//経過スピード
		int					line_no	//モーションライン
		) = 0;

	virtual void Animation(float	speed) = 0;//経過スピード
	virtual void UsualAnimation() = 0;

	virtual void SetBodyAngle(CRVector3 v) = 0;

	virtual void SetMotion(
		CHR_MOTION_TYPE		motion,
		CHR_MOTION_PARTS	parts,
		int					line_no,
		RATIO				blend,	//ライン１のBlend比率
		bool				update
		) = 0;

	virtual void SetSpeed(
		CHR_MOTION_PARTS	parts,
		int					line_no,
		RATIO				speed
		) = 0;

	virtual void SetWeight(
		CHR_MOTION_PARTS	parts,
		int					line_no,
		RATIO				w
		) = 0;

	virtual void GetWeight(
		CHR_MOTION_PARTS	parts,
		RATIO*	pOutArray
		) = 0;

	virtual const Matrix& GetBoneMatrix(CHARACTER_BONE_TYPE type) = 0;

	virtual void SetMotionFrame(
		CHR_MOTION_PARTS	parts,
		int					line_no,
		float				frame) = 0;

	virtual float GetFrame(
		CHR_MOTION_PARTS	parts,
		int					line_no
		) = 0;

	virtual float GetMotionFrame(
		CHR_MOTION_PARTS	parts,
		int					line_no
		) = 0;
};

typedef CharacterModelBase* LPCHARACTERMODELBASE;

#endif