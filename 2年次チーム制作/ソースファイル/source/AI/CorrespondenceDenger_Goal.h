#ifndef Goal_CorrespondenceDenger_H
#define Goal_CorrespondenceDenger_H


class Goal_CorrespondenceDenger : public Goal
{
	enum b_num{ none = -1,escape, lookback };

private:
	FuzzyModule* m_dicision_Module;	//ì¶Ç∞ÇÈÇ©êÌÇ§Ç©ÇåàÇﬂÇÈ

	void					Initialize_FuzzyModule();
	bool					Evalute_CurrentGoalPos(R_Targetwrapping* object);
	//goal_status	Directon_Behavior(Character* character);
public:
	Goal_CorrespondenceDenger(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director);
	~Goal_CorrespondenceDenger()
	{
		SAFE_DELETE(m_behavior_Director);
	}
//public:
//	bool					Path_Search( Character* character, const bool& research, Vector3& goal_Pos);
public:
	planningkey		Effect(const Symbol& symbol);
	planningkey		Prerequisite(const Symbol& symbol);

	bool					Active(Character* character);
	goal_status		Process( Character* character);
	void					Completed(Character* character);
	void					Failed( Character* character);
	bool					HandleMessage(AI_MESSAGE* msg);
};

class Behavior_Director_CorrespondenceDenger : public Behavior_DirectorBase
{
private:
	goal_status Activate(NPC_Brain* brain, Character* character);

public:
	Behavior_Director_CorrespondenceDenger() 
	{}
	goal_status	Direction(NPC_Brain* brain, Character* character,bool init = false);
};

#endif

