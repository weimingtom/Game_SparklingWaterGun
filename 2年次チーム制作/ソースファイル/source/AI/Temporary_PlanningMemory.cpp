#include	"..\All.h"

Temporary_PlanningMemory::Temporary_PlanningMemory(const NPC_Brain* holder) :
m_holder(holder),m_current_Regist(0)
{
	//èâä˙âª
	m_delete_Frame[(int)censor_type::sight] = 120;
	m_delete_Frame[(int)censor_type::hearing] = 300;
	m_delete_Frame[(int)censor_type::attack_me] = 180;
}

int	Temporary_PlanningMemory::Is_Register(const censor_type& type, const ENTITY_ID& register_ID)
{
	int index = 0;
	for (int i = 0; i < max_memory; i++)
	{
		if (!m_memory_Array[i].m_active)
		{
			index = i;
			continue;
		}
		if (m_memory_Array[i].m_sender_type != type)
			continue;
		
		if (m_memory_Array[i].m_rock_ID == register_ID)
			return -1;
	}
	return index;
}

bool	Temporary_PlanningMemory::Registar(const censor_type& type, const ENTITY_ID& register_ID)
{
	MyAssert(!(m_current_Regist > max_memory), "%sÇÃìoò^êîÇ™Ç¢Ç¡ÇœÇ¢Ç≈Ç∑", m_holder->character_Name);
	int register_Num = Is_Register(type, register_ID);
	if (register_Num != -1)
	{
		m_memory_Array[register_Num].m_active = true;
		m_memory_Array[register_Num].m_registar_Frame = GAMEMATCHINFO.GetMatchTime();
		m_memory_Array[register_Num].m_rock_ID = register_ID;
		m_memory_Array[register_Num].m_sender_type = type;
		m_current_Regist++;
		return true;
	}
	return false;
}

void	Temporary_PlanningMemory::Update(const int& current_Frame)
{
	for (auto& cur_mem : m_memory_Array)
	{
		if (!cur_mem.m_active)
			continue;
		int progress_Frame = current_Frame - cur_mem.m_registar_Frame;
		if (progress_Frame >= m_delete_Frame[enum_cast(cur_mem.m_sender_type)])
		{
			cur_mem.m_active = false;
			cur_mem.m_registar_Frame = -1;
			cur_mem.m_rock_ID = game_id::id_error;
			cur_mem.m_sender_type = censor_type::none;
			m_current_Regist--;
		}
	}
	//MyDebugMgrStr(dword_color::blue, "PlanningMemory %d", m_current_Regist);
}

bool	Temporary_PlanningMemory::Remove_Array(const ENTITY_ID& remove_ID)
{
	for (auto& it : m_memory_Array)
	{
		if (!it.m_active)
			continue;
		if (it.m_rock_ID == remove_ID)
		{
			it.m_active = false;
			it.m_registar_Frame = -1;
			it.m_rock_ID = game_id::id_error;
			it.m_sender_type = censor_type::none;
			m_current_Regist--;
			return true;
		}
	}
	return false;
}