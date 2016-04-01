#include	"..\All.h"

#pragma region move_edge

goal_status behavior_processfunc::move_edge::Arrival_GoalPoint
(NPC_Brain* brain, Character* character)
{
	return goal_status::active;
}

goal_status behavior_processfunc::move_edge::Range_of_Target
(NPC_Brain* brain, Character* character)
{
	//�^�[�Q�b�g���L�����N�^�[�̏ꍇ����ł��܂����Ƃ��̎����l����(�Ƃ肠���������ɁA���̂����l����)
	if ((IS_CONTROLLOBJ_ID(Accesor_Brain::Current_Target(brain))))
	{
		float releability;
		Character_Record* const target_record = brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain), &releability);
		MyAssert(target_record, "�����Ń^�[�Q�b�g�̕������m�F");
		if (target_record->m_status == arive_status::dead)
			return goal_status::failed;
	}
	float target_dist;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist) &&
		Accesor_Brain::Evalutor(brain)->Target_InSight())
		return goal_status::completed;

	return goal_status::active;
}

goal_status behavior_processfunc::move_edge::Follow_Target(NPC_Brain* brain, Character* character)
{
	float reliability;
	Character_Record* target_record = brain->GetMemory().Get_CharacterMemory(Accesor_Brain::Current_Target(brain),
		Accesor_Brain::Param(brain).record_character_ReliabilityTime, &reliability);
	if (!target_record)
		return goal_status::failed;
	Vector3 cur_Pos = ControllObjFunc::GetPos(character);

	Vector3 vec = target_record->m_sensed_Pos - cur_Pos;
	float dist_sq = vec.LengthSq();
	if (dist_sq <= 3.0f)
		return goal_status::failed;

	vec.Normalize();
	Vector3 target_Velocity = target_record->m_sensed_Velocity;
	target_Velocity.Normalize();
	float dot_cos = Vector3Dot(target_Velocity, vec);
	//if ()

	return goal_status::active;
}

#pragma endregion

#pragma  region move_point

goal_status behavior_processfunc::move_point::Arrival_GoalPoint(NPC_Brain* brain, Character* character)
{
	return goal_status::active;
}

goal_status behavior_processfunc::move_point::Range_of_Target(NPC_Brain* brain, Character* character)
{
	//�^�[�Q�b�g���L�����N�^�[�̏ꍇ����ł��܂����Ƃ��̎����l����(�Ƃ肠���������ɁA���̂����l����)
	if ((IS_CONTROLLOBJ_ID(Accesor_Brain::Current_Target(brain))))
	{
		float releability;
		Character_Record* const target_record = brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain), &releability);
		MyAssert(target_record, "�����Ń^�[�Q�b�g�̕������m�F");
		if (target_record->m_status == arive_status::dead)
			return goal_status::failed;
	}
	float target_dist;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist) &&
		Accesor_Brain::Evalutor(brain)->Target_InSight())
		return goal_status::completed;

	return goal_status::active;
}

goal_status behavior_processfunc::move_point::Follow_Target(NPC_Brain* brain, Character* character)
{
	float reliability;
	Character_Record* target_record = brain->GetMemory().Get_CharacterMemory(Accesor_Brain::Current_Target(brain),
		Accesor_Brain::Param(brain).record_character_ReliabilityTime, &reliability);
	if (!target_record)
		return goal_status::failed;
	Vector3 cur_Pos = ControllObjFunc::GetPos(character);
	
	Vector3 vec = target_record->m_sensed_Pos - cur_Pos;
	float dist_sq = vec.LengthSq();
	if (dist_sq <= 3.0f)
		return goal_status::failed;

	vec.Normalize();
	Vector3 target_Velocity = target_record->m_sensed_Velocity;
	target_Velocity.Normalize();
	float dot_cos = Vector3Dot(target_Velocity, vec);
	//if ()

	return goal_status::active;
}

#pragma endregion

#pragma region Animation

#pragma  region shotweapon

goal_status behavior_processfunc::shotweapon::AttackCharacter(NPC_Brain* brain, Character* character)
{
	float reliability;
	Character_Record* target_record = brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain), &reliability);
	if (!target_record)
	{
		MyAssert(false, "�����ŋL�������m�F");
		return goal_status::failed;
	}
	//���S����
	if (target_record->m_status == arive_status::dead)
		return goal_status::completed;
	
	//��������
	float target_dist;
	if (!Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist))
	{
		CONSOLE_PRINT("�˒����O�ɂȂ���");
		return goal_status::failed;
	}
	
	//���E����
	if (!Accesor_Brain::Evalutor(brain)->Target_InSight())
		return goal_status::failed;

	return goal_status::active;
}

goal_status behavior_processfunc::shotweapon::Medical_Character(NPC_Brain* brain, Character* character)
{
	float reliability;
	Character_Record* target_record = brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain), &reliability);
	if (!target_record)
	{
		MyAssert(false, "�����ŋL�������m�F");
		return goal_status::failed;
	}
	//���S����
	if (target_record->m_status == arive_status::dead)
		return goal_status::failed;

	//��������
	float target_dist;
	if (!Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist))
	{
		CONSOLE_PRINT("�˒����O�ɂȂ���");
		return goal_status::failed;
	}

	//���E����
	if (!Accesor_Brain::Evalutor(brain)->Target_InSight())
		return goal_status::failed;

	//��������
	//�Ƃ肠����
	if (target_record->m_hp >= 80)
		return goal_status::completed;

	return goal_status::active;
}

goal_status behavior_processfunc::shotweapon::Start_Up_Object(NPC_Brain* brain, Character* character)
{
	float target_dist;
	if (!Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist))
		return goal_status::failed;
	
	ShareRatioObjectBase* target_object = dynamic_cast<ShareRatioObjectBase*>
		(IDMANAGER.GetPointer(Accesor_Brain::Current_Target(brain)));
	if (!target_object)
	{
		MyAssert(false, "�^�[�Q�b�g�I�u�W�F�N�g����");
		return goal_status::failed;
	}
	TEAM_TYPE myteam = ControllObjFunc::GetMatchData(character)->team_type;

	//���݂�ShareRate�擾
	const ShareRatioParam& param = target_object->Get_ShareParam();
	float  rate = param.share_value / param.max_share_value;
	if (param.team_type == myteam &&
		rate >= Accesor_Brain::Param(brain).m_feel_high_SharePoint)
	{
		return goal_status::completed;
	}

	return goal_status::active;
}

#pragma endregion

goal_status	behavior_processfunc::RollingAttack(NPC_Brain* brain, Character* character)
{
	if (ChrFunc::GetStateType(character) == CHARACTER_STATE_TYPE::usual_mode)
	{
		return goal_status::completed;
	}
		
	return goal_status::active;
}

goal_status behavior_processfunc::Throw_Grenede(NPC_Brain* brain, Character* character)
{
	return goal_status::completed;
}

#pragma endregion
