#include	"..\All.h"

#pragma region Goal 

Goal_GetAdvertisementPoint::Goal_GetAdvertisementPoint(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director)
: Goal(brain, "GetAdvertisementPoint", evo, goal_tag::get_advertsementpoint,use_director)
{
	m_behavior_Director->Set_Parent(this);
}

bool	Goal_GetAdvertisementPoint::Active(Character* character)
{
	//current_Behavior = b_num::none;
	//ターゲットなるオブジェクトを設定する
	//ENTITY_ID finalGoal_TargetID = Accesor_Brain::Final_GoalEvalutor(m_holder)->Get_TargetID();
	//if (finalGoal_TargetID != ENTITY_ID::id_error)
	//{
	//	Accesor_Brain::Set_Current_Target(m_holder, finalGoal_TargetID);
	//}
	//else
	//{

	//
	if (!Accesor_Brain::Evalutor(m_holder)->Direction_AdvertisementPointObject(character))
	{
		m_holder->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(m_holder),false);
		return false;
	}

	//MyAssert(Accesor_Brain::Evalutor(m_holder)->Direction_AdvertisementPointObject(character), "Goal_GetAdvertisementPoint::Activeでターゲットオブジェクトが決まりませんでした");
	//}

	if (IS_CONTROLLOBJ_ID(Accesor_Brain::Current_Target(m_holder)))
	{
		return false;
	}
	
	MapObjectInterface* target = dynamic_cast<MapObjectInterface*>(IDMANAGER.GetPointer(Accesor_Brain::Current_Target(m_holder)));
	if (!target)
		MyAssert(false, "これはやばい");
	if (m_behavior_Director->Direction(m_holder, character, true) == goal_status::failed)
	{
		//MyAssert(false, "ありえないことがおきた");
		return false;
	}
	return true;
}

goal_status	Goal_GetAdvertisementPoint::Process(Character* character)
{
	//ターゲットの無効化チェック
	ShareRatioObjectBase* current_target = dynamic_cast<ShareRatioObjectBase*>
		(IDMANAGER.GetPointer(Accesor_Brain::Current_Target(m_holder)));
	if (!current_target)
	{
		MessageBoxPlus("Goal_GetAdvertisementPoint::Processでターゲットポインタ紛失確認");
		return goal_status::failed;
	}

	//視界内にターゲットがいるときはshareObjectが味方のShareが一定以上にならないかを評価する
	behavior_tag current_tag = Accesor_Brain::BehaviorManager(m_holder)->Get_Current_Tag();
	if (Accesor_Brain::Evalutor(m_holder)->Target_InSight() && 
		current_tag != behavior_tag::shot_weapon_stand)
	{
		if (Evalute_SharePoint_Current_Target(character))
		{
			m_holder->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(m_holder), false);
			return goal_status::failed;
		}
			
	}

	//トラックに張り付いている場合画面外に出たことを考慮
	float reliability;
	MapObject_Record*	target_memory = m_holder->Get_MapObjectMemory(Accesor_Brain::Current_Target(m_holder), &reliability);
	if (!target_memory)
		return goal_status::failed;
	Field_Node* target_Node = navigation_system->Find_Node_Pos(target_memory->m_sensed_Pos);
	if (!target_Node)
		return goal_status::failed;

	goal_status behavior_status = Behavior(character);
	if (behavior_status == goal_status::failed)
		m_holder->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(m_holder), false);


	return behavior_status;
}

void	Goal_GetAdvertisementPoint::Completed(Character* character)
{
	
}

void	Goal_GetAdvertisementPoint::Failed(Character* character)
{
	
}

bool	Goal_GetAdvertisementPoint::HandleMessage(AI_MESSAGE* msg)
{
	R_Targetwrapping* work = nullptr;
	switch (msg->type)
	{
	case ai_messagetype::evalute_currentGoalPos :
		work = (R_Targetwrapping*)(msg->extra_Info);
		return Evalute_SharePoint_Current_Target(work->search_Character);
		break;
		
	default :
		break;
	}
	return false;
}

bool Goal_GetAdvertisementPoint::Evalute_SharePoint_Current_Target(Character* character)
{
	//現在のターゲットを取得
	ShareRatioObjectBase* current_Traget = dynamic_cast<ShareRatioObjectBase*>
		(IDMANAGER.GetPointer((Accesor_Brain::Current_Target(m_holder))));
	if (!current_Traget)
	{
		MyAssert(false,"ターゲット紛失");
		return false;
	}
	//現在のレート取得
	TEAM_TYPE my_team = ControllObjFunc::GetMatchData(character)->team_type;
	TEAM_TYPE share_teamType = current_Traget->Get_ShareParam().team_type;
	float target_SharePower = current_Traget->Get_ShareParam().share_value / current_Traget->Get_ShareParam().max_share_value;
	
	if (my_team == share_teamType && 
		target_SharePower >= Accesor_Brain::Param(m_holder).m_feel_high_SharePoint)
		return true;

	return false;
}

#pragma endregion

#pragma region Director

goal_status Behavior_Director_GetAdvertisementPoint::Activate(NPC_Brain* brain, Character* character)
{
	MapObjectInterface* target = dynamic_cast<MapObjectInterface*>(IDMANAGER.GetPointer(Accesor_Brain::Current_Target(brain)));
	if (!target)
	{
		MyAssert(target, "これはやばい");
		brain->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(brain), false);
		return goal_status::failed;
	}

	float target_dist;
	//ShotWeapon
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist))
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_stand,
			m_holder, behavior_processfunc::shotweapon::Start_Up_Object);
		return goal_status::active;
	}

	//現在座標取得
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	Vector3 target_Pos = target->Get_Param().pos;
	//ちょっとだけ前方向に補正
	Matrix mat = target->Get_Matrix();
	Vector3 front(mat._31, mat._32, mat._33);
	front.Normalize();
	target_Pos += front * 1.3f;

	//ノードに変換
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
			m_holder, behavior_processfunc::move_edge::Range_of_Target);
		Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(), target_Pos,
				Accesor_Brain::Param(brain).m_interval_advertisementpoint_SearchPath);
		//ai_message->Demand_Search_Path(brain, character, target_Pos, false);
		return goal_status::active;
	}
	//MovePoint
	Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_point,
		m_holder, behavior_processfunc::move_point::Range_of_Target);
	ai_message->Send_BehaviorMessage(brain, character, ai_messagetype::set_currentGoalPos, &target_Pos);
	return goal_status::active;
}

goal_status Behavior_Director_GetAdvertisementPoint::MoveEdge(NPC_Brain* brain, Character* character)
{
	float ret = goal_status::active;
	float target_dist;
	if (!Accesor_Brain::Evalutor(brain)->Target_InSight())
	{
		float reliabiliy;
		MapObject_Record* target_record = brain->Get_MapObjectMemory(
			Accesor_Brain::Current_Target(brain), &reliabiliy);
		if (!target_record)
			return goal_status::failed;
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::look_back,
			m_holder, nullptr);
		//振り向く方向設定
		Vector3 my_Pos = ControllObjFunc::GetPos(character);
		Vector3 target_vec = target_record->m_sensed_Pos - my_Pos;
		target_vec.Normalize();
		ai_message->Send_BehaviorMessage(brain, character, ai_messagetype::set_LookVec, &target_vec);
		return goal_status::active;
	}

	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist))
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_stand,
			m_holder, behavior_processfunc::shotweapon::Start_Up_Object);
	
		return goal_status::active;
	}
	return goal_status::failed;
}

goal_status Behavior_Director_GetAdvertisementPoint::MovePoint(NPC_Brain* brain, Character* character)
{
	float ret = goal_status::active;
	float target_dist;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist))
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_stand,
			m_holder, behavior_processfunc::shotweapon::Start_Up_Object);
		return goal_status::active;
	}
	return goal_status::failed;
}

goal_status Behavior_Director_GetAdvertisementPoint::ShotWeapon(NPC_Brain* brain, Character* character)
{
	float target_dist;
	//射程から外れていた場合は失敗
	if (!Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist))
	{
		return goal_status::failed;
	}
	//次のターゲットを決定する
	if (Accesor_Brain::Evalutor(brain)->Direction_AdvertisementPointObject(character))
	{
		ShareRatioObjectBase* next_target = dynamic_cast<ShareRatioObjectBase*>
			(IDMANAGER.GetPointer(Accesor_Brain::Current_Target(brain)));
		MyAssert(next_target, "オブジェクトがない、やばい");
		Vector3 goal_pos = next_target->Get_Param().pos;
		//ちょっとだけ前方向に補正
		Matrix mat = next_target->Get_Matrix();
		Vector3 front(mat._31, mat._32, mat._33);
		front.Normalize();
		goal_pos += front * 1.3f;
		Field_Node* start = navigation_system->Find_Node_Pos(ControllObjFunc::GetPos(character));
		Field_Node* goal = navigation_system->Find_Node_Pos(goal_pos);
		if (!start || !goal)
		{
			//MyAssert(false, "スタートとゴールがない、やばい");
			brain->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(brain), false);
			return goal_status::failed;
		}
		if (start->index != goal->index)
		{
			Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_edge,
				m_holder, behavior_processfunc::move_edge::Range_of_Target);
			Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(), goal_pos,
				Accesor_Brain::Param(brain).m_interval_advertisementpoint_SearchPath);
			return goal_status::active;
		}
		else
		{
			Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_point,
				m_holder, behavior_processfunc::move_point::Range_of_Target);
			//	TOCONTROLL(object)->Change_Behavior(b_num::move_point, character);
			ai_message->Send_BehaviorMessage(brain, character, ai_messagetype::set_currentGoalPos, &goal_pos);
			return goal_status::active;
		}
	}
	return goal_status::failed;
}

goal_status Behavior_Director_GetAdvertisementPoint::LookBack(NPC_Brain* brain, Character* character)
{
	float target_dist;
	bool line_of_fire, in_fov;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist, &line_of_fire, &in_fov))
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_stand,
			m_holder, behavior_processfunc::shotweapon::Start_Up_Object);
		return goal_status::active;
	}
	return goal_status::failed;
}

goal_status Behavior_Director_GetAdvertisementPoint::Direction(NPC_Brain* brain, Character* character, bool init)
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

	case behavior_tag::shot_weapon_stand:
		ret = ShotWeapon(brain, character);
		break;

	case behavior_tag::look_back :
		ret = LookBack(brain, character);
		break;
		
	}
	return ret;
}
#pragma endregion
