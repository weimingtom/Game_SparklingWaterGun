#ifndef __CHARACTER_MOVE_STATE_H__
#define __CHARACTER_MOVE_STATE_H__

#include "Character.h"
#include "CameraMove.h"
#include "AimPoint.h"
#include "DamageHit.h"

class CharacterMoveStateTable;
typedef CharacterMoveStateTable* LPCHARACTERMOVESTATETABLE;

//********************************************************
//	�X�e�[�g�ɑ��郁�b�Z�[�W���
//********************************************************

namespace chr_state_msg_type
{
	enum _ENUM_CHARACTER_STATE_MESSAGE_TYPE
	{
		_error_type = -1,

		send_move_vector,	//v1�Ɉړ��ʃx�N�g�����i�[���đ���
		send_view_position,	//v1�Ɏ����x�N�g�����i�[���đ���
		send_jump_state,	//v1.x�ɃX�e�[�g�ԍ������đ���
		send_lock_on_flg,	//v1.x�Ƀt���O 0 or !=0	�����đ���
		send_lock_on_change_right_left,	//�Ȃɂ�����Ȃ��Ă悵
		send_wepon_fire_flg,//�Ȃɂ�����Ȃ��Ă悵
		send_shuffle_flg,	//v1.x�Ƀt���O 0 or !=0	�����đ���
		send_vanish_vec,	//v1�Ɉړ��ʃx�N�g�����i�[���đ���
		send_hover,			//�Ȃɂ�����Ȃ��Ă悵
	};
}
typedef chr_state_msg_type::_ENUM_CHARACTER_STATE_MESSAGE_TYPE CHRSTATE_MSGTYPE;

//	�X�e�[�g��exinfo�Ƃ��đ���\����
class CharacterStateMessage
{
public:
	CharacterStateMessage() :type(chr_state_msg_type::_error_type), v1(0,0,0), v2(0,0,0), v3(0,0,0){}
public:
	//���ߖ�
	CHRSTATE_MSGTYPE type;
	//�x�N�g�����
	Vector3	v1, v2, v3;
};

typedef CharacterStateMessage	*LPCHARACTERSTATEMESSAGE;

//**********************************************************
//	���b�N�I���N���X
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
//	shuffle�N���X
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
//	�L�����N�^�̍s���X�e�[�g�̊��N���X
//******************************************************
class CharacterMoveStateBase
{
protected:
	LPCHARACTER m_pOwner;
public:
	CharacterMoveStateBase(LPCHARACTER pOwner);
	virtual ~CharacterMoveStateBase(){}
public:
	virtual void Enter() = 0;	//�X�e�[�g���s�J�n���ɌĂ΂��
	virtual void Execute() = 0;	//�X�e�[�g���s���ɌĂ΂��
	virtual void Exit() = 0;	//�X�e�[�g���s�I�����ɌĂ΂��
	virtual void Render(){};
	virtual void UpdateTransMatrix();
	virtual bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type) = 0;	//�����̃X�e�[�g�Ɍ��ݕύX�ł��邩�ǂ���
	virtual bool HandleMessage(LPCHARACTERSTATEMESSAGE msg) = 0;	//���b�Z�[�W��M�֐�
};

//******************************************************
//	�L�����N�^�̎��R�ړ��N���X
//******************************************************
class CharacterMoveStateUsualMode :public CharacterMoveStateBase
{
	/*
		���ߖ��@send_move_vector 	�d�l v1�ɓ����Ă���x�N�g�������݂̈ړ��ʂɉ��Z����
		���ߖ��@send_view_position 	�d�l v1�ɓ����Ă���x�N�g�������݂̎��_�����ɃZ�b�g����
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
//	�L�����N�^�̃W�����v���[�h�N���X
//******************************************************
class CharacterMoveStateJumpMode :public CharacterMoveStateBase
{
	/*
	���ߖ��@send_move_vector 	�d�l v1�ɓ����Ă���x�N�g�������݂̈ړ��ʂɉ��Z����
	���ߖ��@send_view_position 	�d�l v1�ɓ����Ă���x�N�g�������݂̎��_�����ɃZ�b�g����
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
	//�W�����v�̏����킯�֐�
	void JumpStartMode();
	void JumpMoveUpMode();
	void JumpMoveDownMode();
	void JumpFinishMode();
private:
	void ChangeStep(JUMPSTEP step);	//�X�e�b�v�ύX�֐�
public:
	void Enter();
	void Execute();
	void Exit();
	bool isCanChangeState(CHARACTER_STATE_TYPE change_state_type);
	bool HandleMessage(LPCHARACTERSTATEMESSAGE msg);
};

//******************************************************
//	�L�����N�^�̕m����Ԃ̈ړ��N���X
//******************************************************
class CharacterMoveStateDyingMode :public CharacterMoveStateBase
{
	/*
	���ߖ��@send_move_vector 	�d�l v1�ɓ����Ă���x�N�g�������݂̈ړ��ʂɉ��Z����
	���ߖ��@send_view_position 	�d�l v1�ɓ����Ă���x�N�g�������݂̎��_�����ɃZ�b�g����
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
//	�L�����N�^�̎��S��Ԃ̃N���X
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
//	�L�����N�^�̋ߐڍU���̃N���X
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
//	�L�����N�^�̃O���l�[�h�����N���X
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
//	�L�����N�^�̃S���S���N���X
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
//	�L�����N�^�̂ӂ��Ƃя�Ԃ̃X�e�[�g
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
//	�L�����N�^�̃p���V���[�g�~���ړ��N���X
//******************************************************
class CharacterMoveStateParachuteMove :public CharacterMoveStateBase
{
	/*
	���ߖ��@send_move_vector 	�d�l v1�ɓ����Ă���x�N�g�������݂̈ړ��ʂɉ��Z����
	���ߖ��@send_view_position 	�d�l v1�ɓ����Ă���x�N�g�������݂̎��_�����ɃZ�b�g����
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
//	�L�����N�^�̃X�e�[�g���ЂƂ܂Ƃ߂ɂ�����(�X�e�[�g�e�[�u��)
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
	void Execute();								//���݂̃X�e�[�g�����s����
	CHARACTER_STATE_TYPE GetState()const;		//���݂̃X�e�[�g�^�C�v�𓾂�
	void SetState(CHARACTER_STATE_TYPE type);	//�V�����X�e�[�g��ݒ肷��
	bool HandleMessage(LPGAMEMESSAGE msg);		//���̊֐���exinfo�Ƃ���CharacterStateMessage�𑗂�ƃX�e�[�g������������
};


//******************************************************
//	�L�����N�^�̃��X�|�[�����̃N���X
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