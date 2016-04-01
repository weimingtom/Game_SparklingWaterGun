#ifndef __BRAIN_H__
#define __BRAIN_H__

#include <deque>
#include	<map>
#include	<string>
#include	"Template_Graph.h"
#include	"../EnumCP11.h"
#include	<array>
//console�o���}�N��
#define CONSOLE 0

//AI�L�����N�^�[�f�o�b�O�}�N��
#define NPC_DEBUG 0

class Goal;
class NPC_Brain;
class CharacterBrainBase;
class Symbol;
class Working_Memory;
class Character_Record;
class MapObject_Record;
class Sound_Record;
class NPC_Censor;
class Censor_AttackMe;
class Censor_Sight;
class Censor_FriendMessage;
struct DamageParam;
struct ExpectationAct;
class DebugLogOutput;
class Behavior_Manager;
class NPCFriendMessage;

#pragma region ActionPlanner

class Action_Node;
class Action_Edge : public Edge_Base
{
public:
	Action_Edge() :Edge_Base(){}
};

class Action_Node : public Node_Base
{
public:
	Goal* goal;
	Action_Node(Goal* goal) : Node_Base(), goal(goal){}
	Action_Node() : Node_Base(){}
};

class ActionSearch;
class Action_Planner : public Graph<Action_Node,Action_Edge>
{
	friend class NPC_Factory;
private :
	std::vector<Goal*>	m_midlle_Goallist;
	Action_Node*			dummy_Node;				//�v�����T�����ɃX�^�[�g�ʒu�ɂȂ�_�~�[�m�[�h	
	NPC_Brain*				m_holder;							//���̃N���X���������Ă���NPC_Brain
	ActionSearch			m_search_Alg;				//�T���A���S���Y��
	float							m_default_CostList[static_cast<int>(goal_tag::NumGoal)][static_cast<int>(goal_tag::NumGoal)];	//�S�[���Ԃ�defaultcost���X�g�iedge�ɑ}���j	

private :
	void							Setting_MidlleGoalList();
	void							Load_GoalCost(char* param_Filename);
	bool							Settting_Goal(std::deque<Goal*>* goal_Stack);
	bool							Chain_Goal(const Symbol& symbol, Character* character);
public :
	Action_Planner(NPC_Brain* b, char* cost_Filename);
	~Action_Planner();

	/**
	*@brief ���݂̃O���t�����A�̍s�����v�����j���O
	*/
	bool	Plannning(const Symbol& symbol, Character* character,std::deque<Goal*>* stack, Action_Node* final_Goal);
	/**
	*@brief�S�S�[���̃X�e�[�^�X������
	*/
	void	Clear_GoalStatus();
};
#pragma endregion

//****************************************************//
//Fuzzy�֘A
//****************************************************//
class NPC_FuzzyTool
{
	friend class NPC_Brain;
private :
	std::map<std::string, FuzzyModule*>	module_Map;
	NPC_FuzzyTool(){}

public :
	~NPC_FuzzyTool();
	void	Initialize();
	
	float	Calculate_Sight_Target_Character(const float& dist, const float& ammo);
};

struct DeFuzzify_Param
{
	float m_sight_target;

	void	Load(char* filename);
};

class Target
{
public :
	ENTITY_ID current;
	Target() : current(game_id::id_error)
	{	}
};

struct StatusParam
{
	bool	m_is_Arive;					//�����t���O
	bool	m_is_Active;				//�s������
	int	m_active_Timer;
	StatusParam() : m_is_Active(true), m_is_Arive(true), m_active_Timer(0)
	{	}
};

enum test;

class Goal_Think
{
	friend class NPC_Factory;
private :
	std::vector<FinalGoal_Evalutor*>	m_goal_Evalutor;
	FinalGoal_Evalutor*	m_current_FinalGoal;	//���݂̍ŏI�S�[��
	NPC_Brain * m_holder;

private: 
	bool	Set_FriendMessageBias(const int messagetype);
	
public :
	Goal_Think(NPC_Brain* holder);
	~Goal_Think();
	void	Initialize();

public :
	Action_Node* Thinking(Character* character);
	bool	HandleMessage(AI_MESSAGE* msg);
	inline void	Reset_CurrentTarget_FinalGoal(){ m_current_FinalGoal->Reset_Target(); }
	FinalGoal_Evalutor* Get_FinalGoal(){ return m_current_FinalGoal; }
	__forceinline void	 Reset_MessageBias()
	{
		for (auto it : m_goal_Evalutor)
			it->Clear_MessageBias();
	}


};

#pragma region Brain

class NPCParam
{
public:
	float	fov;			//����p
	float	sight;		//����(sq)
	int		m_chara_SightInterval;			//�L�����N�^�[�ɑ΂���X�V�Ԋu
	int		m_map_SightInterval;				//�}�b�v�I�u�W�F�N�g�ɑ΂���X�V�Ԋu
	int		ammo_Interval;
	int		sound_Interval;
	float		sight_Guarantee;						//�����݃I�u�W�F�N�g�������Ă��邱�Ƃ�ۏ؂���M���l()�i0~1.0�j
	float		max_HearingRange;				//�Q�[������ԑ傫��������������ő勗���iEnvManager���特������Ă���Ƃ��ɕK�v�j
	float		reaction_FootSound;				//�����ɔ������鋗��
	float		reaction_GunSound;				//�e���ɔ������鋗��
	int		record_character_ReliabilityTime;	//CharacterRecord�̐M���l���O�ɂȂ鎞��	
	int		record_mapobject_ReliabilityTime;	//MapObjectRecord�̐M���l��0�ɂȂ鎞��
	int		record_ExtAct_ReliabilityTime;		//�s���\�����L�^���Ă����t���[����
	int		feel_few_WeaponCharge;				//charge�����Ⴂ�Ɗ�����l�i���Symbol::WeaponCharge�Ŏg�p�j
	float		m_feelAttack;								//�������U�����Ă���Ɗ����鋗��
	float		m_feel_high_SharePoint;				//�L���S�[���ɂ�����share���������Ɗ�����l�i0~1�j
	float		m_max_rotateAngle_Speed;
	float		m_enemy_AttackMe_Fov;			//�G���������U�����悤�Ƃ��Ă���Ɣ��f���鎋��p
	float		m_feel_NearRangeDist;				//�ߐڍU��������Ɗ����鋗��(sq)
	int		m_interval_characterTarget_SearchPath;
	int		m_interval_advertisementpoint_SearchPath;
	float		m_far_of_Vessel;							//Goal_AvoidanceVessel�𔻒f����Ƃ��ɏ�蕨�������Ɗ����鋗��
	int		m_probability_grenede_attack;	//�O���l�[�h�U�����s���m��
	int		m_response_Sight;						//censor_sight��Planning�������x
	int		m_responce_AttackMe;				//censor_attackme��Planning�������x
	int		m_responce_FriendMessage;		//censor_FriendMessage��Planning�������x
	float		m_knockdownEnemy_distBias_Begin;	//KnockDownEnemy�]���֐��ɂ����ĕ]���l�����������͂��߂�����
	float		m_knockdownEnemy_distBias_End;	//KnockDownEnemy�]���֐��ɂ����ĕ]���l��0�ɂ��鋗��
	float		m_friendMessageBias_MedicalFriend;	//MedicalFriend�ɂ�����]���֐���MessageBias
	NPCParam();

	bool	Load(char* filename);

};

/**
*@brief ����S�[���Ǘ��N���X
*/
class ParticularGoal_Manager
{
private :
	std::vector<Goal*> m_particularGoal_vector;
	ENTITY_ID				m_target_ID;		//����S�[����p�̃^�[�Q�b�gID
	NPC_Brain*				m_holder;
	bool							m_active;
private :
	void	Initialize_Goal();
public :
	ParticularGoal_Manager( NPC_Brain* holder);
	~ParticularGoal_Manager();

public :
	inline void Active(){ m_active = true;}
	inline void	 Exit(){ m_active = false; }
	inline bool Is_Active(){ return m_active; }
	bool	Insert_Goal(Character* character, const partculargoal_type& insert_goal);
	inline const ENTITY_ID& Get_TargetID(){ return m_target_ID; }
	inline void	Set_TargetID(const ENTITY_ID& id){ m_target_ID = id; }

};



struct RAP_CENSROR
{
	Censor_Sight*					m_sight;
	Censor_AttackMe*			m_attack_Me;
	Censor_FriendMessage*	m_friend_Message;
	RAP_CENSROR() : m_sight(nullptr), m_attack_Me(nullptr), m_friend_Message(nullptr)
	{	}
	void	Release()
	{
		SAFE_DELETE(m_sight);
		SAFE_DELETE(m_attack_Me);
	}
};

class NPC_Evalutor;
class PathPlanner;

class NPC_Brain :public CharacterBrainBase
{
private :
	std::vector<Goal*>			m_final_Goallist;
	std::deque<Goal*>			m_goal_Deque;	//�S�[����
	RAP_CENSROR					m_censor;
	NPCParam						m_chara_Param;
	StatusParam						m_status_Param;
	WorkingMemory				m_memory;
	Symbol								m_symbol;
	NPC_FuzzyTool					m_fuzzy;
	Goal_Think*						m_think;
	Action_Planner*				m_planner;		//�O���t����ōs�������肷��
	Target								m_target;
	DeFuzzify_Param				m_defuzzify_param;
	DebugLogOutput*			m_console;
	std::string						m_logfilename;
	Behavior_Manager*			m_behavior_Manager;
	NPC_Evalutor*					m_evalutor;
	PathPlanner*					m_path_Planner;
	ParticularGoal_Manager*	m_particularGoal_Manager;
	NPCFriendMessage*			m_message_Icon;

private :
	/**
	*@note �֐��|�C���^�̑���������ɍs��
	*/
	void	Intialize_Censor();
	void	Set_FinalGoal();
	void	Goal_Completed(Character* p);
	void	Goal_Process(Character* p);
	void	Goal_Failed(Character* p);
	void	All_Reset(Character* character);
	bool	Planning(Character* charcter);
	bool	Is_Arive(Character* character);
	void	Clear_FinalGoalStatus();
	/**
	*@brief �L���X�V���̏���
	*/
	bool	Management_Message_Memory(GameMessage* msg);
	void	Update_Censor(Character* character);

public	:
	 NPC_Brain(const char* name,const int Controllder_Num,const int team_type);
	 ~NPC_Brain();
	 void Update(Character* p);
	bool HandleMessage(LPGAMEMESSAGE msg);
	/**
	*@brief �����̃S�[�����őO��ɑ}������
	*/
	bool	Insert_Goal(Character* character,Goal*insert_Goal);

	/**
	*@brief	mapobject�̋L�����擾 
	*@return �g�p�\�Ȃ��Record�̃|�C���^�Ƒ������ɐM���l���i�[�����,�g���Ȃ��ꍇ��nullptr
	*/
	MapObject_Record*	Get_MapObjectMemory(const ENTITY_ID& ID, float*reliability);
	/**
	*@brief	character�̋L�����擾
	*@return �g�p�\�Ȃ��Record�̃|�C���^�Ƒ������ɐM���l���i�[�����,�g���Ȃ��ꍇ��nullptr
	*/
	Character_Record*	Get_CharacterMemory(const ENTITY_ID& ID, float*reliability);
	inline  MEMORY& GetMemory() { return (m_memory.memory); }
	//�e�X�g
	const char*							character_Name;
	void								Log(const std::string& str);
private :
	friend class NPC_Factory;
	friend class Accesor_Brain;
	friend class NPC_Evalutor;
	friend class Goal_Think;
	friend class AI_MessageDispatcher;
};


#include	"NPC_Evalutor.h"
#include	"Accessor_Brain.h"
#include	"BlackBoard.h"

#pragma endregion

#endif
