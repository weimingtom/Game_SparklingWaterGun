#ifndef __STATE_TEMPLATE_H__
#define __STATE_TEMPLATE_H__

//********************************************************
//		�X�e�[�g�}�V�� �e���v���[�g�N���X
//********************************************************

//bool ��� int�̂ق�������?�Ȃ���
typedef int BOOL;

//********************************************************
//		�X�e�[�g�N���X
//********************************************************
/*
	Enter�֐��̓X�e�[�g�J�n��
	Execute�֐��̓X�e�[�g���s��
	Exit�֐��̓X�e�[�g�I����
	�ɌĂ΂��
*/
template <
	class _Client,
	class _ret_type=BOOL>
class State
{
public:
	typedef _Client*	_client_type_ptr;
	typedef _ret_type _Return_value_type;
public:
	virtual _Return_value_type Enter(_client_type_ptr)
	{
		return 0;
	}
	virtual _Return_value_type Execute(_client_type_ptr)
	{
		return 0;
	}
	virtual _Return_value_type Exit(_client_type_ptr)
	{
		return 0;
	}
};
//********************************************************
//		�X�e�[�g�N���X���ꉻver(�߂�l��void�̏ꍇ)
//********************************************************
template <class _Client>
class State
<_Client,void>
{
public:
	typedef _Client*_client_type_ptr;
public:
	virtual ~State(){}
	virtual void Enter(_client_type_ptr){}
	virtual void Execute(_client_type_ptr){}
	virtual void Exit(_client_type_ptr){}
};
//********************************************************
//		�X�e�[�g�}�V���N���X
//********************************************************
/*
	set_state�֐��Ō��݂̃X�e�[�g��Exit�֐����Ăяo���A
	�V���ȃX�e�[�g���Z�b�g��Enter�֐����Ăяo���B

	state_execute�֐��Ō��݂̃X�e�[�g��Execute�֐����Ăяo���B
	�X�e�[�g���ݒ肳��Ă��Ȃ��ꍇ�͂O��Ԃ�
*/
template <
	class _Client,
	class _ret_value=BOOL>
class StateMachine
{
public:
	typedef _Client*_client_type_ptr;
	typedef _ret_value _Return_value_type;
	typedef State<_Client,_ret_value>*_MyState_ptr;
private:
	_client_type_ptr		m_pClient;
	_MyState_ptr			m_pState;
	_MyState_ptr			m_pNextState;
public:
	StateMachine(_client_type_ptr Client) :
		m_pState(0),
		m_pNextState(0),
		m_pClient(Client)
	{
	}
	virtual~StateMachine()
	{
		if(m_pState)delete m_pState;
		if(m_pNextState)delete m_pNextState;
	}
public:
	void set_state(_MyState_ptr const Newstate)
	{
		if(m_pNextState)
			delete m_pNextState;
		m_pNextState=Newstate;
	}
	_Return_value_type state_execute()
	{
		if(m_pNextState){
			if(m_pState){
				m_pState->Exit(m_pClient);
				delete m_pState;
			}
			m_pNextState->Enter(m_pClient);
			m_pState=m_pNextState;
			m_pNextState=0;
		}
		if(m_pState)
			return m_pState->Execute(m_pClient);
		return 0;
	}
};
//********************************************************
//		�X�e�[�g�N���X���ꉻver(�߂�l��void�̏ꍇ)
//********************************************************
template <class _Client>
class StateMachine
	<_Client,void>
{
public:
	typedef _Client*_client_type_ptr;
	typedef void _Return_value_type;
	typedef State<_Client,_Return_value_type>*_MyState_ptr;
private:
	_client_type_ptr		m_pClient;
	_MyState_ptr			m_pState;
	_MyState_ptr			m_pNextState;
public:
	StateMachine(_client_type_ptr Client) :
		m_pState(0),
		m_pNextState(0),
		m_pClient(Client)
	{
	}
	virtual~StateMachine()
	{
		if(m_pState)delete m_pState;
		if(m_pNextState)delete m_pNextState;
	}
public:
	_MyState_ptr GetState()
	{
		return m_pState;
	}
	void set_state(_MyState_ptr const Newstate)
	{
		if(m_pNextState)
			delete m_pNextState;
		m_pNextState=Newstate;
	}
	_Return_value_type state_execute()
	{
		if(m_pNextState){
			if(m_pState){
				m_pState->Exit(m_pClient);
				delete m_pState;
			}
			m_pNextState->Enter(m_pClient);
			m_pState=m_pNextState;
			m_pNextState=0;
		}
		if(m_pState)
			return m_pState->Execute(m_pClient);
	}
};
#endif