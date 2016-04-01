#ifndef __GOAL_WANDER_FIELD_H__
#define __GOAL_WANDER_FIELD_H__

class Goal_WanderField : public Goal
{
protected: 
	enum b_num{none = -1,move_edge,move_point};

private :
	bool				Direction_Target(Character* character);
public:
	Goal_WanderField(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director);
	~Goal_WanderField()
	{
		SAFE_DELETE(m_behavior_Director);
	}
	
public:
	planningkey		Effect(const Symbol& symbol);
	planningkey		Prerequisite(const Symbol& symbol);

	bool					Active(Character* character);
	goal_status		Process(Character* character);
	void					Completed(Character* character);
	void					Failed(Character* character);
	bool					HandleMessage(AI_MESSAGE* msg);
};

class Behavior_Director_WanderField : public Behavior_DirectorBase
{
private:
	bool		Direction_Target(NPC_Brain* brain,Character* character);
public:
	Behavior_Director_WanderField()
	{}
	goal_status	Direction(NPC_Brain* brain, Character* character,bool init = false);
};

#endif
