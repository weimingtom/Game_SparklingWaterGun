#ifndef __GOAL_H__
#define __GOAL_H__
#include	"NPC_Enum.h"

class NPC_Brain;
class Character;
class Symbol;
class CharacterMoveStateBase;
class AI_BehaviorBase;
struct AI_MESSAGE;
class AI_MessageDispatcher;
class Behavior_DirectorBase;
class Goal;

typedef float(*GoalCost_Calculater)(Goal* connection_source, NPC_Brain*,Character*,
														const float& defalut_cost);

class Goal
{
	friend class Action_Planner;
	friend class AI_MessageDispatcher;
protected:
	
	GoalCost_Calculater					m_cost_Calculater;		//�S�[���R�X�g�v�Z�֐�
	goal_tag									m_tag;								//�S�[����
	NPC_Brain*								m_holder;							//���̃S�[���̕ێ���
	int											m_last_EndFrame;		//�Ō�ɐ����A���s�����t���[��
	Behavior_DirectorBase*			m_behavior_Director;
protected:
	int											timer;							//�S�[���̐�������

protected:
	goal_status								Behavior(Character* character);
	//bool											Path_Research(Character* character,  Vector3 & goal_Pos);
	void											End(Character* character);
public:
	goal_status								status;							//�S�[���̏��
	Goal(NPC_Brain* holder, char* goalname, GoalCost_Calculater calculater,
		const goal_tag& tag,Behavior_DirectorBase* use_director) :
			goal_Name(goalname), status(goal_status::inactive), m_cost_Calculater(calculater), timer(0), m_tag(tag),
		m_holder(holder), m_behavior_Director(use_director)
	{
	}
	virtual ~Goal(){}

public:
	char*										goal_Name;	//�S�[���̖��O
	//**********************************************************
	//	�v�����j���O�p�֐�
	//**********************************************************
	virtual planningkey					Effect(const Symbol& symbol) = 0;
	virtual planningkey					Prerequisite(const Symbol& symbol) = 0;
public:
	//**********************************************************
	//	�G�t�F�N�^�[
	//**********************************************************
	virtual bool								Active(Character* character) = 0;
	virtual goal_status					Process(Character* character) = 0;
	virtual void								Completed( Character* character) = 0;
	virtual void								Failed(Character* character) = 0;
public :
	inline const goal_tag&						Get_Tag(){ return m_tag; }
	virtual	bool								HandleMessage(AI_MESSAGE* msg){ return false; }
};

class Behavior_DirectorBase
{
protected :
	Goal*	m_holder;
public :
	Behavior_DirectorBase() :m_holder(nullptr)
	{}
	virtual ~Behavior_DirectorBase(){}
	virtual goal_status	Direction(NPC_Brain* brain, Character* character,bool	enter = false) = 0;
	void		Set_Parent(Goal* parent){ m_holder = parent; }
};

#endif
