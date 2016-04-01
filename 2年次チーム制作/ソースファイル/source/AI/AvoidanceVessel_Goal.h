#ifndef __AVOIDANCEVESSEL_GOAL_H__
#define __AVOIDANCEVESSEL_GOAL_H__

class Goal_AvoidanceVessel : public Goal
{
private :
	ENTITY_ID	m_avoidance_Target;
	Vector3		m_avoidance_Point;

public:
	Goal_AvoidanceVessel(NPC_Brain* brain, Behavior_DirectorBase* use_director);
	~Goal_AvoidanceVessel()
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

class Behavior_Director_AvoidanceVessel : public Behavior_DirectorBase
{
private:
	goal_status	Activate(NPC_Brain* brain, Character* character);
	
public:
	Behavior_Director_AvoidanceVessel()
	{}
	goal_status	Direction(NPC_Brain* brain, Character* character, bool	init = false);
};


#endif
