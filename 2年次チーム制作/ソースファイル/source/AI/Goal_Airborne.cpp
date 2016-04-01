#include	"..\All.h"

#pragma region GOAL

Goal_Airborne::Goal_Airborne(
	NPC_Brain* brain, Behavior_DirectorBase* use_director)
	: Goal(brain, "AvoidanceVessel", nullptr, goal_tag::avoidancevessel, use_director)
{

}

bool	Goal_Airborne::Active(Character* character)
{
	Accesor_Brain::BehaviorManager(m_holder)->Change_Behavior(character, behavior_tag::air_borne,
		this,nullptr);
	status = goal_status::active;
	return true;
}

goal_status	Goal_Airborne::Process(Character* character)
{
	status = Behavior(character);
	return status;
}

void	Goal_Airborne::Completed(Character* character)
{
	status = goal_status::inactive;
}

void	Goal_Airborne::Failed(Character* character)
{
	status = goal_status::inactive;
}

bool Goal_Airborne::HandleMessage(AI_MESSAGE* msg)
{
	return false;
}

#pragma endregion


#pragma region Director

goal_status Behavior_Director_Goal_Airborne::Direction(NPC_Brain* brain, Character* character, bool	init)
{
	if (init)
	{
		if (Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character,
			behavior_tag::air_borne, m_holder, nullptr))
			MyAssert(false, "ChangeBehavior::AirBorne‚ÉŽ¸”s");
		return goal_status::active;
	}

	return goal_status::completed;
}

#pragma endregion
