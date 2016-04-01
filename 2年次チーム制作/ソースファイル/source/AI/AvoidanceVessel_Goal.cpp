#include	"..\All.h"

#pragma region GOAL

Goal_AvoidanceVessel::Goal_AvoidanceVessel(
	NPC_Brain* brain, Behavior_DirectorBase* use_director)
	: Goal(brain, "AvoidanceVessel", nullptr, goal_tag::avoidancevessel, use_director)
{

}

bool	Goal_AvoidanceVessel::Active(Character* character)
{
	//ターゲット設定

	m_avoidance_Target = Accesor_Brain::ParticularGoal_Manager(m_holder)->Get_TargetID();
	float reliability;
	MapObject_Record*	target_Record = m_holder->Get_MapObjectMemory(m_avoidance_Target, &reliability);
	if (!target_Record)
	{
		MyAssert(false, "これはおかしい");
		return false;
	}
	if (!Accesor_Brain::Evalutor(m_holder)->Direction_AvoidancePoint(
					m_holder, character,target_Record,&m_avoidance_Point))
		return false;

	Accesor_Brain::BehaviorManager(m_holder)->Change_Behavior(character, behavior_tag::move_point,
		this, behavior_processfunc::move_point::Arrival_GoalPoint);
	ai_message->Send_BehaviorMessage(m_holder, character, ai_messagetype::set_currentGoalPos, &m_avoidance_Point);

	return true;
}

goal_status	Goal_AvoidanceVessel::Process(Character* character)
{
	status = Behavior(character);
	return status;
}

void	Goal_AvoidanceVessel::Completed(Character* character)
{
	status = goal_status::inactive;
}

void	Goal_AvoidanceVessel::Failed(Character* character)
{
	status = goal_status::inactive;
}

bool Goal_AvoidanceVessel::HandleMessage(AI_MESSAGE* msg)
{
	return false;
}

#pragma endregion


#pragma region Director

//goal_status Behavior_Director_AvoidanceVessel::Activate(NPC_Brain* brain, Character* character)
//{
//
//	return goal_status::active;
//}
//

goal_status Behavior_Director_AvoidanceVessel::Direction(NPC_Brain* brain, Character* character, bool	init)
{
	switch (Accesor_Brain::BehaviorManager(brain)->Get_Current_Tag())
	{
	case behavior_tag::move_point : 
		return goal_status::completed;
		break;
	}

	return goal_status::failed;
}

#pragma endregion
