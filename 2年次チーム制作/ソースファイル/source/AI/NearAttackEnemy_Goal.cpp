#include "..\All.h"

#pragma region GOAL

Goal_NearAttackEnemy::Goal_NearAttackEnemy(
	NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director)
: Goal(brain, "NearAttackEnemy", evo, goal_tag::nearattack_enemy, use_director)
{
	m_behavior_Director->Set_Parent(this);
}

bool	Goal_NearAttackEnemy::Active(Character* character)
{
	if (!Accesor_Brain::Evalutor(m_holder)->Direction_NearAttackTarget(m_holder, character))
	{
		MyAssert(false, "これはやばい");
		return false;
	}

	if (m_behavior_Director->Direction(m_holder, character, true) == goal_status::failed)
		MyAssert(false, "これはやばい");

	return true;
}

goal_status	Goal_NearAttackEnemy::Process(Character* character)
{
	
	goal_status ret = Behavior(character);
	return ret;
	
}

void	Goal_NearAttackEnemy::Completed(Character* character)
{

}

void	Goal_NearAttackEnemy::Failed(Character* character)
{

}

bool Goal_NearAttackEnemy::HandleMessage(AI_MESSAGE* msg)
{

	return false;
}

#pragma endregion


#pragma region Director

goal_status Behavior_Director_NearAttackEnemy::Activate(NPC_Brain* brain, Character* character)
{
	float reliability;
	Character_Record* target_record = brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain), &reliability);
	if (!target_record)
	{
		MyAssert(false, "なぞのエラー発生");
		brain->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(brain), false);
		return goal_status::failed;
	}
	//近接攻撃射程内ならローリングサンダー！
	if (Accesor_Brain::Evalutor(brain)->Target_is_NearRange(brain, character))
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::rolling_attack,
			m_holder, behavior_processfunc::RollingAttack);
		return goal_status::active;
	}

	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	Vector3 target_Pos = target_record->m_sensed_Pos;
	Field_Node* current_Node = navigation_system->Find_Node_Pos(my_Pos);
	Field_Node* goal_Node = navigation_system->Find_Node_Pos(target_Pos);
	if (!current_Node || !goal_Node)
	{
		//MyAssert(false, "ゴールかスタートが見つからないよ,やばい");
		brain->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(brain), false);
		return goal_status::failed;
	}
	//MoveEdge
	if (current_Node->index != goal_Node->index)
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_edge,
			m_holder, behavior_processfunc::move_edge::Arrival_GoalPoint);
		Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(),target_Pos,
			Accesor_Brain::Param(brain).m_interval_characterTarget_SearchPath);
		//ai_message->Demand_Search_Path(brain, character, target_Pos, false);
		return goal_status::active;
	}
	//MovePoint
	Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_point,
		m_holder, behavior_processfunc::move_point::Arrival_GoalPoint);
	ai_message->Send_BehaviorMessage(brain, character, ai_messagetype::set_currentGoalPos, &target_Pos);

	return goal_status::active;
}

goal_status Behavior_Director_NearAttackEnemy::MoveEdge(NPC_Brain* brain, Character* character)
{
	if (Accesor_Brain::Evalutor(brain)->Target_is_NearRange(brain, character))
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::rolling_attack,
			m_holder, behavior_processfunc::RollingAttack);
		return goal_status::active;
	}

	return goal_status::failed;
}

goal_status Behavior_Director_NearAttackEnemy::MovePoint(NPC_Brain* brain, Character* character)
{
	if (Accesor_Brain::Evalutor(brain)->Target_is_NearRange(brain, character))
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::rolling_attack,
			m_holder, behavior_processfunc::RollingAttack);
		return goal_status::active;
	}
	return goal_status::failed;
}

goal_status Behavior_Director_NearAttackEnemy::RollingAttack(NPC_Brain* brain, Character* character)
{
	return goal_status::completed;
}

goal_status Behavior_Director_NearAttackEnemy::Direction(NPC_Brain* brain, Character* character, bool	init)
{
	if (init)
		return Activate(brain, character);
	
	goal_status ret = goal_status::error;
	switch (Accesor_Brain::BehaviorManager(brain)->Get_Current_Tag())
	{
	case behavior_tag::move_edge:
		ret = MoveEdge(brain, character);
		break;

	case behavior_tag::move_point:
		ret = MovePoint(brain, character);
		break;

	case behavior_tag::rolling_attack:
		ret = RollingAttack(brain, character);
		break;
	}

	return ret;
}

#pragma endregion


