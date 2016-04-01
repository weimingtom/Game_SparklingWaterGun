#ifndef __CHARACTER_MOVE_H__
#define __CHARACTER_MOVE_H__

#include "Character.h"
#include "CODBOForwardDecl.h"
#include "FriendMessage.h"

//*****************************************************
//	キャラクタの動きに関する関数をまとめたクラス
//*****************************************************

class	CharacterMoveFunctions;
typedef CharacterMoveFunctions	ChrMove;
typedef DWORD DAMAGEFLAGS;

class CharacterMoveFunctions : private Character
{
private:
	CharacterMoveFunctions();
	~CharacterMoveFunctions();
private:
	static bool					UpCheck(LPCHARACTER C);
public:
	//毎フレーム普段更新する関数のまとめ
	static bool					UpdateAllMoveFunc(LPCHARACTER C, float max_speed_val, RATE down_speed_val, float glavity);	//戻り値は地面についているか
	static void					UpdateAllDamageFunc(LPCHARACTER C);
	static void					UpdateAimTargetInfo(LPCHARACTER C, LPTEAMVIEWDATA viewData);
	static bool					UpdateParachuteMoveFunc(LPCHARACTER C, float max_speed_val, float down_speed);
public:
	//移動関係
	static void					AddMove(LPCHARACTER C, CRVector3 move);		//指定した方向に移動量を加算する
	static void					PositionUpdate(LPCHARACTER C);				//座標に移動量を加算する
	static void					DownSpeed(LPCHARACTER C, RATE val);			//移動量をvalの割合だけ減少させる
	static void					SetMaxXZSpeed(LPCHARACTER C, float max_val);//XZ平面の最大移動量をmax_valに丸める
	static bool					FloorCheck(LPCHARACTER C);					//床とのあたり判定チェック
	static bool					CeilingCheck(LPCHARACTER C);
	static bool					WallCheck(LPCHARACTER C);					//壁とのあたり判定チェック
	static bool					WallCheckGetNormal(LPCHARACTER C, Vector3& out_normal);//壁とのあたり判定チェック(あたった壁の法線も得る)
	static void					Bound(LPCHARACTER C,CRVector3 normal, RATIO r);
	static bool					Hover(LPCHARACTER C, RATIO power);
public:
	//角度とか向き関係
	static void					AngleSet(LPCHARACTER C, CRVector3 viewvec);//指定した方向に向かせる(一瞬で)
	static void					AngleControll(LPCHARACTER C, CRVector3 view_pos, RADIAN speed);//指定した場所に向かってspeedの速度で向く
	static void					AngleControllAxisY(LPCHARACTER C, CRVector3 view_pos, RADIAN speed);//指定した場所に向かってspeedの速度で向く
	static void					CheckMaxViewUpAndDown(LPCHARACTER C, Vector3& v);	//キャラクタが上下に向きすぎていた場合に補正する
public:
	//あたり判定関係
	static void					Vanish(LPCHARACTER C, CRVector3 move);
	static DAMAGEFLAGS			UsualDamageCheck(LPCHARACTER C);
	static void					SetDamageEffect(LPCHARACTER C, DAMAGEFLAGS flgs, const DamageParam& dParam);
public:
	//状態遷移関係
	static CHARACTER_STATE_TYPE	DamageStateUpdate(LPCHARACTER C, DAMAGEFLAGS flgs);
	static void					Shuffle(LPCHARACTER C,bool flg);			//シャカシャカするかどうか
	static void					LockOn(LPCHARACTER C, bool flg);			//ロックオンするかどうか
	static void					LockOnRLChange(LPCHARACTER C);			//ロックオン時のカメラ左右切り替え
public:
	static void					AddShuffleGauge(LPCHARACTER C,RATIO power);
public:
	static void					SetExplositonBlur(CRVector3 pos, RATIO power);
	static void					SetExplositon(CRVector3 pos, RATIO power, LPCHARACTER C);
	static void					SetExplositon(CRVector3 pos, RATIO power, const DamageParam& param, float damage_size);
public:
	//アイコン
	static void					SetFriendMessegeIcon(LPCHARACTER C, FriendMessageIcon::MsgType type);
};

#endif
