#include	"..\All.h"

#pragma region Goal

Goal_FollowFriend::Goal_FollowFriend(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director)
: Goal(brain, "FollowFriend", evo, goal_tag::follow_friend, use_director)
{
	m_behavior_Director->Set_Parent(this);
}

bool	Goal_FollowFriend::Active(Character* character)
{
	Censor_FriendMessage* message_censor = Accesor_Brain::Censor(m_holder)->m_friend_Message;
	//処理したメッセージがFollowMeではない場合失敗となる
	if (message_censor->GetFriendMessageType() != FriendMessageIcon::MsgType::_COME_ON)
		return false;
	//ターゲットを設定する
	Accesor_Brain::Set_Current_Target(m_holder, message_censor->Get_SenderID());
	if (m_behavior_Director->Direction(m_holder, character, true) == goal_status::failed)
	{
		return false;
	}
	//ついていく時間を決定する
	m_follow_Time = base_FollowTime + Rand::Equal_DistanceNum(-adjust_Frame, adjust_Frame);
	WriteSpace* myspace = npc_commander->Get_WriteData(m_holder, ControllObjFunc::GetMatchData(character)->team_type);
	if (!myspace)
	{
		MyAssert(false, "これはやばい");
		return true;
	}
	myspace->m_message_priority = message_priority::follow_friend;
	myspace->m_follow_Friend = Accesor_Brain::Current_Target(m_holder);

	return true;
}

goal_status	Goal_FollowFriend::Process(Character* character)
{
	m_follow_Time--;
	if (m_follow_Time <= 0)
	{
		status = goal_status::completed;
		return status;
	}

	//ターゲットの生存チェック
	float reliability;
	Character_Record* target_record = m_holder->GetMemory().Get_CharacterMemory
		(Accesor_Brain::Current_Target(m_holder), Accesor_Brain::Param(m_holder).record_character_ReliabilityTime, &reliability);
	if (!target_record)
		return goal_status::failed;

	if (target_record->m_status == arive_status::dying ||
		target_record->m_status == arive_status::dead)
		return goal_status::failed;

	//ターゲットに近づきすぎた場合は待機
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	Vector3 target_me = target_record->m_sensed_Pos - my_Pos;
	float dist_sq = target_me.LengthSq();
	if (dist_sq < 4.0f)
		return goal_status::active;

	status = Behavior(character);
	return status;

}

void	Goal_FollowFriend::Completed(Character* character)
{
	npc_commander->Clear_MyWriteSpece(m_holder,
		ControllObjFunc::GetMatchData(character)->team_type);
}

void	Goal_FollowFriend::Failed(Character* character)
{
	npc_commander->Clear_MyWriteSpece(m_holder,
		ControllObjFunc::GetMatchData(character)->team_type);
}

bool	Goal_FollowFriend::HandleMessage(AI_MESSAGE* msg)
{
	return false;
}


#pragma endregion

#pragma region Director

goal_status Behavior_Director_FollowFriend::Activate(NPC_Brain*brain, Character* character)
{
	//記憶取得
	float reliability;
	Character_Record* target_record = brain->GetMemory().Get_CharacterMemory(Accesor_Brain::Current_Target(brain),
		Accesor_Brain::Param(brain).record_character_ReliabilityTime, &reliability);
	
	if (reliability <= 0.5f || !target_record)
		return goal_status::failed;
	
	//ノードが被るかチェック
	Vector3 cur_Pos = ControllObjFunc::GetPos(character);
	Vector3 target_Pos = target_record->m_sensed_Pos;
	if (navigation_system->Duplication_Check(cur_Pos, target_record->m_sensed_Pos))
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_point,
			m_holder, behavior_processfunc::move_point::Follow_Target);
		
		ai_message->Send_BehaviorMessage(brain, character, 
							ai_messagetype::set_currentGoalPos, &target_Pos);
		return goal_status::active;
	}
	//座標をノードに変換
	Field_Node* cur_Node = navigation_system->Find_Node_Pos(cur_Pos);
	Field_Node* target_Node = navigation_system->Find_Node_Pos(target_record->m_sensed_Pos);
	if (!cur_Node || !target_Node)
		return goal_status::failed;
	
	Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_edge, m_holder,
		behavior_processfunc::move_edge::Follow_Target);
	Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(), target_Pos,
		Accesor_Brain::Param(brain).m_interval_characterTarget_SearchPath);

	return goal_status::active;

}

goal_status Behavior_Director_FollowFriend::Direction(NPC_Brain* brain, Character* character, bool	init)
{
	if (init)
		return Activate(brain, character);

	goal_status ret = goal_status::failed;
	switch (Accesor_Brain::BehaviorManager(brain)->Get_Current_Tag())
	{
	case behavior_tag::move_edge :

		break;
		
	case behavior_tag::move_point :

		break;
	}

	return ret;
}



#pragma endregion
