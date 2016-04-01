#include	"..\All.h"
#include	<array>

float	GoalEvalutor_AttackEnemy::Calculate_Score(NPC_Brain* brain, Character* character)
{
	static int	testscore = 120;
	static int game_Frame = 60;	//�t���[�����[�g
	//���ƂȂ�G���擾
	std::vector<Character_Record*> candidate_List;
	MemoryFunction::Get_CharacterRecordList((brain->GetMemory()), (memory_predicate::Candidate_AttackTarget), &candidate_List);

	//���ƂȂ�G�����Ȃ��ꍇ�͂��̃S�[���͓K�p���Ȃ�
	if (candidate_List.empty())
		return .0f;

	int	current_Frame = GAMEMATCHINFO.GetMatchTime();
	int	memory_ReliabilityFrame = Accesor_Brain::Param(brain).record_character_ReliabilityTime;
	float	distBias_Begin = Accesor_Brain::Param(brain).m_knockdownEnemy_distBias_Begin;
	float	distBias_End = Accesor_Brain::Param(brain).m_knockdownEnemy_distBias_End;


	//��ԃ^�[�Q�b�g�Ƃ��čœK�ȓG�̃X�R�A��Ԃ�
	float max_Score = .0f;
	ENTITY_ID  target = game_id::id_error;
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	for (auto candate = candidate_List.begin(); candate != candidate_List.end(); candate++)
	{
		//�L���̐M���l���Ⴂ�ꍇ�́~
		float reliability;
		if (!(*candate)->Can_Use(current_Frame, memory_ReliabilityFrame, &reliability))
			continue;
		if (reliability < 0.7f)
			continue;
		
		//GoalResult�ɋL�^����Ă���ID�͕]�����Ȃ�
		if (brain->GetMemory().m_goal_Result->Is_Register((*candate)->m_id))
			continue;

		//�����`�[���́~
		if (ControllObjFunc::GetMatchData(character)->team_type == (*candate)->m_team_Type)
			continue;
		if (!(*candate)->m_visibilily)
			continue;

		//�˒����ɓ���܂ł̎��Ԃ����Ȃ肨�������ςɌv��
		float dist = ((*candate)->m_sensed_Pos - my_Pos).Length();
		//���Ɏ˒����ɓ����Ă���ꍇ��0�ƂȂ�
		float current_range = ChrFunc::GetNowChargeRange(character);
		float in_RangeDist = Max(.0f, dist - current_range);
	//	in_RangeDist = sqrtf(in_RangeDist);
		float my_MovePower = GetConstChrParam(character)->move_max_xz_speed;

		float	in_RangeTime = (in_RangeDist / my_MovePower) / (float)game_Frame;

		//�G�Ƃ̋����ɉ����ĕ]���l��␳����
		
		float dist_Bias = 1.0f;
		if (dist >= distBias_Begin)
		{
			dist_Bias = dist_Bias - ((dist - distBias_Begin) / distBias_End);
		}

		//���̒e��O�e�������������̎��Ԃ��v��
		//�Ƃ肠�����e�X�g��
		float	down_Time = 120.0f / (float)game_Frame;
		//�X�R�A���[�g�ɕϊ�
		float	total_Time = in_RangeTime + down_Time;
		float score_Rate = (float)testscore / total_Time;
		//���X�N�v�Z
		float	risk = 0.5f;
		float score = score_Rate * m_Bias * (1.0f - risk) * reliability * dist_Bias;
		if (score > max_Score)
		{
			max_Score = score;
			target = (*candate)->m_id;
		}
	}
	//���̃S�[�����K�����ꂽ�ꍇ�̃^�[�Q�b�g��ۑ�
	m_target_ID = target;
	return max_Score;
}

float GoalEvalutor_GetAdvertisementPoint::Calculate_Score(NPC_Brain* brain, Character* character)
{
	static int game_Frame = 60;	//�t���[�����[�g
	static float test_score = 120.0f;

	//���ƂȂ�MapObject�̎擾
	std::vector<MapObject_Record*> candidate_vector;
	MemoryFunction::Get_MapObjectRecordList(brain->GetMemory(),
		memory_predicate::Candidate_AdvertisementPoint,&candidate_vector);

	if (candidate_vector.empty())
		return .0f;

	float max_score = .0f;
	ENTITY_ID target_ID = ENTITY_ID::id_error;
 	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	//���ꂼ��̃I�u�W�F�N�g�ɑ΂��ĕ]���t�����Ă���
	for (auto cur_memory : candidate_vector)
	{
		//GoalResult�ɋL�^����Ă���ID�͕]�����Ȃ�
		if (brain->GetMemory().m_goal_Result->Is_Register(cur_memory->m_id))
			continue;

		//���̃I�u�W�F�N�g�łǂꂾ����share�|�C���g�����邩�v������
		ShareRatioObjectBase* target_Object = dynamic_cast<ShareRatioObjectBase*>
			(IDMANAGER.GetPointer(cur_memory->m_id));
		if (!target_Object)
			continue;

		//�m�[�h�ɕϊ����ĉ�ʊO�Ȃ�e��
		Field_Node*	node = navigation_system->Find_Node_Pos(cur_memory->m_sensed_Pos);
		if (!node)
			continue;

		const ShareRatioParam& target_Param = target_Object->Get_ShareParam();
		TEAM_TYPE my_team = ControllObjFunc::GetMatchData(character)->team_type;
		float current_SharePower = target_Param.share_value / target_Param.max_share_value;
		//�`�[���^�C�v�������ň��l�ȏ��Share�������Ă���I�u�W�F�N�g�͕]�����Ȃ�
		if (target_Param.team_type == my_team &&
			current_SharePower >= Accesor_Brain::Param(brain).m_feel_high_SharePoint)
			continue;
		
		float get_SharePoint = 1.0f - current_SharePower;
		
		if (target_Param.team_type != my_team)
			get_SharePoint += 1.0f;

		//�����v��(�Ƃ肠�������������ɂ�锻��)
		float dist = (cur_memory->m_sensed_Pos - my_Pos).Length();
		float my_move_Power = GetConstChrParam(character)->move_max_xz_speed;
		float current_range = ChrFunc::GetNowChargeRange(character);
		float in_RangeDist = Max(.0f, dist - current_range);
		float	in_RangeTime = (in_RangeDist / my_move_Power) / (float)game_Frame;
		
		//�Ƃ肠�����e�X�g
		float max_rate_Time = 180.0f / (float)game_Frame;
		float total_time = in_RangeTime + max_rate_Time;
		float score_Rate = test_score / total_time;

		//���X�N�l�i�e�X�g�l�j
		float	risk = 0.5f;
		float score = score_Rate * m_Bias * (1.0f - risk);
		if (score > max_score)
		{
			max_score = score;
			target_ID =  cur_memory->m_id;
		}
	}
	m_target_ID = target_ID;
	return max_score;
}

float	GoalEvalutor_MedicalFriend::Calculate_Score(NPC_Brain* brain, Character* character)
{
	////�񕜂�K�v�Ƃ��Ă��閡���̐l�����v��
	////�����Ɛl������X�R�A������o���̂��x�X�g�H
	////num_Player* bias * dist
	//�e�X�g�l
	ENTITY_ID target_id = ENTITY_ID::id_error;
	static float permit_reliability = 0.6f;
	static float max_hp = 100.0f;
	int current_Frame = GAMEMATCHINFO.GetMatchTime();
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	TEAM_TYPE	my_team = ControllObjFunc::GetMatchData(character)->team_type;
	//���ƂȂ閡�������o
	std::vector<Character_Record*> candidate_vector;
	MemoryFunction::Get_CharacterRecordList(brain->GetMemory(),
		memory_predicate::Candidate_MedicalFriend, &candidate_vector);
	if (candidate_vector.empty())
		return .0f;

	ENTITY_ID messagesender_id = Accesor_Brain::Censor(brain)->m_friend_Message->Get_SenderID();
	float max_score = .0f;
	for (auto it : candidate_vector)
	{
		float reliability;
		if (!it->Can_Use(current_Frame,
			Accesor_Brain::Param(brain).record_character_ReliabilityTime, &reliability))
			continue;
		//�K��l�ȉ��̐M���l
		if (reliability <= permit_reliability)
			continue;
		if (it->m_team_Type != my_team)
			continue;

		//GoalResult�ɋL�^����Ă���ꍇ�͉񕜂��Ȃ�
		if (brain->GetMemory().m_goal_Result->Is_Register(it->m_id))
			continue;

		float hp_desire = max_hp - it->m_hp;
		float dist = (it->m_sensed_Pos - my_Pos).Length();
		float dist_disire = 5.0f / dist;
		//dying�̏ꍇ���␳��������
		float	is_dying = (it->m_status == arive_status::dying) ? 2.0f : 1.0f;
		
		//�������U������Ă���ꍇ�͏����Ⴍ�Ȃ�
		float risk = (Accesor_Brain::Symbol(brain).m_is_Danger !=
																			ENTITY_ID::id_error)?0.5f : 1.0f;
		float score = hp_desire * dist_disire * risk * is_dying * m_Bias;
		//���b�Z���W���[�^�[�Q�b�g����v�����ꍇ�̕␳
		if (messagesender_id == it->m_id)
			score *= m_message_Bias;

		if (score > max_score)
		{
			max_score = score;
			target_id = it->m_id;
		}
	}
	m_target_ID = target_id;
	return max_score;
}

float GoalEvalutor_Scouting::Get_Rate(NPC_Brain* brain, const SOUND_TYPE& type)
{
	static float gun_rate = 0.3f;
	static float foot_rate = .5f;
	switch (type)
	{
	case SOUND_TYPE::foot:
		return foot_rate;
		break;
	case SOUND_TYPE::gun:
		return gun_rate;
		break;
	default:
#if NPC_ASSART
		MyAssert(false, "GoalEvalutor_Scouting::Get_Rate�ɑ����Ă���Type����������", enum_cast(type));
#endif
		break;
	}
	return .0f;
}

float	GoalEvalutor_Scouting::Calculate_Score(NPC_Brain* brain, Character* character)
{
	return .0f;
}

float GoalEvalutor_CorrespondenceDenger::Calculate_Score(NPC_Brain* brain, Character* character)
{
	static float ret = 100.0f;
	//��ʊO����̍U���ɑ΂��Ă̕]�����s��
	MEMORY* const memory = &brain->GetMemory();
	std::vector<Threat_OutSight>*const candidate_List = memory->Get_Threat_OutSightContainer();
	if (candidate_List->empty())
		return  .0f;

	//��������G�����߂�
	float most_MinAangle = -2.0f;
	Vector3 my_Front = CharacterUtilityFunctions::GetFront(character);
	my_Front.Normalize();
	for (auto& it : *candidate_List)
	{
		//GoalResult�ɋL�^����Ă���ID�͕]�����Ȃ�
		if (brain->GetMemory().m_goal_Result->Is_Register(it.m_id))
			continue;

		float cos = Vector3Dot(my_Front,it.m_vec);
		if (cos >= most_MinAangle)
		{
			most_MinAangle = cos;
			m_target_ID = it.m_id;
		}
	}
	return ret;
}

float GoalEvalutor_FollowFriend::Calculate_Score(NPC_Brain* brain, Character* character)
{
	static const float field_size = 50.0f;
	static const float test_bias = 0.05f;
	static const float test_value = 10.0f;

	Censor_FriendMessage* messanger = Accesor_Brain::Censor(brain)->m_friend_Message;
	if (messanger->GetFriendMessageType() != FriendMessageIcon::MsgType::_COME_ON)
		return .0f;
	
	//�L�����N�^�[�̋L���擾
	float reliability;
	Character_Record*	target_Record = brain->GetMemory().Get_CharacterMemory(
		messanger->Get_SenderID(),Accesor_Brain::Param(brain).record_character_ReliabilityTime, &reliability);
	if (reliability <= 0.6f)
		return false;

	Vector3 cur_Pos = ControllObjFunc::GetPos(character);
	//�����v��
	Vector3 vec = target_Record->m_sensed_Pos - cur_Pos;
	float dist = vec.Length();
	float dist_desire = Max(field_size - dist,.0f);
	
	float score = test_value * test_bias * dist_desire;
	return score;
	
}

float GoalEvalutor_Wander_Field::Calculate_Score(NPC_Brain* brain, Character* character)
{
	return 0.0000001f;
}
