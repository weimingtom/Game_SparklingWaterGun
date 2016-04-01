#include	"..\All.h"


#pragma region GOAL

Goal_GrenadeAttack::Goal_GrenadeAttack(
	NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director)
	: Goal(brain, "GrenadeAttack", evo, goal_tag::grenede_attack,use_director)
{
	m_behavior_Director->Set_Parent(this);
}

bool	Goal_GrenadeAttack::Active(Character* character)
{
	if (!Accesor_Brain::Evalutor(m_holder)->Direction_GrenadeAttackTarget(m_holder, character))
		return false;
	
	if (!m_behavior_Director->Direction(m_holder, character, true))
		return false;

	return true;
}

goal_status	Goal_GrenadeAttack::Process(Character* character)
{
	return goal_status::active;
}

void	Goal_GrenadeAttack::Completed(Character* character)
{

}

void	Goal_GrenadeAttack::Failed(Character* character)
{

}

bool Goal_GrenadeAttack::HandleMessage(AI_MESSAGE* msg)
{
	return false;
}

#pragma endregion


#pragma region Director

goal_status Behavior_Director_GrenadeAttack::Activate(NPC_Brain* brain, Character* character)
{
	
	return goal_status::active;
}

goal_status Behavior_Director_GrenadeAttack::Direction(NPC_Brain* brain, Character* character, bool	init)
{
	if (init)
		return Activate(brain, character);
	
	return goal_status::completed;
}

#pragma endregion
