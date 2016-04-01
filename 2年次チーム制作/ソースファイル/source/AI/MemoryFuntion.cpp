#include	"..\All.h"

int	MemoryFunction::Is_Register_PlanningMemory(NPC_Brain* access_Brain,
												const censor_type& type, const ENTITY_ID& target_ID)
{
	return Accesor_Brain::WorkingMemory(access_Brain)->memory.m_planning_Memory->Is_Register(type, target_ID);
}

bool	MemoryFunction::Register_PlanningMemory(NPC_Brain* access_Brain, const censor_type& type, const ENTITY_ID& target_ID)
{
	return Accesor_Brain::WorkingMemory(access_Brain)->memory.m_planning_Memory->Registar(type, target_ID);
}

bool	MemoryFunction::Remove_PlannningMemory(NPC_Brain* access_Brain, const ENTITY_ID& remove_id)
{
	return Accesor_Brain::WorkingMemory(access_Brain)->memory.m_planning_Memory->Remove_Array(remove_id);
}

bool	memory_predicate::Candidate_AttackTarget(Character_Record* record)
{
	if (!record->m_visibilily)
		return false;
	

	if (record->m_status == arive_status::dead ||
		record->m_status == arive_status::i_dont_now)
		return false;
	
	return true;
}

bool	memory_predicate::Candidate_AdvertisementPoint(MapObject_Record* record)
{
	if (record->m_object_type == mapobjcet_type::poster)
		return true;
	return false;
}

bool	memory_predicate::Candidate_MedicalFriend(Character_Record* record)
{
	if (record->m_status == arive_status::dead ||
		record->m_status == arive_status::i_dont_now)
		return false;

	return true;
}

bool	MemoryFunction::Character_IsDie(MEMORY* access_memory, const ENTITY_ID& die_character)
{
	if (access_memory->m_character_Record.find(die_character) != access_memory->m_character_Record.end())
	{
		access_memory->m_character_Record[die_character]->m_status = arive_status::dead;
		return true;
	}
	return false;
}