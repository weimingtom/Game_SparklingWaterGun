#pragma once 

class Goal_Airborne : public Goal
{

public:
	Goal_Airborne(NPC_Brain* brain, Behavior_DirectorBase* use_director);
	~Goal_Airborne()
	{
		SAFE_DELETE(m_behavior_Director);
	}
public:
	planningkey		Effect(const Symbol& symbol){ return planningkey::error; }
	planningkey		Prerequisite(const Symbol& symbol){ return planningkey::error; }

	bool					Active(Character* character);
	goal_status		Process(Character* character);
	void					Completed(Character* character);
	void					Failed(Character* character);
	bool					HandleMessage(AI_MESSAGE* msg);
};

class Behavior_Director_Goal_Airborne : public Behavior_DirectorBase
{
private:
	goal_status	Activate(NPC_Brain* brain, Character* character);

public:
	Behavior_Director_Goal_Airborne()
	{}
	goal_status	Direction(NPC_Brain* brain, Character* character, bool	init = false);
};

