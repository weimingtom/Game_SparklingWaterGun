#include	"..\All.h"
#include	<algorithm>

mapobjcet_type Convert_MapObject_MyEnum(const MAP_OBJ_TYPE& type)
{
	if (type == MAP_OBJ_TYPE::_truck)
		return mapobjcet_type::denger_object;

	if (type >= MAP_OBJ_TYPE::_share_obj_begin && 
		type <= MAP_OBJ_TYPE::_share_obj_end)
		return mapobjcet_type::poster;

	if (type >= MAP_OBJ_TYPE::_share_obj_begin &&
		type <= MAP_OBJ_TYPE::_share_obj_end)
		return mapobjcet_type::poster;
	
	return mapobjcet_type::error;
}

void WorkingMemory::Initialize(NPC_Brain*	brain)
{
	m_holder = brain;
	memory.All_Clear();
	memory.m_planning_Memory = new Temporary_PlanningMemory(brain);
	memory.m_goal_Result = new Goal_Result(brain, 200);
	Initialize_MapObject();
}

void	WorkingMemory::Initialize_MapObject()
{
	MapObjectManager::MAPOBJ_ARRAY work_Array;
	MAP_MANAGER.Get_ShareTypeObject(work_Array);
	MAP_MANAGER.Get_TypeObject(work_Array, MAP_OBJ_TYPE::_truck);
	for (auto it : work_Array)
	{
		ENTITY_ID insert_ID = it->GetId();
		memory.Is_Record_MapObject(insert_ID);
		//初期配置をあらかじめ記憶に書き込んでおく
		MapObject_Record* record_ptr = memory.m_mapobject_Record[insert_ID];
		record_ptr->m_became_Pos = it->Get_Param().pos;
		record_ptr->m_sensed_Pos = it->Get_Param().pos;
		record_ptr->m_object_type = Convert_MapObject_MyEnum(it->Get_Type());
		//record_ptr->m
	}
}

WorkingMemory::~WorkingMemory()
{
	memory.All_Release();
}

bool WorkingMemory::HandleMessage(AI_MESSAGE* msg)
{
	switch (msg->type)
	{

	case ai_messagetype::chracter_Die : 
		if (!MemoryFunction::Character_IsDie(&memory, *static_cast<ENTITY_ID*>(msg->extra_Info)))
		{
			MyAssert(false, "本来あったらこまること");
			return true;
		}
		break;
	}
	return true;
}

bool WorkingMemory::Update(const int& current_Frame, Character* character)
{
	Update_Character_Record();
	memory.m_planning_Memory->Update(current_Frame);
	memory.m_goal_Result->Update(current_Frame);
	if (!memory.m_character_Record.empty())
	{
		for (auto& mem : memory.m_character_Record)
		{
			//MyDebugMgrStr(dword_color::blue, "bisibility %d", mem.second->m_visibilily);
			MyDebugMgrStr(dword_color::blue, "action %s", Get_ExtActStr(mem.second->m_action.m_tag));
			MyDebugMgrStr(dword_color::blue, "キャラ記憶%d",memory.m_character_Record.size());
		}
	}
	//MyDebugMgrStr(dword_color::white, "Chara記憶数%d", memory.m_character_Record.size());
	return true;
}

void	WorkingMemory::Update_Character_Record()
{
	for (auto it = memory.m_character_Record.begin(); it != memory.m_character_Record.end();)
	{
		if (it->second->Update())
		{
			SAFE_DELETE(it->second);
			memory.m_character_Record.erase(it++);
			continue;
		}
		it++;
	}
}

