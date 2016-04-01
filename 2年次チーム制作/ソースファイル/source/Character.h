#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "iextreme.h"
#include "ControllObject.h"
#include "Camera.h"
#include "ParentAndChild.h"
#include "CharacterAim.h"
#include "CharacterUI.h"
#include "CODBOForwardDecl.h"
#include "CharacterEye.h"

//****************************************************//
//　			キャラクタ基本パラメーター
//****************************************************//

typedef struct
{
	Matrix					m_trans_mat;		//姿勢行列
	Vector3					m_move;				//移動量
	Vector3					m_view_vec;			//視線ベクトル
	Vector3					m_parent_move;		//親の移動によって動かされた量
	ENTITY_ID				m_RideObjectId;		//乗っているオブジェクト
	float					m_LifePoint;		//体力
	float					m_Count;			//いろいろな処理のカウント
	ENTITY_ID				m_last_damage_parent;//最近受けたダメージの持ち主
	int						m_last_damage_count; //最近受けたダメージからの経過フレーム
	COLORf					m_OutLineColor;		//自身の輪郭線の色

	struct
	{
		float MoveSpeed;
		float ChargeSpeed;
		RATIO PowerUpAlpha;
	}
	UpValue;
}
CHRPARAM, *	LPCHRPARAM;

//****************************************************//
//　			キャラクタのフレームごとの情報
//****************************************************//
class  CHRFRAMEINFO
{
public:
	Vector3		pre_pos;		//１フレーム前の位置
	Vector3		frame_move;		//１フレームの移動
	bool		isHitWall;		//壁との接触情報
	bool		isHitFloor;		//床との接触情報
public:
	CHRFRAMEINFO();
	void Reset();
};

typedef const CHRFRAMEINFO& CRCHRFRAMEINFO;


//キャラクタの上方向は固定で！




const Vector3 CHARACTER_UPVEC(0, 1, 0);

//****************************************************//
//　キャラクタ行動ステート
//****************************************************//

namespace chr_move_state
{
	enum _ENUM_CHARACTER_MOVE_STATE_TYPE
	{
		error_mode = -1,

		usual_mode = 0,		//通常移動時
		jump_mode,			//ジャンプ時
		dying_mode,			//瀕死時
		die_mode,			//死亡時
		near_atk_mode,		//近距離攻撃
		throw_grenade,
		rolling,
		vanish,
		parachute,
		apeer_vending_machine,

		_state_max,
	};
}

typedef chr_move_state::_ENUM_CHARACTER_MOVE_STATE_TYPE CHARACTER_STATE_TYPE;

//****************************************************//
//　キャラクタクラス
//****************************************************//



class Character :public ControllObject,public CHILD
{
private:
	LPCHARACTERMOVESTATETABLE	m_pStateTable;			//このキャラクタのステートテーブル
protected:
	AimTargetCharacter			m_AimTarget;			//他のプレイヤーが狙うターゲット
	LPCHARACTER_UI				m_pUserInterface;		//このキャラクタのUI
	LPCHARACTERMODELBASE		m_pCharacterModel;		//キャラクタモデル
	CHRPARAM					m_Param;				//キャラクタ基本パラメータ
	LPCHARACTER_BRAINBASE		m_pBrain;				//このキャラクタを動かす脳
	CHARACTER_STATE_TYPE		m_State_type;			//現在実行しているステートのタイプ
	LPCHARACTERWEPONBASE		m_pWepon;				//所持している武器
	LPCHARACTER_LOCKON			m_pLockOnClass;			//現在のカメラのロックオン状態を保持するクラス
	CHRFRAMEINFO				m_frame_info;			//現在のフレームの情報
	CharacterEye				m_Eye;					//目の表示クラス
	R_InterFace_Character*		m_render_Interface;		//キャラクターの描画クラス
public:
	Character(
		ENTITY_ID					id,				//このキャラクタの一意な番号
		LPCHARACTER_BRAINBASE		m_pBrain,		//このキャラクタを動かす脳クラス
		LPCHARACTERMODELBASE		pCharacterModel,//このキャラクタのモデルクラス
		LPCHARACTER_UI				pUserInterface,
		int							team_type,
		TeamData*					tData
		);
	~Character();
private:
	//乗っているものが動いたときに呼ばれる関数
	void ParentMove(LPCMATRIX pParentWorldMat, LPCMATRIX pParentWorldMatInv, LPCMATRIX pParentMoveMat, LPCMATRIX AffineMat);
	void PowerUp(MSG_TYPE type, float value);
public:
	CRCHRFRAMEINFO GetFrameInfo()const;
	Vector3 GetBodyCenter();
	void Controll();							//このキャラクタを操作する関数
	void Update(TeamData& teamData);		//毎フレームの更新
	void Render(ControllObject& viewowner, TeamData& teamdata);								//描画
	void RenderUI(ControllObject& viewowner, TeamData& teamdata);							//UIを描画
	bool HandleMessage(LPGameMessage msg);		//このキャラクタへのメッセージを送る
	void Render_ShadowMap(ControllObject& viewowner, TeamData& teamdata);
	void TestFunction();
};



#endif