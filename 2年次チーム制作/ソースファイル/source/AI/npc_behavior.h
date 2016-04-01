#ifndef __NPC_BEHAVIOR_H__
#define __NPC_BEHAVIOR_H__

#include "../EnumCP11.h"

class Field_Edge;
class NPC_Brain;
class Character;
class Goal;
class WayPointInformation;
class NPC_Aim;

class Field_Node;
class Field_Edge;

template<class Node_Type, class Edge_Type>
class PathSearch_Base;

#pragma region enum

_ENUM_BEGIN(ai_messagetype, AI_MESSAGETYPE)

error = -1,
//beahvior_msg
search_path_ok,				//�o�H�T��Ok
demand_search_path,		//�o�H�T���\��
set_route,						//LIST�Ƀ��[�g�𗬂�����
set_currentGoalPos,			//�^�[�Q�b�g���W�ݒ�
set_LookVec,					//targetRotateVec���Z�b�g�iAnime_LookBack�j
update_memory,				//�L�����X�V���ꂽ
lock_goal,						//�S�[���̃��b�N
chracter_Die,
wait,
set_messsage_bias,			//�ŏI�S�[���]���֐��ɕ␳�l�����Z
//goal_msg
evalute_currentGoalPos,	//

_ENUM_END(ai_messagetype, AI_MESSAGETYPE)

struct AI_MESSAGE
{
	ai_messagetype type;
	void*	extra_Info;

	AI_MESSAGE(const ai_messagetype& type, void* extra_Info) :
		type(type), extra_Info(extra_Info){}
	AI_MESSAGE(){}

};


_ENUM_BEGIN(behavior_tag, BEHAVIOR_TAG)

error = -1,
move_edge,
move_point,
shot_weapon_stand,
shot_weapon_circle,
rolling_attack,
charge_weapon,
look_back,
grenede_throw,
air_borne,

num_behavior

_ENUM_END(behavior_tag, BEHAVIOR_TAG)

inline char* message_char(const ai_messagetype & tag)
{
	static char* ret[] =
	{
		"search_path_ok",				//�o�H�T��Ok
		"demand_search_path",		//�o�H�T���\��
		"set_route",						//LIST�Ƀ��[�g�𗬂�����
		"set_currentGoalPos",			//�^�[�Q�b�g���W�ݒ�
		"set_LookVec",					//targetRotateVec���Z�b�g�iAnime_LookBack�j
		"update_memory",				//�L�����X�V���ꂽ
	};
	return ret[enum_cast(tag)];
}

#pragma endregion

//�o�H�Č������ɕK�v�ȏ�񃉃b�s���O
struct R_Targetwrapping
{
	Character*	search_Character;	//��������L�����N�^�[
	bool				see_Through;
	Vector3		current_GoalPos;
	R_Targetwrapping(){}
	R_Targetwrapping(Character* c, const bool& s,const Vector3& current_Goal/*,const OBJECTTYPE& type*/) :
		search_Character(c), see_Through(s),current_GoalPos(current_Goal)/*,object_Type(type)*/{}
	R_Targetwrapping(const R_Targetwrapping&);

};


class AI_BehaviorBase
{
	friend class Behavior_Manager;
protected :
	Goal*										m_parent;	//	�U�镑���̕ێ���
	goal_status								m_status;
	B_ProcessFunc							m_process_Function;
public :
	const behavior_tag tag;
public:
	AI_BehaviorBase(const behavior_tag& init_tag, const char* name) : 
		tag(init_tag), behavior_Name(name),m_parent(nullptr),m_process_Function(nullptr){}
	virtual ~AI_BehaviorBase(){}
	const char* behavior_Name;
public:
	virtual void Enter(NPC_Brain* brain, Character* character) = 0;
	virtual goal_status Execute(NPC_Brain* brain, Character* character) = 0;
	virtual void Exit(NPC_Brain* brain, Character* character) = 0;
	virtual bool HandleMessage(AI_MESSAGE* msg) = 0;
	inline const goal_status Get_Status(){ return m_status; }
};

#pragma region Goto(MoveEdge,Point)

class PathPlanner;

//�G�b�W�̈ړ�
class Move_Edge : public AI_BehaviorBase
{
	friend class PathPlanner;
	static const int move_error_time = 60;
	static const int	wait_error_time = 120;
private :
	std::list<Field_Edge*>			route_List;
	bool											m_wait;
	bool											m_jump;
	float											m_max_rotate_Speed;
	Vector3									m_before_Pos;	//�P�t���[���O�̍��W
	int											m_move_ErrorTimer;	
	int											m_wait_ErrorTimer;
	
private :
	/**
	*@brief	���݂̌o�H�𕽊�������
	*@note	����������͈̂ꂩ������
	*/
	bool	Smoothing_Route(Character* character);
	/**
	*@brief 
	*/
	bool	Check_Erase_Edge(Character* character, const Vector3& current_Pos,
											const Vector3& tonode_Pos,const float& dist_sqs);
	goal_status	Move(Character* character, const Vector3& current_Pos, const Vector3& tonode_Pos);
	/**
	*@brief �i���`�F�b�N
	*/
	bool	Check_Difference_In_Level(Character* character,const Vector3& current_Pos,const Vector3& to_NodePos);
	goal_status	Jump(Character* character, const Vector3& current_Pos, const Vector3& tonode_Pos,
		 Field_Edge* current_Edge);
	
	void	SetRoute(PathSearch_Base<Field_Node, Field_Edge>* alg);
	/**
	*@brief ��Q���Ɉ����������ĂȂ����`�F�b�N����
	*@return �����������Ă���ꍇ��false
	*/
	bool	MoveCheck(const Vector3& current_Pos);

	/**
	*@brief �o�H�T���ҋ@���A���炩�̌����ŒT���ł��Ȃ��Ȃ��Ă��܂����Ƃ��̂��߂̉����
	*/
	bool	WaitErrorCheck();
	/**
	*@brief AI���F�����Ă��Ȃ�MapObject�i�N���[�����j�ɂ��R�Ȃǂ��痎���Ă��܂������̑Ή���
	*/
	bool	MoveErrorCheck(Character* character,const Vector3& current_Pos, const Vector3& current_TargetPos);

public :
	Move_Edge();
	~Move_Edge();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);

public :
	void	Set_DebugEdge();
};

//�w�肵�����W�Ɉړ�
class Move_Point : public AI_BehaviorBase
{
	static const int move_error_time = 60;
private:
	bool						m_Active;	//�S�[�����W���Z�b�g����Ă��邩�ǂ����̃t���O
	bool						m_jump;
	Vector3				m_goal_Pos;
	Vector3				m_before_Pos;	//��t���[���O�̍��W
	int						m_move_ErrorTimer;

private :
	void	Move(NPC_Brain* brain, Character* character,const Vector3& cur_Pos);
	void	Jump(NPC_Brain* brain, Character* character,const Vector3& cur_Pos);
	bool	Check_Difference_In_Level(Character* character,const Vector3& cur_Pos);
	bool	MoveCheck(const Vector3& cur_Pos);
	bool	GoalCheck(const Vector3& cur_Pos);

public :
	Move_Point();
	~Move_Point();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);

};
#pragma endregion

//******************************************************
//	AnimationState
//******************************************************
#pragma region SHOT_WEAPON(���̕ӂЂǂ����炻�̂�����������)

/**********����ōU��(�s��)*********/
class Anime_ShotWeapon_Stand : public AI_BehaviorBase
{
private:
	ENTITY_ID								m_target_Id;
	ENTITY_ID								m_holder_Id;
	NPC_Aim*								m_aim;
private :
	//void	Aim(NPC_Brain* brain,Character* character);
public:
	Anime_ShotWeapon_Stand(NPC_Aim* use_aimObject);
	~Anime_ShotWeapon_Stand();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

class Anime_ShotWeapon_Circle: public AI_BehaviorBase
{
	static const int							m_timer = 120;	//�Ȃ�炩�̌����ł��̏ꂩ�瓮���Ȃ��܂ܑ����Ă��ꍇ�̃^�C�}�[
private:
	ENTITY_ID								m_target_Id;
	ENTITY_ID								m_holder_Id;
	Vector3									m_current_Centor_Pos;						//���݂̉~�̒��S���W
	const WayPointInformation*	m_current_CentorNodeInformation;	//���S�̃m�[�h�̏��
	int											m_current_MoveTargetIndex;				//container��̓Y��std::vector<Field_Node*> m_even_with_Circle[]		[] <-���̐���
	int											m_current_MoveCircleSize;					//std::vector<Field_Node*> m_even_with_Circle�̃T�C�Y
	NPC_Aim*								m_aim;
	Vector3									m_before_Pos;	//�O�̃t���[���̍��W
	int											m_current_Time;

private:
	void	Culculate_WayPointInformation(const Vector3& pos);
	void	Move(NPC_Brain* brain, Character* character);
	void	Shuffle_Point();
public:
	Anime_ShotWeapon_Circle(NPC_Aim* use_aimObject);
	~Anime_ShotWeapon_Circle();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

#pragma endregion 

class Anime_ChargeWeapon : public AI_BehaviorBase
{
private :
	int	m_need_Charge;
public :
	Anime_ChargeWeapon();
	~Anime_ChargeWeapon();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

class Anime_RollingAttack : public AI_BehaviorBase
{
public:
	Anime_RollingAttack();
	~Anime_RollingAttack();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

class Anime_ThrowGrenade : public AI_BehaviorBase
{
public:
	Anime_ThrowGrenade();
	~Anime_ThrowGrenade();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

//�U�����
class Anime_LookBack : public AI_BehaviorBase
{
	static const int m_lookBack_Time = 200;
private :
	Vector3	m_look_Vec;	//�����ׂ�����
	bool			m_update_Memory;	//�U���������ŋL�����X�V���ꂽ���ǂ���
	bool			m_wait;			//�U������Ă���̑ҋ@�t���O
	int			m_timer;
public:
	Anime_LookBack();
	~Anime_LookBack();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

class Anime_AirBorne : public AI_BehaviorBase
{
	static const int m_changevec_intarval = 250;
	
private :
	int			m_timer;
	Vector3	m_move_vec;	//�ړ�����
	float m_radius;
private :
	void	Direction_MoveVec();
	void	Radius_Check(const Vector3& player_pos);

public:
	Anime_AirBorne();
	~Anime_AirBorne();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

#pragma region Manager

class Behavior_Manager
{
	friend class AI_BehaviorBase;
private :
	AI_BehaviorBase*	m_current_Behavior;
	behavior_tag			m_current_tag;
	NPC_Brain*				m_holder;
	std::vector<AI_BehaviorBase*>	m_behavior_Container;

private :
	void	InitializeContainer();

public :
	Behavior_Manager(NPC_Brain* holder);
	~Behavior_Manager();

public :
	bool	Change_Behavior(Character* character,const behavior_tag& next_behavior,
										Goal* parent,B_ProcessFunc use_process_Function);
	const goal_status			Get_BehaviorStatus();
	void	Set_MoveEdgeSearch_Interval(const int& interval);
	void	Exit_Current_Behavior(Character* character);
	goal_status Execute_Current_Behavior(Character* character);
	bool	Behavior_IsActive();
	inline	AI_BehaviorBase*		Get_Current_Behavior(){ return m_current_Behavior; }
	inline	const behavior_tag&	Get_Current_Tag(){ return m_current_tag; }
};

#pragma endregion

#endif
