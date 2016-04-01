class Goal_MedicalFriend : public Goal
{
private :
	bool	Target_IsFine(Character_Record* brain);

public:
	Goal_MedicalFriend(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director);
	~Goal_MedicalFriend()
	{
		SAFE_DELETE(m_behavior_Director);
	}
public:
	bool					Path_Search( Character* character, const bool& research, Vector3& goal_Pos);
public:
	planningkey		Effect(const Symbol& symbol);
	planningkey		Prerequisite(const Symbol& symbol);

	bool					Active(Character* character);
	goal_status		Process( Character* character);
	void					Completed(Character* character);
	void					Failed(Character* character);
};

class Behavior_Director_MedicalFriend : public Behavior_DirectorBase
{
private:
	goal_status Activate(NPC_Brain* brain, Character* character);
	goal_status	MoveEdge(NPC_Brain* brain, Character* character);
	goal_status	MovePoint(NPC_Brain* brain, Character* character);
	goal_status	ShotWeapon(NPC_Brain* brain, Character* character);
public:
	Behavior_Director_MedicalFriend()
	{}
	goal_status	Direction(NPC_Brain* brain, Character* character,bool init = false);
};

