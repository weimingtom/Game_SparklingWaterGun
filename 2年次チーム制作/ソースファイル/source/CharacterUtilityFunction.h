#ifndef __CHARACTER_UTILITY_FUNCTION_H__
#define __CHARACTER_UTILITY_FUNCTION_H__

#include "Character.h"
#include "CharacterModel.h"
#include "CharacterModelResource.h"
#include "CODBOForwardDecl.h"
#include "FriendMessage.h"


class	CharacterUtilityFunctions;
typedef CharacterUtilityFunctions	ChrFunc;

struct DamageParam;

class CharacterUtilityFunctions: public Character
{
private:
	CharacterUtilityFunctions();
	~CharacterUtilityFunctions();
public:
	//初期化
	static void						InitCharacterParam(LPCHRPARAM pOut);	//全項目を初期化
	static void						CreateTransMatrix(Matrix& out, CRVector3 front,CRVector3 pos);
public:
	//ゲッター
	static CHARACTER_STATE_TYPE		GetStateType(LPCharacter C);	//現在の行動ステートタイプを得る
	static const CHRPARAM&			GetParam(LPCharacter C);		//パラメーター所得
	static CRVector3				GetFront(LPCharacter C);		//前方向ベクトルを得る
	static Vector3					GetRight(LPCharacter C);		//右方向ベクトルを得る
	static float					GetAngle(LPCharacter C);		//向いているY軸の角度を得る(正規化済み)
	static const LPCAMERA			GetCamera(LPCharacter C);		//キャラクタの持っているカメラ情報を得る
	static LPCHARACTERMODELBASE		GetModelPtr(LPCharacter C);		//キャラクタのモデルポインタを得る
	static LPCHARACTERWEPONBASE		GetWepon(LPCharacter C);		//キャラクタの武器を得る
	static LPCHARACTER_UI			GetUI(LPCharacter C);
	static RATIO					GetChargePower(LPCharacter C);
	static void						GetBonePos(LPCHARACTER C, CHARACTER_BONE_TYPE bone_type, Vector3& out);
	static void						GetBoneLocalPos(LPCHARACTER C, CHARACTER_BONE_TYPE bone_type, Vector3& out);
public:
	//便利関数たち
	static void						GetTransMatrix(LPCharacter C, Matrix& out, CRVector3 scale);
	static Vector3					GetMyCameraFront(LPCharacter C);//自分のカメラの前方向ベクトルを得る
	static Vector3					GetMyCameraRight(LPCharacter C);//自分のカメラの右方向ベクトルを得る
public:
	//ダメージ関連
	static void						CreateToBrainDamageMsg(LPCHARACTER	C, LPGameMessage msg, DamageParam* pDamage);
	static void						SetCureValue(LPCharacter C,float val);		//回復
	static void						SetDamageValue(LPCharacter C, float val);	//ダメージ
	static bool						isDead(LPCharacter C);	//死んでいるかどうか
	static bool						isDying(LPCharacter C);	//瀕死かどうか
	static bool						HitCheck(LPCharacter C, CRVector3 hitball, float hitballsize);	//キャラクタと球の衝突判定
	static void						UpdateDamageLifePoint(LPCharacter C);	//非ダメージ時間カウントの更新と体力の回復
public:
	//作成と消去関連
	static bool						GetRandomRespornPoint(TEAM_TYPE team, LPMapObjectInterface* ppOut);
	static void						SetStartCharacter(LPCHARACTER C, LPCONTROLLER cnt);
	static void						ApperVendingMachine(LPCHARACTER C, CRVector3 pos, float angle);
	static LPCONTROLLOBJCT			RespornCharacter(LPCHARACTER C);	//キャラクタリスポーン
	static void						DeleteCharacter(LPCHARACTER C);		//キャラクタを抹消する
	static void						SetAutoAimPointEnable(LPCHARACTER C, bool flg);
public:
	static bool						HitCheckOtherControllObj(LPCHARACTER C);
public:
	//武器関係
	static bool						isHitColaGun(CRVector3 p);
	static bool						isHitColaGlenade(CRVector3 p);		//現在の
	static bool						isShuffle(LPCharacter C);		//振っているかどうか
	static bool						isZoom(LPCharacter C);
	static bool						WeponFire(LPCHARACTER C);
	static float					GetMaxChargeWeaponRange(LPCHARACTER C);
	static float					GetMinChargeWeaponRange(LPCHARACTER C);
	static float					GetNowChargeRange(LPCHARACTER C);
	static void						SetGaugeMove(LPCHARACTER C,CRVector2 pos, CRVector2 size);
	static void						ResetGauge(LPCHARACTER C);
public:
	//ルール関連
	static bool						isFrend(LPCHARACTER me, LPCHARACTER you);	//meにとってyouが味方かどうか
public:
	//モーション関連
	static void						ResetMotion(LPCHARACTER me, CHR_MOTION_PARTS parts);
	static void						SetMotion(LPCHARACTER me, CHR_MOTION_PARTS parts,CHR_MOTION_TYPE motion);
	static void						SetMotionSpeed(LPCHARACTER me, CHR_MOTION_PARTS parts, float speed);
	static bool						SetWalkingMotion(LPCHARACTER me, CRVector2 move_v, RATIO speed);
public:
	//能力値関係
	static void						SetOutLineColorHP(LPCHARACTER me);
	static void						SendMoveSpeedUpMsg(ENTITY_ID to);
	static void						SendChargeSpeedUpMsg(ENTITY_ID to);
	static void						SetPlusAlpha(LPCHARACTER me, RATIO a);
	static void						SetPowerUpEffect(LPCHARACTER me, MSG_TYPE type);
	static void						UpdateUpValueAlpha(LPCHARACTER me);
public:
	static bool						PlayFootSound(LPCHARACTER me, CHARACTER_BONE_TYPE& out_touch_foot);
public:
	static void						SendFriendIconMsgToBrain(LPCHARACTER me, FriendMessageIcon::MsgType type);
public:
	//描画関連
	static void						RenderEye(LPCHARACTER me);
	static void						ApperEmptyCan(CRVector3 pos, float size, TEAM_TYPE type);
};


#endif