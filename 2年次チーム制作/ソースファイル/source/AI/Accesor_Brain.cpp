#include	"..\All.h"

NPC_Evalutor* Accesor_Brain::Evalutor(NPC_Brain* access_Brain)
{
	return access_Brain->m_evalutor;
}

ENTITY_ID Accesor_Brain::Current_Target(NPC_Brain* access_Brain)
{
	return access_Brain->m_target.current;
}

const NPCParam& Accesor_Brain::Param(NPC_Brain* access_Brain)
{
	return access_Brain->m_chara_Param;
}

const Symbol& Accesor_Brain::Symbol(NPC_Brain* access_Brain)
{
	return access_Brain->m_symbol;
}

FinalGoal_Evalutor* Accesor_Brain::Final_GoalEvalutor(NPC_Brain* access_Brain)
{
	return access_Brain->m_think->Get_FinalGoal();
}

const RAP_CENSROR* Accesor_Brain::Censor(NPC_Brain* access_Brain)
{
	return &access_Brain->m_censor;
}

const StatusParam& Accesor_Brain::StatusParam(NPC_Brain* access_Brain)
{
	return access_Brain->m_status_Param;
}

NPC_FuzzyTool* Accesor_Brain::FuzzyTool(NPC_Brain* access_Brain)
{
	return &access_Brain->m_fuzzy;
}

Behavior_Manager* Accesor_Brain::BehaviorManager(NPC_Brain* access_Brain)
{
	return access_Brain->m_behavior_Manager;
}

void	Accesor_Brain::Set_Current_Target(NPC_Brain* access_Brain, const ENTITY_ID& set_ID)
{
	access_Brain->m_target.current = set_ID;
}

void	Accesor_Brain::Target_Clear(NPC_Brain* access_Brain)
{
	access_Brain->m_target.current = ENTITY_ID::id_error;
}

WorkingMemory*	Accesor_Brain::WorkingMemory(NPC_Brain* access_Brain)
{
	return &access_Brain->m_memory;
}

PathPlanner*	Accesor_Brain::PathPlanner(NPC_Brain* access_Brain)
{
	return access_Brain->m_path_Planner;
}

const DeFuzzify_Param& Accesor_Brain::FuzzyParam(NPC_Brain* access_Brain)
{
	return access_Brain->m_defuzzify_param;
}

ParticularGoal_Manager*	Accesor_Brain::ParticularGoal_Manager(NPC_Brain* access_Brain)
{
	return access_Brain->m_particularGoal_Manager;
}

NPCFriendMessage* Accesor_Brain::FriendMessage(NPC_Brain* access_Brain)
{
	return access_Brain->m_message_Icon;
}