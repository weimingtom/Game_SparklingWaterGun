#ifndef __GOAL_ADVERTISEMENTPOINT_H__
#define __GOAL_ADVERTISEMENTPOINT_H__



class Goal_GetAdvertisementPoint : public Goal
{
private :
	/**
	*@brief 現在のターゲットのShare率を見てターゲットに最適かどうかを判断する
	*@return trueで現在もターゲットにしてもOK
	*/
	bool				Evalute_SharePoint_Current_Target(Character* character);

public :
	Goal_GetAdvertisementPoint(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director);
	~Goal_GetAdvertisementPoint()
	{
		SAFE_DELETE(m_behavior_Director);
	}

public :
	planningkey		Effect(const Symbol& symbol);
	planningkey		Prerequisite(const Symbol& symbol);

	bool					Active(Character* character);
	goal_status		Process(Character* character);
	void					Completed(Character* character);
	void					Failed(Character* character);
	bool					HandleMessage(AI_MESSAGE* msg);
};		

class Behavior_Director_GetAdvertisementPoint : public Behavior_DirectorBase
{
private:
	goal_status Activate(NPC_Brain* brain, Character* character);
	goal_status MoveEdge(NPC_Brain* brain, Character* character);
	goal_status MovePoint(NPC_Brain* brain, Character* character);
	goal_status ShotWeapon(NPC_Brain* brain, Character* character);
	goal_status LookBack(NPC_Brain* brain, Character* character);

public:
	Behavior_Director_GetAdvertisementPoint()
	{}
	goal_status	Direction(NPC_Brain* brain, Character* character,bool init = false);
};

#endif
