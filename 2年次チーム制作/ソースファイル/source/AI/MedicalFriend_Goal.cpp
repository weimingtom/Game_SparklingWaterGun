#include	"..\All.h"

Goal_MedicalFriend::Goal_MedicalFriend(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director)
: Goal(brain, "Medical_Friend", evo, goal_tag::medical_friend,use_director)
{
	m_behavior_Director->Set_Parent(this);
}

bool	Goal_MedicalFriend::Path_Search(Character* character, const bool& research, Vector3& goal_Pos)
{
	return false;
}

bool	Goal_MedicalFriend::Active(Character* character)
{
	if (!Accesor_Brain::Evalutor(m_holder)->Direction_MedicalFriend(m_holder, character))
		return false;

	if (m_behavior_Director->Direction(m_holder, character, true) == goal_status::failed)
	{
		MyAssert(false, "����͂�΂�");
		return false;
	}
	WriteSpace* myspace = npc_commander->Get_WriteData(m_holder, ControllObjFunc::GetMatchData(character)->team_type);
	if (!myspace)
	{
		MyAssert(false, "����͂�΂�");
		return true;
	}
	myspace->m_message_priority = message_priority::rescue_friend;
	myspace->m_rescue_Friend = Accesor_Brain::Current_Target(m_holder);
	return true;
}

bool	Goal_MedicalFriend::Target_IsFine(Character_Record* record_ptr)
{
	//�e�X�g
	if (record_ptr->m_hp >= 85.0f)
		return true;
	return false;
}

goal_status	Goal_MedicalFriend::Process( Character* character)
{
	//�^�[�Q�b�g�����܂�Ȃ��ꍇ�͎��s(���ꂢ��Ȃ����ۂ�)
	if (Accesor_Brain::Current_Target(m_holder) == game_id::id_error)
	{
		MemoryFunction::Remove_PlannningMemory(m_holder, Accesor_Brain::Current_Target(m_holder));
		return goal_status::failed;
	}

	float reliability;
	Character_Record* target_Record = m_holder->
		Get_CharacterMemory(Accesor_Brain::Current_Target(m_holder), &reliability);
	if (!target_Record)
	{
		return goal_status::failed;
	}
	//�^�[�Q�b�g�����C�ɂȂ�����
	if (Target_IsFine(target_Record))
	{
		m_holder->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(m_holder), true);
		return goal_status::completed;
	}
		
	goal_status behavior_status = Behavior(character);
	if (behavior_status == goal_status::failed)
	{
		//�ꎞ�L���������
		if (target_Record->m_update)
		{
			MemoryFunction::Remove_PlannningMemory(m_holder, Accesor_Brain::Current_Target(m_holder));
		}
		else//���E�L���̍X�V�t���O��false�������ꍇ���łɎ���ł���\��������̂œo�^
		{
			m_holder->GetMemory().m_goal_Result->Register( Accesor_Brain::Current_Target(m_holder),false);
		} 
	}
	return behavior_status;
}

void	Goal_MedicalFriend::Completed(Character* character)
{
	npc_commander->Clear_MyWriteSpece(m_holder,
		ControllObjFunc::GetMatchData(character)->team_type);
}

void	Goal_MedicalFriend::Failed(Character* character)
{
	npc_commander->Clear_MyWriteSpece(m_holder,
		ControllObjFunc::GetMatchData(character)->team_type);
}

goal_status Behavior_Director_MedicalFriend::Activate(NPC_Brain* brain, Character* character)
{
	float target_dist;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist))
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_stand,
			m_holder, behavior_processfunc::shotweapon::Medical_Character);
		return goal_status::active;
	}
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	Vector3 target_Pos;
	//�^�[�Q�b�g�̎��E����
	if (Accesor_Brain::Evalutor(brain)->Target_InSight())
	{
		float reliability;
		const Character_Record* target_Record = brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain),
			&reliability);
		if (!target_Record)
		{
			MyAssert(target_Record, "�^�[�Q�b�g�̋L�����Ȃ��A��΂�");
			return goal_status::failed;
		}
		target_Pos = target_Record->m_sensed_Pos;
	}
	else//���E�O�̏ꍇ�͈ʒu��\�z����
	{
		float reliability;
		if (!Accesor_Brain::Evalutor(brain)->Expectation_TargetPos_Character(brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain), &reliability), &target_Pos, reliability))
		{
#if ASSERT
			MyAssert(false, "attackEnemy::active�Ń^�[�Q�b�g�̋L�����������Ă��܂�");
#endif
			return goal_status::failed;
		}
	}
	if (navigation_system->Duplication_Check(ControllObjFunc::GetPos(character), target_Pos))
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_point, m_holder,
			behavior_processfunc::move_point::Range_of_Target);
		ai_message->Send_BehaviorMessage(brain, character, ai_messagetype::set_currentGoalPos, &target_Pos);
		return goal_status::active;
	}
	Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_edge, m_holder,
		behavior_processfunc::move_point::Range_of_Target);
	Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(), target_Pos,
		Accesor_Brain::Param(brain).m_interval_characterTarget_SearchPath);
	
	return goal_status::active;
}

goal_status  Behavior_Director_MedicalFriend::MoveEdge(NPC_Brain* brain, Character* character)
{
	float target_dist;
	bool line_of_fire;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist,&line_of_fire) &&
		Accesor_Brain::Evalutor(brain)->Target_InSight())
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_stand, m_holder,
			behavior_processfunc::shotweapon::Medical_Character);
		return goal_status::active;
	}
	float reliability;
	Character_Record* target_record = brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain), &reliability);
	if (reliability <= 0.85f || !line_of_fire)
	{
		brain->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(brain), false);
	}
	return goal_status::failed;
}

goal_status  Behavior_Director_MedicalFriend::MovePoint(NPC_Brain* brain, Character* character)
{
	float target_dist;
	bool line_of_fire;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist, &line_of_fire) &&
		Accesor_Brain::Evalutor(brain)->Target_InSight())
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_stand, m_holder,
			behavior_processfunc::shotweapon::Medical_Character);
		return goal_status::active;
	}
	//�L���̐M���x�����l�ȉ��̏ꍇ�͓o�^
	float reliability;
	Character_Record* target_record = brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain), &reliability);
	if (reliability <= 0.85f ||!line_of_fire)
	{
		brain->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(brain), false);
	}
	return goal_status::failed;
}

goal_status  Behavior_Director_MedicalFriend::ShotWeapon(NPC_Brain* brain, Character* character)
{
	//Behavior�����������̂����s�����̂����擾
	goal_status beahvior_Status = Accesor_Brain::BehaviorManager(brain)->Get_BehaviorStatus();
	if (beahvior_Status == goal_status::completed)
		return beahvior_Status;

	if (beahvior_Status != goal_status::failed)
		MyAssert(false, "failed��completed�ȊO�ł����ɂ���̂͂�������");

	Vector3 my_Pos = ControllObjFunc::GetPos(character);

	//�^�[�Q�b�g��ǂ�����߂邩�ǂ��������肷��
	//���E����O��Ď��s�����ꍇ
	if (!Accesor_Brain::Evalutor(brain)->Target_InSight())
	{
		float target_dist;
		Vector3 target_Pos;
		if (!Accesor_Brain::Evalutor(brain)->Target_at_LoseSight(character, &target_Pos, &target_dist))
		{
			return goal_status::failed;
		}
		else
		{
			//target_pos��mypos��Node_index�������ꍇ��MoveEdge
			if (navigation_system->Duplication_Check(my_Pos, target_Pos))
			{
				Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_point,
					m_holder, behavior_processfunc::move_point::Range_of_Target);
				ai_message->Send_BehaviorMessage(brain, character, ai_messagetype::set_currentGoalPos,
					&target_Pos);
			}
			else
			{
				Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_edge,
					m_holder, behavior_processfunc::move_edge::Range_of_Target);
				Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(), target_Pos,
					Accesor_Brain::Param(brain).m_interval_characterTarget_SearchPath);
				//ai_message->Demand_Search_Path(brain, character, target_Pos, false);
			}
			return goal_status::active;
		}
	}
	//�˒�����O��Ď��s�����ꍇ
	float target_dist;
	if (!Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist))
	{
		int current_charge = (int)((float)ChrFunc::GetUI(character)->GetColaGaugePower() * 100);
		target_dist = sqrtf(target_dist);
		float desiraliblity = Accesor_Brain::FuzzyTool(brain)->Calculate_Sight_Target_Character(target_dist, (float)current_charge);
		if (desiraliblity > 60.0f)
		{
			float reliability;
			Character_Record* record = brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain), &reliability);
			MyAssert(record, "�����ŗ�����̂͂�΂�");
			if (navigation_system->Duplication_Check(my_Pos, record->m_sensed_Pos))
			{
				return goal_status::failed;
			}
			else
			{
				Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_edge,
					m_holder, behavior_processfunc::move_edge::Range_of_Target);
				Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(), record->m_sensed_Pos,
					Accesor_Brain::Param(brain).m_interval_characterTarget_SearchPath);
			//	ai_message->Demand_Search_Path(brain, character, record->m_sensed_Pos, false);
			}
			return goal_status::active;
		}
		else
			return goal_status::failed;
	}
	//MyAssert(false, "�����܂ł͂��Ȃ��͂��Ȃ̂�");
	//�����炭�L�����N�^�[�����S���Ă���
	return goal_status::failed;
}

goal_status	Behavior_Director_MedicalFriend::Direction(NPC_Brain* brain, Character* character, bool init)
{
	if (init)
		return Activate(brain, character);

	goal_status ret = goal_status::error;
	switch (Accesor_Brain::BehaviorManager(brain)->Get_Current_Tag())
	{
	case behavior_tag::move_edge:
		ret = MoveEdge(brain, character);
		break;

	case	behavior_tag::move_point:
		ret = MovePoint(brain, character);
		break;

	case behavior_tag::shot_weapon_stand:
		ret = ShotWeapon(brain, character);
		break;
	}
	return ret;

	return goal_status::completed;
}

