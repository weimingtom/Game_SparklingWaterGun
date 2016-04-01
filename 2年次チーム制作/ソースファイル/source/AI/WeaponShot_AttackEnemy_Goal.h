#ifndef __ATTACK_ENEMY_GOAL_H__
#define __ATTACK_ENEMY_GOAL_H__


class Goal_WeaponShot_AttackEnemy : public Goal
{
private :
	/**
	*@brief ターゲットIDが有効かどうかをチェック
	*/
	bool	Target_IsActive();
public:
	Goal_WeaponShot_AttackEnemy(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director);
	~Goal_WeaponShot_AttackEnemy()
	{
		SAFE_DELETE(m_behavior_Director);
	}
	
public:
	planningkey		Effect(const Symbol& symbol);
	planningkey		Prerequisite(const Symbol& symbol);

	bool					Active(Character* character);
	goal_status		Process(Character* character);
	void					Completed( Character* character);
	void					Failed(Character* character);
	bool					HandleMessage(AI_MESSAGE*msg);
	
};

class Behavior_Director_WeaponShot_AttackEnemy : public Behavior_DirectorBase
{
private :
	goal_status	Activate(NPC_Brain* brain, Character* character);
	goal_status	MoveEdge(NPC_Brain* brain, Character* character);
	goal_status	MovePoint(NPC_Brain* brain, Character* character);
	goal_status	ShotWeapon(NPC_Brain* brain, Character* character);
public :
	Behavior_Director_WeaponShot_AttackEnemy()
	{}
	goal_status	Direction(NPC_Brain* brain, Character* character,bool	init =false);
};


#endif
