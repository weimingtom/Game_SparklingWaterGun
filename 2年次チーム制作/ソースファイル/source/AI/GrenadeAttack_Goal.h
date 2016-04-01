#ifndef __GRENADEATTACK_GOAL_H__
#define __GRENADEATTACK_GOAL_H__

class Goal_GrenadeAttack : public Goal
{
public:
	Goal_GrenadeAttack(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director);
	~Goal_GrenadeAttack()
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
	bool					HandleMessage(AI_MESSAGE*msg);

	
};


class Behavior_Director_GrenadeAttack : public Behavior_DirectorBase
{
private:
	goal_status	Activate(NPC_Brain* brain, Character* character);

public:
	Behavior_Director_GrenadeAttack()
	{}
	goal_status	Direction(NPC_Brain* brain, Character* character, bool	init = false);
};


#endif

