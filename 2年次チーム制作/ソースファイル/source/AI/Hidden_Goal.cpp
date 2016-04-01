#include	"..\All.h"

Goal_Hidden::Goal_Hidden(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director)
: Goal(brain, "Hidden", evo, goal_tag::hidden,use_director)
{
	m_behavior_Director->Set_Parent(this);
}

bool	Goal_Hidden::Active(Character* character)
{
	//“¦‚°‚é‘ÎÛ‚ðÝ’è‚·‚é
	m_escape_Target = Accesor_Brain::Symbol(m_holder).m_is_Danger;
	if (m_escape_Target == game_id::id_error)
	{
		MyAssert(false, "Goal_Hidden‚Åƒ^[ƒQƒbƒgƒGƒ‰[");
		return false;
	}
	float reliability;
	Character_Record* target_Record = m_holder->Get_CharacterMemory(m_escape_Target,&reliability);
	if (!target_Record && !m_holder->GetMemory().Is_Record_Threat_OutSight(m_escape_Target))
	{
#if NPC_ASSART
		MyAssert(false, "Goal_Hidden‚Åƒ^[ƒQƒbƒg‚Ì‹L‰¯‚ª‚ ‚è‚Ü‚¹‚ñ");
#endif
		m_holder->GetMemory().m_goal_Result->Register(m_escape_Target, false);
		return false;
	}
	if(m_behavior_Director->Direction(m_holder,character,true) == goal_status::failed)
		return false;

	return true;
}

goal_status	Goal_Hidden::Process( Character* character)
{
	goal_status status = Behavior(character);

	return status;
}

void	Goal_Hidden::Completed( Character* character)
{
	End(character);
}

void	Goal_Hidden::Failed(Character* character)
{
	End(character);
}

bool	Goal_Hidden::Evalute_Current_GoalPos()
{
	return true;
}

bool Behavior_Director_Hidden::Direction_Escape_Vec(NPC_Brain* brain, Vector3* out, 
																					const ENTITY_ID& escape_Target)
{
	float reliability;
	Character_Record* target_Record = brain->Get_CharacterMemory(escape_Target, &reliability);
	if (target_Record)
	{
		*out = target_Record->m_action.m_vec;
		return true;
	}

	Threat_OutSight*const outsight_record = brain->GetMemory().Get_Threat_OutSight(escape_Target);
	if (!outsight_record)
	{
		MyAssert(false, "‚±‚ê‚Í‚â‚Î‚¢");
		return false;
	}
	*out = outsight_record->m_vec;
	return true;
}

Field_Node* Behavior_Director_Hidden::Direction_Esacpe_Point(NPC_Brain* brain, Character* character,
																							const ENTITY_ID& escape_Target)
{
	Field_Node* ret = nullptr;
	//float reliability;
	//Character_Record* target_Record = m_holder->Get_CharacterMemory(m_escape_Target, &reliability);
	//if (!target_Record)
	//	return nullptr;

	//“G‚ªUŒ‚‚µ‚Ä‚«‚Ä‚¢‚é‚©‚Ç‚¤‚©
	//expectation_act target_action = target_Record->m_action.m_tag;
	Vector3 priority_EscapeVec;
	if (!Direction_Escape_Vec(brain, &priority_EscapeVec, escape_Target))
	{
		return nullptr;
	}
	//if (target_action == expectation_act::attack_me)
	//	priority_EscapeVec = target_Record->m_action.m_vec;
	//else
	//	priority_EscapeVec = target_Record->m_sensed_Angle;
	priority_EscapeVec.y = .0f;

	priority_EscapeVec.Normalize();
	//•ûŠp’l‚É•ÏŠ·
	int start_direction = 0;
	int num_direcion = enum_cast(direction::num_direction);
	float most_near_cos = FLT_MAX;
	for (int i = 0; i < num_direcion; i++)
	{
		direction dd = (direction)i;
		const Vector3 d = navigation_system->Get_Direction(dd);
		float cos = Vector3Dot(d, priority_EscapeVec);
		if (cos < .0f)
			continue;
		cos = 1.0f - cos;
		if (most_near_cos > cos)
		{
			start_direction = i;
			most_near_cos = cos;
		}
	}
	//•ûŠp‚©‚ç‘Þ”ðæƒm[ƒhcontainer‚ðE‚Á‚Ä‚­‚é9
	Vector3 current_Pos = ControllObjFunc::GetPos(character);
	Field_Node* cur_Node = navigation_system->Find_Node_Pos(current_Pos);
	if (!cur_Node)
	{
		//Œ´ˆö‚ª‚í‚©‚ç‚È‚¢‚Ì‚Å‚Æ‚è‚ ‚¦‚¸
		//MyAssert(false, "ƒm[ƒhE‚Á‚Ä‚±‚ê‚È‚¢");
		return nullptr;
	}
	const WayPointInformation* node_Info = navigation_system->Get_NodeInformation(cur_Node->index);
	const std::vector<Field_Node*>* hidden_Pointptr = nullptr;
	int count = num_direcion;
	int d = start_direction;
	for (int i = 0; i < num_direcion; i++)
	{
		if (i % 2 == 0)
			d = (d + (num_direcion - count)) % num_direcion;
		else
			d = (d + count) % num_direcion;

		if (!node_Info->m_hidden_Point[d].empty())
		{
			hidden_Pointptr = &node_Info->m_hidden_Point[d];
			break;
		}
		count--;
	}
	//Vector3 escape_pos;
	if (!hidden_Pointptr)
	{
		MyAssert(false, "%d”Ônode‚Ì‘Þ”ðêŠ‚ª‘¶Ý‚µ‚È‚¢", cur_Node->index);
		Vector3 p = current_Pos + priority_EscapeVec * 20.0f;
		ret = navigation_system->Find_Node_Pos(p);
		if (!ret)
		{
			//escape_pos = p;
			return ret;
		}
		//escape_pos = ret->pos;
		return ret;
	}
	//ˆê”Ô‹ß‚¢‚Æ‚±‚ë‚É“¦‚°‚é
	float min_dist = FLT_MAX;
	for (auto it : *hidden_Pointptr)
	{
		float dist = (current_Pos - it->pos).LengthSq();
		if (dist < min_dist)
			ret = it;
	}
	//escape_pos = ret->pos;
	return ret;
}


goal_status Behavior_Director_Hidden::Activate(NPC_Brain*brain, Character* character)
{
	Goal_Hidden* parent_ptr = static_cast<Goal_Hidden*>(m_holder);
	ENTITY_ID escape_Target = parent_ptr->Get_EscapeTarget();
	Vector3 current_Pos = ControllObjFunc::GetPos(character);
	Field_Node* escape_Node = Direction_Esacpe_Point(brain, character, escape_Target);
	Field_Node* current_Node = navigation_system->Find_Node_Pos(current_Pos);
	if (!escape_Node || !current_Node)
	{
#if NPC_ASSART
		MyAssert(false, "‚Ç‚Á‚¿‚©‚Ìƒm[ƒh‚ª‚È‚¢");
#endif
		brain->GetMemory().m_goal_Result->Register(escape_Target, false);
		return goal_status::failed;
	}
	//“¦‚°‚é’n“_‚ð‘—‚é
	parent_ptr->Set_EscapePos(escape_Node->pos);
	if (escape_Node->index == current_Node->index)
	{
		brain->GetMemory().m_goal_Result->Register(escape_Target, false);
		return goal_status::failed;
	}
	Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_edge, m_holder,
		behavior_processfunc::move_edge::Arrival_GoalPoint);
	Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(),
															escape_Node->pos);
	//ai_message->Demand_Search_Path(brain, character, escape_Node->pos, false);
	return goal_status::active;
}

goal_status Behavior_Director_Hidden::Direction(NPC_Brain* brain, Character* character, bool	init)
{
	if (init)
		return Activate(brain, character);

	return goal_status::completed;
}
