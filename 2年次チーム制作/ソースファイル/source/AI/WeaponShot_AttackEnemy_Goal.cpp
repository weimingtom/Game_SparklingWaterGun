#include	"..\All.h"

#pragma region Goal

Goal_WeaponShot_AttackEnemy::Goal_WeaponShot_AttackEnemy
(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director) :
Goal(brain, "ShotWeaponEnemy",evo,goal_tag::shotweapon_enemy,use_director)
{
	m_behavior_Director->Set_Parent(this);
}

bool	Goal_WeaponShot_AttackEnemy::Active(Character* character)
{
	//�^�[�Q�b�g��ݒ肷��
	Accesor_Brain::Evalutor(m_holder)->Decision_TargetEnemy(character);
	
	//�ŏI�S�[���̃^�[�Q�b�g��null��
	Accesor_Brain::Final_GoalEvalutor(m_holder)->Reset_Target();
#if ASSERT
	MyAssert(IS_CONTROLLOBJ_ID(m_holder->Get_CurrentTarget()), "%s�̃^�[�Q�b�g�̃^�C�v�����������ł�", m_holder->character_Name);
#endif
	if (Accesor_Brain::Current_Target(m_holder) == game_id::id_error)
		return false;
	if (m_behavior_Director->Direction(m_holder, character, true) == goal_status::failed)
		return false;

	status = goal_status::active;
	return true;

}

bool	Goal_WeaponShot_AttackEnemy::Target_IsActive()
{
	Character* target = dynamic_cast<Character*>(IDMANAGER.GetPointer(Accesor_Brain::Current_Target(m_holder)));
	if (!target)
		return false;
	return true;
}

goal_status	Goal_WeaponShot_AttackEnemy::Process(Character* character)
{
	//�^�[�Q�b�g�����܂�Ȃ��ꍇ�͎��s(���ꂢ��Ȃ����ۂ�)
	if (Accesor_Brain::Current_Target(m_holder) == game_id::id_error)
	{
		MemoryFunction::Remove_PlannningMemory(m_holder, Accesor_Brain::Current_Target(m_holder));
		return goal_status::failed;
	}
	//�^�[�Q�b�g�G���[�`�F�b�N
	if (!Target_IsActive())
		return goal_status::completed;

	//�^�[�Q�b�g�̂�������Lost�����ꍇ
	float reliability;
	Character_Record* target_Record = m_holder->
																Get_CharacterMemory(Accesor_Brain::Current_Target(m_holder), &reliability);
	if (!target_Record)
	{
		return goal_status::failed;
	}
	goal_status behavior_status = Behavior(character);
	if (behavior_status == goal_status::failed)
	{
		//�ꎞ�L���������
		if (target_Record->m_update)
			MemoryFunction::Remove_PlannningMemory(m_holder, Accesor_Brain::Current_Target(m_holder));
		else
			//�L�����X�V����Ȃ��ꍇ�͎���ł���\��������̂œo�^
			m_holder->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(m_holder),false);
	}

	//�ߐڍU���̎˒��ɓ����Ă����ꍇ�͎��s�Ƃ��ăv�����j���O������
	if (Accesor_Brain::Evalutor(m_holder)->Target_is_NearRange(m_holder, character))
	{
		//�ꎞ�L���������
		MemoryFunction::Remove_PlannningMemory(m_holder, Accesor_Brain::Current_Target(m_holder));
		return goal_status::failed;
	}
	return behavior_status;
}

void	Goal_WeaponShot_AttackEnemy::Completed(Character* character)
{
	//�L���Ƀ^�[�Q�b�g�����S�������Ƃ�m�点��
	ENTITY_ID target = Accesor_Brain::Current_Target(m_holder);
	ai_message->Send_MemoryMessage(m_holder, ai_messagetype::chracter_Die,&target);
	Accesor_Brain::Target_Clear(m_holder);
	status = goal_status::completed;
	End(character);
}

void	Goal_WeaponShot_AttackEnemy::Failed(Character* character)
{
	Accesor_Brain::Target_Clear(m_holder);
	status = goal_status::inactive;
	End(character);
}

bool	Goal_WeaponShot_AttackEnemy::HandleMessage(AI_MESSAGE* msg)
{
	switch (msg->type)
	{
	case ai_messagetype::evalute_currentGoalPos :
		//return Evalute_CurrentGoalPos(static_cast<R_Targetwrapping*>(msg->extra_Info));
	default :
		break;
	}
	return false;
}

#pragma endregion 

#pragma region Director

goal_status Behavior_Director_WeaponShot_AttackEnemy::Activate(NPC_Brain* brain, Character* character)
{
	//��������
	float target_dist;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist))
	{
		//ShotWeapon_Circle
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_circle, m_holder,
			behavior_processfunc::shotweapon::AttackCharacter);
		//TOCONTROLL(object)->Change_Behavior(b_num::shot_weapon, character);
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
		behavior_processfunc::move_edge::Range_of_Target);
	Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(), target_Pos,
		Accesor_Brain::Param(brain).m_interval_characterTarget_SearchPath);

	return goal_status::active;
}

goal_status Behavior_Director_WeaponShot_AttackEnemy::MoveEdge(NPC_Brain* brain, Character* character)
{
	float target_dist;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist) &&
		Accesor_Brain::Evalutor(brain)->Target_InSight())
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_circle, m_holder,
			behavior_processfunc::shotweapon::AttackCharacter);
		//TOCONTROLL(object)->Change_Behavior(b_num::shot_weapon, character);
		return goal_status::active;
	}
	return goal_status::failed;
}

goal_status Behavior_Director_WeaponShot_AttackEnemy::MovePoint(NPC_Brain* brain, Character* character)
{
	float target_dist;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist) &&
		Accesor_Brain::Evalutor(brain)->Target_InSight())
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_circle, m_holder,
			behavior_processfunc::shotweapon::AttackCharacter);
		//TOCONTROLL(object)->Change_Behavior(b_num::shot_weapon, character);
		return goal_status::active;
	}
	return goal_status::failed;
}

goal_status Behavior_Director_WeaponShot_AttackEnemy::ShotWeapon(NPC_Brain* brain, Character* character)
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
				//�����̃m�[�h�̏ꍇ�͎��s�A�ߐڍU���ɂȂ�悤�ɂ�����
				return goal_status::failed;
			}
			else
			{
				Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_edge,
					m_holder, behavior_processfunc::move_edge::Range_of_Target);
				Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(), record->m_sensed_Pos,
					Accesor_Brain::Param(brain).m_interval_characterTarget_SearchPath);
				return goal_status::active;
			}
	
		}
		else
			return goal_status::failed;
	}
	MyAssert(false, "�����܂ł͂��Ȃ��͂��Ȃ̂�");
	return goal_status::failed;
}

goal_status Behavior_Director_WeaponShot_AttackEnemy::Direction(NPC_Brain* brain,
	Character* character, bool init)
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

	case behavior_tag::shot_weapon_circle:
		ret = ShotWeapon(brain, character);
		break;
	}
	return ret;
}

#pragma endregion

