#ifndef __CHARACTER_MOVE_STATE_H__
#define __CHARACTER_MOVE_STATE_H__

#include "Character.h"
#include "CameraMove.h"
#include "AimPoint.h"
#include "DamageHit.h"

class CharacterMoveStateTable;
typedef CharacterMoveStateTable* LPCHARACTERMOVESTATETABLE;

//********************************************************
//	ステートに送るメッセージ情報
//********************************************************

namespace chr_state_msg_type
{
	enum _ENUM_CHARACTER_STATE_MESSAGE_TYPE
	{
		_error_type = -1,

		send_move_vector,	//v1に移動量ベクトルを格納して送る
		send_view_position,	//v1に視線ベクトルを格納して送る
		send_jump_state,	//v1.xにステート番号を入れて送る
		send_lock_on_flg,	//v1.xにフラグ 0 or !=0	を入れて送る
		send_lock_on_change_right_left,	//なにもいれなくてよし
		send_wepon_fire_flg,//なにもいれなくてよし
		send_shuffle_flg,	//v1.xにフラグ 0 or !=0	を入れて送る
		send_vanish_vec,	//v1に移動量ベクトルを格納して送る
		send_hover,			//なにもいれなくてよし
	};
}
typedef chr_state_msg_type::_ENUM_CHARACTER_STATE_MESSAGE_TYPE CHRSTATE_MSGTYPE;

//	ステートにexinfoとして送る構造体
class CharacterStateMessage
{
public:
	CharacterStateMessage() :type(chr_state_msg_type::_error_type), v1(0,0,0), v2(0,0,0), v3(0,0,0){}
public:
	//命令名
	CHRSTATE_MSGTYPE type;
	//ベクトル情報
	Vector3	v1, v2, v3;
};

typedef CharacterStateMessage	*LPCHARACTERSTATEMESSAGE;

//**********************************************************
//	ロックオンクラス
//**********************************************************
class CharacterLockOn
{
private:
	enum CamState
	{
		_USUAL,
		_ZOOM,
		_SHUFFLE
	};
	CamState						m_State;
	AIMTARGET_ID					m_Target;
	LPCHARACTER						m_pOwner;
	CameraFunction::RIGHTorLEFT		m_camera_pos;
public:
	CharacterLockOn(LPCHARACTER p);
public:
	bool isZoom()const;
	bool isShuffle()const;
	AIMTARGET_ID GetTarget();
	void LockOn(bool flg);
	void Shuffle(bool flg);
	void ChangeRL();
	void Update();
};

//**********************************************************
//	shuffleクラス
//**********************************************************
class CharacterShuffle
{
private:
	bool		m_isShuffle;
	LPCHARACTER	m_pOwner;
public:
	CharacterShuffle(LPCHARACTER	pOwner);
	void Update();
	bool isShuffle()const;
	void ChangeShuffle();
	void SetShuffle(bool flg);
};

//******************************************************
//	キャラクタの行動ステートの基底クラス
//******************************************************
class CharacterMoveStateBase
{
protected:
	LPCHARACTER m_pOwner;
public:
	CharacterMoveStateBase(LPCHARACTER pOwner);
	virtual ~CharacterMoveStateBase(){}
public:
	virtual void Enter() = 0;	//ステート実行開始時に呼ばれる
	virtual void Execute() = 0;	//ステート実行中に呼ばれる
	virtual void Exit() = 0;	//ステート実行終了時に呼ばれる
	virtual void Render(){};
	virtual void UpdateTransMatrix();
	virtual bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type) = 0;	//引数のステートに現在変更できるかどうか
	virtual bool HandleMessage(LPCHARACTERSTATEMESSAGE msg) = 0;	//メッセージ受信関数
};

//******************************************************
//	キャラクタの自由移動クラス
//******************************************************
class CharacterMoveStateUsualMode :public CharacterMoveStateBase
{
	/*
		命令名　send_move_vector 	仕様 v1に入っているベクトルを現在の移動量に加算する
		命令名　send_view_position 	仕様 v1に入っているベクトルを現在の視点方向にセットする
	*/
private:
	enum STATE_TYPE
	{
		_USUAL,
		_SHUFFLE,
	};
private:
	int				foot_sound_timer;
	STATE_TYPE		now_state;
public:
	CharacterMoveStateUsualMode(LPCHARACTER p);
public:
	void Enter();
	void Execute();
	void Exit();
	bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type);
	bool HandleMessage(LPCHARACTERSTATEMESSAGE msg);
};

//******************************************************
//	キャラクタのジャンプモードクラス
//******************************************************
class CharacterMoveStateJumpMode :public CharacterMoveStateBase
{
	/*
	命令名　send_move_vector 	仕様 v1に入っているベクトルを現在の移動量に加算する
	命令名　send_view_position 	仕様 v1に入っているベクトルを現在の視点方向にセットする
	*/
private:
	enum JUMPSTEP
	{
		START,
		MOVEUP,
		MOVEDOWN,
		FINISH
	};
private:
	JUMPSTEP		m_step;
	int				m_frame;
	const Vector3	m_glavity;
	float			m_max_xz_speed;
	const float		m_jump_power;
	const int		m_jump_frame;
	const int		m_jump_end_frame;
	int				m_Sound_Timer;
	bool			m_UsedHover;
	int				m_UsedHoverCount;
public:
	CharacterMoveStateJumpMode(LPCHARACTER p);
private:
	//ジャンプの処理わけ関数
	void JumpStartMode();
	void JumpMoveUpMode();
	void JumpMoveDownMode();
	void JumpFinishMode();
private:
	void ChangeStep(JUMPSTEP step);	//ステップ変更関数
public:
	void Enter();
	void Execute();
	void Exit();
	bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type);
	bool HandleMessage(LPCHARACTERSTATEMESSAGE msg);
};

//******************************************************
//	キャラクタの瀕死状態の移動クラス
//******************************************************
class CharacterMoveStateDyingMode :public CharacterMoveStateBase
{
	/*
	命令名　send_move_vector 	仕様 v1に入っているベクトルを現在の移動量に加算する
	命令名　send_view_position 	仕様 v1に入っているベクトルを現在の視点方向にセットする
	*/
private:
	typedef void(CharacterMoveStateDyingMode::*DyingModeStep)();
	DyingModeStep		step;
	int					timer;
public:
	CharacterMoveStateDyingMode(LPCHARACTER p);
private:
	void step_begin();
	void step_move();
	void step_end();
private:
	void ChangeStep(DyingModeStep next);
public:
	void Enter();
	void Execute();
	void Exit();
	bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type);
	bool HandleMessage(LPCHARACTERSTATEMESSAGE msg);
};

//******************************************************
//	キャラクタの死亡状態のクラス
//******************************************************
class CharacterMoveStateDieMode :public CharacterMoveStateBase
{
private:
	int m_timer;
public:
	CharacterMoveStateDieMode(LPCHARACTER p);
private:
	void SetDestroyEffect(CRVector3 pos,ENTITY_ID render_entity);
	void AddScore();
public:
	void Enter();
	void Execute();
	void Exit();
	bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type);
	bool HandleMessage(LPCHARACTERSTATEMESSAGE msg);
};

//******************************************************
//	キャラクタの近接攻撃のクラス
//******************************************************
class CharacterMoveStateNearAttackMode :public CharacterMoveStateBase
{
private:
	enum ATK_STEP
	{
		ATK1,
		ATK2,
		FINISH
	};	
	ENTITY_ID			m_Target;
	int					m_timer;
	ATK_STEP			m_step;
	DamageObject		m_cola_atk;
	DamageObject		m_kick_atk;
public:
	CharacterMoveStateNearAttackMode(LPCHARACTER p);
	~CharacterMoveStateNearAttackMode();
private:
	void FollowTarget(LPCHARACTER p);
	void FrontMove(LPCHARACTER p);
	LPCharacter GetTarget();
	ATK_STEP ColaAttack();
	ATK_STEP KickAttack();
	ATK_STEP FinishMode();
public:
	void Enter();
	void Execute();
	void Exit();
	bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type);
	bool HandleMessage(LPCHARACTERSTATEMESSAGE msg);
};

//******************************************************
//	キャラクタのグレネード投擲クラス
//******************************************************
class CharacterMoveStateThrowGrenade :public CharacterMoveStateBase
{
private:
	int m_timer;
private:
	void SetGaugeMoveTarget();
	void ThrowGrenade();
public:
	CharacterMoveStateThrowGrenade(LPCHARACTER p);
	~CharacterMoveStateThrowGrenade();
public:
	void Enter();
	void Execute();
	void Exit();
	bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type);
	bool HandleMessage(LPCHARACTERSTATEMESSAGE msg);
};

//******************************************************
//	キャラクタのゴロゴロクラス
//******************************************************
class CharacterMoveStateMoveRolling :public CharacterMoveStateBase
{
private:
	enum STATE_TYPE
	{
		_PreRoll = 0,
		_Rolling = 1,
		_UsualFinish = 2,
		_ImpactFinish = 3
	};
	STATE_TYPE		m_State;
	int				m_Timer;
	Vector3			m_Pre_pos;
	Vector3			m_MoveVec;
	DamageObject	m_DamageObj;
	float			m_Speed;
	bool			m_isSuccess;
	bool			m_isTouchFloor;
	RADIAN			m_RollAngle;
private:
	void ChangeState(STATE_TYPE		NextState);
	void AngleControll(CRVector3 v);
	void SetDamageParams();
	void UpdateDamageClass();
	void StartEffect();
private:
	void StepPreRoll();
	void StepRolling();
	void StepUsualFinish();
	void StepImpactFinish();
public:
	CharacterMoveStateMoveRolling(LPCHARACTER p);
	~CharacterMoveStateMoveRolling();
public:
	void UpdateTransMatrix();
	void Enter();
	void Execute();
	void Exit();
	bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type);
	bool HandleMessage(LPCHARACTERSTATEMESSAGE msg);
};


//******************************************************
//	キャラクタのふきとび状態のステート
//******************************************************
class CharacterMoveStateVanish :public CharacterMoveStateBase
{
	typedef void(CharacterMoveStateVanish::*StepFunc)();
private:
	StepFunc	m_nowStep;
	Vector3		m_vec;
	int			m_Timer;
public:
	CharacterMoveStateVanish(LPCHARACTER p);
	~CharacterMoveStateVanish();
private:
	void Bound(CRVector3 normal);
	void StepFunc_Vanish();
	void StepFunc_VanishFinish();
public:
	void Enter();
	void Execute();
	void Exit();
	bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type);
	bool HandleMessage(LPCHARACTERSTATEMESSAGE msg);
};

//******************************************************
//	キャラクタのパラシュート降下移動クラス
//******************************************************
class CharacterMoveStateParachuteMove :public CharacterMoveStateBase
{
	/*
	命令名　send_move_vector 	仕様 v1に入っているベクトルを現在の移動量に加算する
	命令名　send_view_position 	仕様 v1に入っているベクトルを現在の視点方向にセットする
	*/
private:
	const int	m_MaxLeaveFrame;
	int			m_LeaveFrame;
	Vector3		m_ParachutePos;
	Vector3		m_ParachuteMove;
public:
	CharacterMoveStateParachuteMove(LPCHARACTER p);
private:
	void SetPlayerPos();
public:
	void Enter();
	void Execute();
	void Exit();
	void Render();
	bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type);
	bool HandleMessage(LPCHARACTERSTATEMESSAGE msg);
};


//**********************************************************************
//	キャラクタのステートをひとまとめにした物(ステートテーブル)
//**********************************************************************
class CharacterMoveStateTable
{
private:
	CHARACTER_STATE_TYPE	m_next_state_type;
	CHARACTER_STATE_TYPE	m_now_state_type;
	CharacterMoveStateBase* m_pStateTable[chr_move_state::_state_max];
public:
	CharacterMoveStateTable(LPCHARACTER pOwner);
	~CharacterMoveStateTable();
public:
	void UpdateTransMatrix();
	void Render();
	void Execute();								//現在のステートを実行する
	CHARACTER_STATE_TYPE GetState()const;		//現在のステートタイプを得る
	void SetState(CHARACTER_STATE_TYPE type);	//新しいステートを設定する
	bool HandleMessage(LPGAMEMESSAGE msg);		//この関数にexinfoとしてCharacterStateMessageを送るとステートが処理をする
};


//******************************************************
//	キャラクタのリスポーン時のクラス
//******************************************************
class CharacterMoveStateResporn :public CharacterMoveStateBase
{
	enum STATE_TYPE
	{
		_PreRoll = 0,
		_Rolling = 1,
		_UsualFinish = 2,
	};
	STATE_TYPE		m_State;
	int				m_Timer;
	Vector3			m_Pre_pos;
	Vector3			m_MoveVec;
	float			m_Speed;
	bool			m_isTouchFloor;
	RADIAN			m_RollAngle;
private:
	void ChangeState(STATE_TYPE		NextState);
	void StartEffect();
private:
	void StepPreRoll();
	void StepRolling();
	void StepUsualFinish();
public:
	CharacterMoveStateResporn(LPCHARACTER p);
	~CharacterMoveStateResporn(){}
public:
	void UpdateTransMatrix();
	void Enter();
	void Execute();
	void Exit();
	void Render();
	bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type);
	bool HandleMessage(LPCHARACTERSTATEMESSAGE msg);
};

#endif