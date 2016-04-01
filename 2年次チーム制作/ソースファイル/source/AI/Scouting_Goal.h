#ifndef __GOAL_SCOUT_H__
#define __GOAL_SCOUT_H__

class Goal_Scouting : public Goal
{
public:
	Goal_Scouting(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director);
	~Goal_Scouting()
	{
		SAFE_DELETE(m_behavior_Director);
	}
public:
	bool					Path_Search( Character* character, const bool& research, Vector3& goal_Pos);
public:
	planningkey		Effect(const Symbol& symbol);
	planningkey		Prerequisite(const Symbol& symbol);

	bool					Active(Character* character);
	goal_status		Process(Character* character);
	void					Completed(Character* character);
	void					Failed(Character* character);
};

class Behavior_Director_Scouting : public Behavior_DirectorBase
{
private:

public:
	Behavior_Director_Scouting() 
	{}
	goal_status	Direction(NPC_Brain* brain, Character* character,bool init = false);
};

#endif
