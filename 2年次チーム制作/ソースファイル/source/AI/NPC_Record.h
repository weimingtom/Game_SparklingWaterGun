#ifndef __RECORD_H__
#define __RECORD_H__
#include	<map>
#include	<list>
#include	"SoundMessage.h"
#include	<array>
struct ExpectationAct
{
	expectation_act		m_tag;		//�s���̃^�O
	/**
	*@note				�s���ɑΉ�����x�N�g�����
	*@attack_me		�U���̕���
	*@move_field		�ړ�����
	*/
	Vector3					m_vec;	
	inline ExpectationAct() : m_tag(expectation_act::none), m_vec(Vector3(FLT_MAX, FLT_MAX, FLT_MAX))
	{	}

};

_ENUM_BEGIN(arive_status,ARIVE_STATUS)
arive,
dead,
dying,
i_dont_now
_ENUM_END(arive_status, ARIVE_STATUS)

class Character_Record
{
private :
	const	int					m_delete_Time = 300;	//�Ƃ肠����100�t���[���̗P�\�������č폜����
	bool								m_delete_Flg;
	int								m_timer;

private :
	float								Calculate_Confidence(const int& current_Frame, const float&ReliabilityTime);
public:
	bool								m_update;					//SencorSight�ɂ���čX�V���ꂽ���ǂ���
	int								m_last_SensedTime;	//�Ō�Ɏ��F�����t���[��(�ő��Sight_Interval���̌덷������
	int								m_became_Visible;	//���̃I�u�W�F�N�g�������n�߂�����
	int								m_team_Type;
	Vector3						m_became_Pos;			//�����n�߂����̍��W
	Vector3						m_sensed_Pos;			//�Ō�Ɏ��F�������W
	Vector3						m_sensed_Velocity;		//�ړ�����
	Vector3						m_sensed_Angle;		//�����Ă�������
	bool								m_visibilily;					//���F���Ă��邩�ǂ���
	bool								m_line_of_Fire;			//�ː����ʂ邩�ǂ���
	bool								m_in_Fov;					//���E���ɂ͂����Ă��邩�ǂ���
	ENTITY_ID					m_id;							//ID
	ExpectationAct				m_action;						//�������Ă������̍s���\���i�Ō�Ɋm�F�����s���j
	int								m_expectation_Time;	//�s���\���������Ō�̎���
	float								m_hp;							//HP
	arive_status					m_status;						//�������
public:
	inline Character_Record() : m_last_SensedTime(-1), m_became_Visible(-1), m_team_Type(-1), m_became_Pos(Vector3(.0f, .0f, .0f)),
		m_sensed_Angle(Vector3(.0f, .0f, .0f)), m_sensed_Pos(Vector3(.0f, .0f, .0f)), m_sensed_Velocity(Vector3(.0f, .0f, .0f)), m_visibilily(false),
		m_line_of_Fire(false),m_id(game_id::id_error), m_action(), m_expectation_Time(-1), m_status(arive_status::i_dont_now)
		, m_delete_Flg(false), m_timer(-1), m_hp(-1.0f), m_in_Fov(false)
	{	}

	bool	Can_Use(const int& current_Frame, const int& ReliabilityTime, float* Reliability);
	bool	Update()
	{
		if (!m_delete_Flg)
			return false;
		m_timer--;
		if (m_timer <= 0)
			return true;
		
		return false;
	}
	void	Delete()
	{
		m_delete_Flg = true;
		m_timer = m_delete_Time;
	}
	/**
	*@brief �폜�t���O�������Ă��邩�ǂ���
	*/
	inline bool	Is_Delete(){ return m_delete_Flg; }
};



class MapObject_Record
{
public:
	int						m_last_SensedTime;	//�Ō�Ɏ��F�����t���[��(�ő��Sight_Interval���̌덷������
	int						m_became_Visible;		//���̃I�u�W�F�N�g�������n�߂�����
	Vector3				m_became_Pos;			//�����n�߂����̍��W
	Vector3				m_sensed_Pos;			//�Ō�Ɏ��F�������W
	Vector3				m_sensed_Velocity;		//�ړ�����
	Vector3				m_sensed_Angle;		//�����Ă�������
	bool						m_visibilily;					//���F���Ă��邩�ǂ���
	bool						m_line_of_Fire;			//�ː����ʂ邩�ǂ���
	bool						m_in_Fov;					//���E���ɂ͂����Ă��邩�ǂ���
	ENTITY_ID			m_id;							//ID
	mapobjcet_type	m_object_type;			//type
private :
	float	Calculate_Confidence(const int& current_Frame, const float&ReliabilityTime);
public:
	bool	Can_Use(const int& current_Frame, const float& ReliabilityTime, float* Reliability);
	MapObject_Record() : m_last_SensedTime(-1), m_became_Visible(-1), m_sensed_Pos(Vector3(0, 0, 0)),
		m_became_Pos(Vector3(0, 0, 0)), m_sensed_Angle(Vector3(0, 0, 0)), m_sensed_Velocity(Vector3(0, 0, 0)),
		m_visibilily(false), m_id(game_id::id_error), m_object_type(mapobjcet_type::error)
	{	}
	bool	Can_Use(const int& current_Frame, const int& ReliabilityTime, float* Reliability);
};

struct Threat_OutSight
{
	ENTITY_ID m_id;
	Vector3	m_vec;
	Threat_OutSight(const ENTITY_ID& id, const  Vector3& vec) : m_id(id), m_vec(vec){}
};
enum class censor_type :int
{
	none = -1,
	sight,
	hearing,
	attack_me,
	num_censor,
};
/**
*@brief �v�����j���O���s����censorObject�ƑΏ�EntityID���ꎞ�I�ɕۑ����Ă����N���X
*@note �Z�����Ԃ�Planning�֐����Ă΂�߂��Ȃ��悤�ɂ��邽�߂̈ꎞ�L��
*/
class Temporary_PlanningMemory
{
	static  const int max_memory = 30;
	struct MemoryObject
	{
		bool				m_active;
		censor_type m_sender_type;		//�v�����j���O��v�����������
		ENTITY_ID	m_rock_ID;
		int				m_registar_Frame;	//�o�^���ꂽ�t���[��
		MemoryObject() : m_sender_type(censor_type::none), m_rock_ID(game_id::id_error),
			m_registar_Frame(-1), m_active(false)
		{}
	};
private:
	std::array<MemoryObject, max_memory> m_memory_Array;
	int																m_delete_Frame[(int)censor_type::num_censor];	//
	int																m_current_Regist;
	const NPC_Brain*										m_holder;

public:
	Temporary_PlanningMemory(const NPC_Brain*);
	bool	Registar(const censor_type& type, const ENTITY_ID& register_ID);
	void	Update(const int& current_Frame);
	/**
	*@brief �d���`�F�b�N
	*@return �d�����Ă����-1 �o�^�ł���ꍇ�͈�ԋ߂��󂫂�Index
	*/
	int	Is_Register(const censor_type& type, const ENTITY_ID& register_ID);

	bool	Remove_Array(const ENTITY_ID& remove_ID);
};

/**
*@brief ��A�̍s���̐����A���s�ɂ��񂷂�L��
*/
class Goal_Result
{
	enum p_result{NONE =-1,COMPLETED,FAILED};
	static const int max_memory = 30;
	struct MemroyObject
	{
		bool				m_active;
		ENTITY_ID	m_id;
		p_result		m_result;
		int				m_registar_Frame;
		MemroyObject() :m_active(false),m_id(ENTITY_ID::id_error),
								m_result(p_result::NONE), m_registar_Frame(-1)
		{}
	};

private :
	std::array<MemroyObject, max_memory> m_memory_Array;
	int																m_active_Frame;
	int																m_current_Regist;
	const NPC_Brain*										m_holder;
public :
	Goal_Result(NPC_Brain* holder, const int& active_frame);
	/**
	*@param1 nullptr�ł͂Ȃ��ꍇ�����󂫂�����ꍇ�ɗv�f�ԍ���Ԃ�
	*@return �o�^����Ă�����true
	*/
	bool	Is_Register(const ENTITY_ID& check_id, int* iterator = nullptr);
	/**
	*@brief �o�^����
	*@param[bool] ������true�@���s��false
	*/
	bool Register(const ENTITY_ID& register_id, const bool&	result);
	void	Update(const int& current_Frame);
	/**
	*@brief �o�^����
	*/
	bool	Remove_Array(const ENTITY_ID& remove_id);
};

class MEMORY
{
private :
	
public:
	std::map<ENTITY_ID, Character_Record*>					m_character_Record;		//�t�B�[���h�̃I�u�W�F�N�g�ɑ΂�����
	std::map<ENTITY_ID, MapObject_Record*>					m_mapobject_Record;		//�}�b�v�̃I�u�W�F�N�g�ɑ΂���L��
	std::vector<Threat_OutSight>										m_threat_Outsight;			//���E�O�̋��Ђɑ΂���L��container
	Temporary_PlanningMemory*											m_planning_Memory;		//�v�����j���O�������ʃ^�[�Q�b�g�ƂȂ���ENTITY���ꎞ�I�Ɋi�[���Ă����L��(Censor_Sight�ȂǂŎg�p)
	Goal_Result*																	m_goal_Result;				//�ŏI�S�[�������s�A���������Ƃ��̏����ꎞ�I�Ɋi�[���Ă����L���̈�i�o�H�T�������s�����Ƃ��Ȃǂɂ���ENTITY_ID���i�[���Ă����Ȃ� :GoalEvalutor�ȂǂŎg�p�j
public : 
	//std::list<Attribute<goal_Tag>>		past_Goal;				//�ȑO�̃S�[���i�ŏI������L�^���邩�͖���j
	void	All_Clear();
	void	All_Release();
public:
	/**
	*@brief		�������̃L�����N�^�[���L�����Ă��邩�ǂ����m�F�A
	�L�����Ă��Ȃ��ꍇ�͗̈���m�ۂ���
	*@param1	�m�F����L�����N�^�[���
	*/
	void	Is_Record_Character(const ENTITY_ID& check_object);
	void	Is_Record_MapObject(const ENTITY_ID& check_object);
	bool Is_Record_Threat_OutSight(const ENTITY_ID& check_Object);

	MapObject_Record* Get_MapObjectMemory(const ENTITY_ID& id, const int& reliability_Time, float*reliability);
	Character_Record* Get_CharacterMemory(const ENTITY_ID& id, const int& reliability_Time, float*reliability);
	bool	Registar_Threat_OutSight(const ENTITY_ID& id, const Vector3& threar_Vec);
	bool	Erase_Threar_OutSight(const ENTITY_ID& erase_Id);
	std::vector<Threat_OutSight>*const  Get_Threat_OutSightContainer(){ return &m_threat_Outsight; }
	Threat_OutSight*const  Get_Threat_OutSight (const ENTITY_ID& id);
	/**
	*@brief �L�����N�^�[�̍X�V�t���O��false��
	*/
	void	Reset_UpdateFlg_CharacterRecord();

};


#endif
