#include	"..\All.h"

goal_status Goal::Behavior( Character* character)
{
	goal_status current_status = goal_status::active;
	Behavior_Manager* behavior_m = Accesor_Brain::BehaviorManager(m_holder);
	if (behavior_m->Behavior_IsActive())
		current_status = behavior_m->Execute_Current_Behavior(character);
	else
	{
		MyAssert(false, "%s‚É‚¨‚¢‚Äbeahvior‚ªnum_BeahviorˆÈã Œ»İ‚ÌU‚é•‘‚¢ %s", goal_Name,
						behavior_m->Get_Current_Behavior()->behavior_Name);
		return goal_status::failed;
	}
	
	if(current_status  == goal_status::completed ||
		current_status == goal_status::failed)
	{
		goal_status  ret = m_behavior_Director->Direction(m_holder,character);
		return ret;
	}
	
	return goal_status::active;
}

//bool	Goal::Path_Search(Character* character, Vector3& goal_Pos)
//{
//	ProgramSpeedCheck::Begin();
//	Astar_Standard search;
//	Field_Node* goal_Node = navigation_system->Find_Node_Pos(goal_Pos);
//	search.Set_GoalNode(goal_Node);
//	Field_Node* start_Node = navigation_system->Find_Node_Pos(ControllObjFunc::GetPos(character));
//	search.Set_StartNode(start_Node);
//	if (start_Node->index == goal_Node->index)
//	{
//		MyAssert(false, "start‚ÆGoal‚ª“¯‚¶ƒm[ƒh");
//	}
//
//	AI_MESSAGE msg(ai_messagetype::search_path_ok, static_cast<void*>(&search));
//	
//
//	if (!Accesor_Brain::BehaviorManager(m_holder)->
//		Get_Current_Behavior()->HandleMessage(&msg))
//		return false;
//
//	//ƒS[ƒ‹À•W‚à“`‚¦‚é
//	msg.type = ai_messagetype::set_currentGoalPos;
//	msg.extra_Info = &goal_Pos;
//	Accesor_Brain::BehaviorManager(m_holder)->Get_Current_Behavior()->HandleMessage(&msg);
//	field_Debug->Send_Goal(character->GetId(), search.dicision_Goal);
//	float time = (float)ProgramSpeedCheck::GetTime() * 0.001f;
//	return true;
//}

void	Goal::End(Character* character)
{
	int current_Frame = GAMEMATCHINFO.GetMatchTime();
	if (current_Frame - m_last_EndFrame <= 3)
	{
		ai_message->Send_GoalThinkMessage(m_holder, character,ai_messagetype::lock_goal,nullptr);
	}
	m_last_EndFrame = current_Frame;
}

//bool	Goal::Change_Behavior(const int& next_Behavior,Character* character)
//{
//
//	//if (next_Behavior >= num_Behavior)
//	//	MyAssert(false, "numbeahvior‚±‚¦‚»‚¤A‚â‚Î‚¢");
//	//if (current_Behavior >= 0)
//	//	behavior_List[current_Behavior]->Exit(m_holder, character);
//
//	//current_Behavior = next_Behavior;
//	//behavior_List[current_Behavior]->Enter(m_holder, character);
//	//return true;
//
//}