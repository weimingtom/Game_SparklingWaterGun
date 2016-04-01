#include	"..\All.h"

#pragma region Goal

Goal_WanderField::Goal_WanderField(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director)
: Goal(brain, "WanderField", evo, goal_tag::wander_field,use_director)
{
	m_behavior_Director->Set_Parent(this);
}

bool	Goal_WanderField::Active(Character* character)
{
	//current_Behavior = b_num::none;
	if (m_behavior_Director->Direction(m_holder, character, true) == goal_status::failed)
	{
#if NPC_ASSART
		MyAssert(false, "ありえないことがおきた");
#endif
		
	}
	return true;
}



goal_status	Goal_WanderField::Process(Character* character)
{
	//ターゲットの無効化チェック
	ShareRatioObjectBase* current_target = dynamic_cast<ShareRatioObjectBase*>
		(IDMANAGER.GetPointer(Accesor_Brain::Current_Target(m_holder)));
	if (!current_target)
	{
		MessageBoxPlus("Goal_GetAdvertisementPoint::Processでターゲットポインタ紛失確認");
		return goal_status::failed;
	}

	goal_status status = Behavior(character);
	
	return status;
}

void	Goal_WanderField::Completed(Character* character)
{

}

void	Goal_WanderField::Failed(Character* character)
{
	
}

bool		Goal_WanderField::HandleMessage(AI_MESSAGE* msg)
{
	return false;
}

#pragma endregion


goal_status	Behavior_Director_WanderField::Direction(NPC_Brain* brain, Character* character, bool init)
{
	MyAssert(Direction_Target(brain,character), "ありえないことがおきた");
	float reliability;
	MapObject_Record* record = brain->Get_MapObjectMemory(Accesor_Brain::Current_Target(brain), &reliability);
	MyAssert(record, "ありえないことがおきた");
	Vector3 target_Pos = record->m_sensed_Pos;
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	if (navigation_system->Duplication_Check(my_Pos, target_Pos))
	{
		//とりあえず空チェック
		if (!navigation_system->Find_Node_Pos(target_Pos))
		{
			brain->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(brain),false);
			return goal_status::failed;
		}
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_point, m_holder,
			behavior_processfunc::move_point::Arrival_GoalPoint);
		ai_message->Send_BehaviorMessage(brain, character, ai_messagetype::set_currentGoalPos,
			&target_Pos);
	}
	else
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_edge, m_holder,
			behavior_processfunc::move_point::Arrival_GoalPoint);
	
		Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(),target_Pos);
	}
	return goal_status::active;
}

bool	Behavior_Director_WanderField::Direction_Target(NPC_Brain* brain,Character* character)
{
	//現在のターゲット取得
	ENTITY_ID current_Target = Accesor_Brain::Current_Target(brain);
	ENTITY_ID next_Target = ENTITY_ID::id_error;
	float most_minDist = FLT_MAX;
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	const std::map<ENTITY_ID, MapObject_Record*>* const memory_ptr =
		&brain->GetMemory().m_mapobject_Record;
	//ターゲットをランダムに決める
	std::vector<ENTITY_ID> candidate_vector;
	candidate_vector.reserve(20);

	for (auto& cur_memory : *memory_ptr)
	{
		if (cur_memory.second->m_id == current_Target)
			continue;
		//ノードに変換
		Field_Node*node = navigation_system->Find_Node_Pos(cur_memory.second->m_sensed_Pos);
		if (!node)
			continue;

		if (cur_memory.second->m_object_type != mapobjcet_type::poster)
			continue;
		candidate_vector.emplace_back(cur_memory.second->m_id);
		// //視界外の場合は距離を引き延ばして補正をかける
		// float bias = (cur_memory.second->m_visibilily) ? 1.0f : 2.0f;
		// float dist = cur_memory.second->m_sensed_Pos - my_Pos.LengthSq();
		// dist *= bias;
		// if (dist < most_minDist)
		// {
		//	 next_Target = cur_memory.second->m_id;
		//	 most_minDist = dist;
		// }
		//}
		//if (next_Target == game_)
	}
	int next_No = Rand::Equal_DistanceNum(0, (candidate_vector.size() - 1));
	next_Target = candidate_vector[next_No];
	if (next_Target != ENTITY_ID::id_error)
	{
		Accesor_Brain::Set_Current_Target(brain, next_Target);
		//brain->Set_CurrentTarget(next_Target);
		return true;
	}
	return false;
}
//#pragma region Evalutor
//#define TOCONTROLL(ptr) ((Evalutor_Beahvior_Goal_WanderField*)(ptr))
//
//goal_status Evalutor_Beahvior_Goal_WanderField::MoveEdge
//(NPC_Brain* brain, Character* character, Goal_WanderField* object)
//{
//
//	return goal_status::failed;
//}
//
//goal_status Evalutor_Beahvior_Goal_WanderField::Move_Point
//(NPC_Brain* brain, Character* character, Goal_WanderField* object)
//{
//
//	return goal_status::failed;
//}
//
//
//goal_status Evalutor_Beahvior_Goal_WanderField::None
//(NPC_Brain* brain, Character* character, Goal_WanderField* object)
//{
//
//	return goal_status::failed;
//}
//
//#pragma endregion
