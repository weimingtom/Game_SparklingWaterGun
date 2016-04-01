#include	"..\All.h"


Goal_Result::Goal_Result(NPC_Brain* holder, const int& active_frame) :
m_holder(holder), m_active_Frame(active_frame), m_current_Regist(0)
{

}

bool	Goal_Result::Is_Register(const ENTITY_ID& check_id, int* iterator)
{
	int	index = 0;
	for (int i = 0; i < max_memory; i++)
	{
		if (!m_memory_Array[i].m_active)
		{
			index = i;
			continue;
		}
		if (m_memory_Array[i].m_id == check_id)
		{
			if (iterator != nullptr)
				*iterator = i;
			return true;
		}
	}
	if (iterator != nullptr)
		*iterator = index;
	return false;
}

bool	Goal_Result::Register(const ENTITY_ID& register_id, const bool&	result)
{
	MyAssert(!(m_current_Regist > max_memory), "%s‚Ì“o˜^”‚ª‚¢‚Á‚Ï‚¢‚Å‚·", m_holder->character_Name);
	int register_Index;
	if (!Is_Register(register_id, &register_Index))
	{
		MyAssert(!(register_Index > max_memory || register_Index < 0), "‚±‚ê‚Í‚â‚Î‚¢");

		m_memory_Array[register_Index].m_active = true;
		m_memory_Array[register_Index].m_registar_Frame = GAMEMATCHINFO.GetMatchTime();
		m_memory_Array[register_Index].m_id = register_id;
		m_memory_Array[register_Index].m_result = (result) ? p_result::COMPLETED : p_result::FAILED;
		m_current_Regist++;
		return true;
	}
	return false;
}

bool	Goal_Result::Remove_Array(const ENTITY_ID& remove_id)
{
	for (auto& it : m_memory_Array)
	{
		if (!it.m_active)
			continue;
		if (it.m_id == remove_id)
		{
			it.m_active = false;
			it.m_registar_Frame = -1;
			it.m_id = game_id::id_error;
			it.m_result = p_result::NONE;
			m_current_Regist--;
			return true;
		}
	}
	return false;
}

void	Goal_Result::Update(const int& current_Frame)
{
	for (auto& cur_mem : m_memory_Array)
	{
		if (!cur_mem.m_active)
			continue;
		int progress_Frame = current_Frame - cur_mem.m_registar_Frame;
		if (progress_Frame >= m_active_Frame)
		{
			cur_mem.m_active = false;
			cur_mem.m_registar_Frame = -1;
			cur_mem.m_id = game_id::id_error;
			cur_mem.m_result = p_result::NONE;
			m_current_Regist--;
		}
	}
	MyDebugMgrStr(dword_color::blue, "Planning_Result %d", m_current_Regist);
}
