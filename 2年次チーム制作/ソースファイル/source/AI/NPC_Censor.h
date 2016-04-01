#ifndef __NPC_CENSOR_H__
#define __NPC_CENSOR_H__

class NPC_Brain;
class Character;
class WorkingMemory;
class ControllObject;
struct DamageParam;
struct ExpectationAct;
class Env_Sound;
class NPCParam;

#include "iextreme.h"
#include "../GameSystem/GameBase.h"

class NPC_Censor
{
protected :
	const int		m_update_Interval;		//�X�V�Ԋu
	const int		m_reaction_Speed;		//�������x

public :
	NPC_Censor(const int update_Interval, const int reaction_Speed) :
		m_update_Interval(update_Interval), m_reaction_Speed(reaction_Speed)
	{	}
	virtual ~NPC_Censor(){}
	virtual void	Update(NPC_Brain* brain, Character* character) = 0;
};


struct Msg_AttackMe
{
	DamageParam damage_Param;
	int				m_current_Frame;
	Msg_AttackMe(const DamageParam& param) : damage_Param(param), m_current_Frame(0)
	{	}
	void	Update(){ m_current_Frame++; }
	
};

class Censor_AttackMe : public NPC_Censor
{
	friend class NPC_Brain;
private :
	std::list<Msg_AttackMe*>		m_reaction_TargetList;
private :
	/**
	*@brief NPC_Brain�Ƀv�����j���O�v����Message�𑗐M���邩�ǂ����𔻒肷��
	*/
	bool	Judge_Dispatch_PlanningMsg(NPC_Brain* brain, Character* character,Msg_AttackMe* judge_Msg);
	/**
	*@brief ID�̏d�����`�F�b�N����
	*/
	bool	Check(const DamageParam& param);
	/**
	*@brief �������_���[�W���󂯂��G��o�^����
	*@param �U�����Ă����G��ID
	*/
	void	Register(NPC_Brain* brain, const DamageParam* target_ID);
	/**
	*@�L�����(action)������������
	*/
	void	Rewrite_Memory(NPC_Brain* brain,const Msg_AttackMe& msg);
	void	Update_Counter(NPC_Brain* brain, Character* character);
public :
	Censor_AttackMe(const int reaction_Speed);
	~Censor_AttackMe();
	void			Release();
public :
	void		Update(NPC_Brain* brain, Character* character);
};

class Censor_Sight : public NPC_Censor
{
	friend class NPC_Brain;
	/**
	*@brief �����ł��邩�ǂ������肷��֐��i���Ԃ񂻂̂����ύX����j
	*/
	bool	Is_See_Through(const MAP_OBJ_TYPE& type){ return true; }
private :
	const float		m_fov;					//����p
	const float		m_sight_Range;	//���F����
	bool					look;

private :
	/**
	*@brief �t�B�[���h�ɑ��݂��Ȃ��Ȃ����L�����N�^�[�ɑ΂��ċL���̍폜Flg���N������
	*/
	void		Delete_NotField_CharacterRecord(NPC_Brain* brain);
	bool		Look_Character(NPC_Brain* brain, Character* chacracter);
	bool		Look_MapObject(NPC_Brain* brain, Character* character);
	void		Update_TemporaryMemory_Character(NPC_Brain* brain, Character* character);
	bool		Judge_Attack(NPC_Brain* brain, Character* character, Character_Record* record);
	bool		Judge_Medical(NPC_Brain* brain, Character* character, Character_Record* record);
	bool		Judge_Dispatch_PlanningMsg(NPC_Brain* brain, Character* character,Character_Record* record);
	bool		Judge_Dispatch_PlanningMsg(NPC_Brain* brain, Character* character, MapObject_Record* record);

	arive_status Cul_Status(Character* target);
	void	Update_Sight(NPC_Brain* brain, Character* character);

public :
	Censor_Sight(const int update_Interval, const float fov,const float sight_Range,const int reaction_Speed);
	~Censor_Sight();
	inline	void	Look(){ look = true; }
public :
	void		Update(NPC_Brain* brain, Character* chacracter);
	bool		SightCheck_and_Line_of_Fire(Character* character, const Vector3& target, bool* visibility, 
																bool* line_of_Fire,bool* in_Fov, int team_type);
};



class Censor_FriendMessage : public NPC_Censor
{
private:
	bool											m_isActive;							//�󂯎�������b�Z�[�W�����݂̃S�[���ɓK������Ă���ꍇtrue�ƂȂ�
	ENTITY_ID								m_send_Entity;					//���b�Z�[�W�𑗐M���Ă���Entity
	FriendMessageIcon::MsgType	m_process_MessageType;	//	�������Ă��郁�b�Z�[�W�^�C�v

public:
	Censor_FriendMessage(const int reaction_Speed);
	~Censor_FriendMessage();

public :
	void	Update(NPC_Brain* brain, Character* character);
	bool	Activate();
	bool	ProcessMessage(NPC_Brain* brain, GameMessage* msg);
	/**
	*@brief ���b�Z�[�W�ɓK�����ꂽ�S�[�����ݒ肳�ꂽ���ǂ����𒲂ׂ�
	*@param ���₳�ꂽ�ŏI�S�[��
	*/
	bool	Check_ThinkingResult(NPC_Brain* brain,FinalGoal_Evalutor* final_goal);
	__forceinline ENTITY_ID	Get_SenderID(){ return m_send_Entity; }
	__forceinline const FriendMessageIcon::MsgType	GetFriendMessageType(){ return m_process_MessageType; }
	__forceinline void				Clear(){ m_isActive = false; m_send_Entity = game_id::id_error; m_process_MessageType = FriendMessageIcon::MsgType::_ERROR; }
};


#pragma region NotUse

//struct Important_Sound
//{
//	SOUND_TYPE					m_type;					//�^�C�v
//	Vector3							m_source_Pos;		//������
//	float									m_power;				//���̑傫��
//	Important_Sound(const SOUND_TYPE& type, const Vector3& pos, const float& power) :
//		m_type(type), m_source_Pos(pos), m_power(power)
//	{	}
//	inline Important_Sound() : m_type(SOUND_TYPE::error), m_source_Pos(Vector3(.0f, .0f, .0f)),
//		m_power(-1.0f)
//	{	}
//	inline void	Set(const SOUND_TYPE& type, const Vector3& pos, const float& power)
//	{
//		m_power = power; m_source_Pos = pos ; m_type = type;
//	}
//
//	inline void	Clear()
//	{
//		m_source_Pos = Vector3(.0f, .0f, .0f); m_power = -1.0f; m_type = SOUND_TYPE::error;
//	}
//};

//class Censor_Hearing : public NPC_Censor
//{
//private :
//	std::vector<Env_Sound*>	m_transact_vector;	//�V���������������������邽�߂̃R���e�i
//	Important_Sound					m_most_ImportantSound;
//	bool										m_activate;				//�v�����j���O�v��msg
//	int										m_dispatch_Timer;
//private :
//	void	Hearing(NPC_Brain* brain, Character* chacracter);
//	bool	Transact(NPC_Brain* brain, Character* chacracter, const Vector3& my_Pos);
//	float	Get_Range(const SOUND_TYPE& type, const NPCParam& param);
//	float Get_Rate(const SOUND_TYPE& type,NPC_Brain* brain);
//	char* Convert_SoundType(const SOUND_TYPE& type);
//public :
//	Censor_Hearing(const int update_Interval, const int reaction_Speed);
//	~Censor_Hearing();
//
//	void Update(NPC_Brain* brain, Character* chacracter);
//	inline  Important_Sound& Get_MostImportant()  { return m_most_ImportantSound; }
//};


#pragma endregion

#endif
