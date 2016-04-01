#include	"..\All.h"	

NPC_Evalutor::NPC_Evalutor(NPC_Brain* holder) :
m_holder(holder)
{

}

bool	NPC_Evalutor::SightCheck_and_Line_of_Fire(Character* character, const Vector3& target,
	bool* visibility, bool* line_of_Fire,bool* in_Fov, int team_type)
{
	return m_holder->m_censor.m_sight->SightCheck_and_Line_of_Fire(character, target, visibility,
																												line_of_Fire,in_Fov, team_type);
}

ExpectationAct	NPC_Evalutor::Expectation_Action(Character* me, Character* enemy, const Character_Record* enemy_record)
{
	/**
	*@param 3�@�������U�����Ă���Ɗ����鋗��
	*/
	auto Check_AttackMe = [](const  Vector3& my_Pos, Character* enemy,
		const Vector3& enemy_Front, const float& feel_Attack, const float& feel_Angle)
	{
		Vector3 target_Pos = ControllObjFunc::GetPos(enemy);
		Vector3 vec = my_Pos - target_Pos;
		float cos_Angle = Vector3Dot(enemy_Front, vec);
		float dist_sq = vec.LengthSq();
		if (dist_sq <= feel_Attack && cos_Angle >= feel_Angle)
		{
			return expectation_act::attack_me;
		}
		return expectation_act::i_dont_know;
	};
	//auto Check_AttackTarget_Character = [](Character* target,  Character* enemy,
	//										const float& feel_Attack)
	//{
	//	int enemy_team = ControllObjFunc::GetMatchData(enemy)->team_type;
	//	int target_team = ControllObjFunc::GetMatchData(target)->team_type;
	//	Vector3 target_pos = ControllObjFunc::GetPos(target);
	//	Vector3 enemy_pos = ControllObjFunc::GetPos(enemy);
	//	float dist_sq = (target_pos - enemy_pos).LengthSq();
	//	if (dist_sq <= feel_Attack)
	//		return(enemy_team != target_team) ? expectation_act::attack_friend : expectation_act::curefrined;
	//	return expectation_act::none;
	//};

	auto Check_AttackTarget_MapObject = [](MapObjectInterface* checkObject, Character* enemy,
		const float& feel_Attack)
	{

	};

	//�G�̃X�e�[�g�擾
	ExpectationAct ret;
	CHARACTER_STATE_TYPE target_state_Type = ChrFunc::GetStateType(enemy);
	switch (target_state_Type)
	{
	case CHARACTER_STATE_TYPE::die_mode:
		ret.m_tag = expectation_act::die;
		return ret;
		break;
	case CHARACTER_STATE_TYPE::dying_mode:
		ret.m_tag = expectation_act::dieing;
		return ret;
		break;
	default:
		break;
	}
	Vector3 target_Velocity = ChrFunc::GetParam(enemy).m_move;
	//�G�̑O�����擾
	Vector3 enemy_Front = ChrFunc::GetFront(enemy);
	enemy_Front.Normalize();
	expectation_act work_act;
	//�������U�����Ă��邩�ǂ����𔻒肷��
	work_act = Check_AttackMe(ControllObjFunc::GetPos(me), enemy, enemy_Front,
		m_holder->m_chara_Param.m_feelAttack, m_holder->m_chara_Param.m_enemy_AttackMe_Fov);
	if (work_act == expectation_act::attack_me)
	{
		ret.m_tag = work_act;
		ret.m_vec = enemy_Front;
		return ret;
	}
	ret.m_tag = expectation_act::move_field;
	ret.m_vec = target_Velocity;
	ret.m_vec.Normalize();
	return ret;
}

void	NPC_Evalutor::Decision_TargetEnemy(Character* character)
{
	//�L���ɂ���G��񂩂�ł��]���l�̍����G���^�[�Q�b�g�ɂ���
	float max_Score = .0f;
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	for (
		std::map<ENTITY_ID, Character_Record*>::iterator it = m_holder->m_memory.memory.m_character_Record.begin();
		it != m_holder->m_memory.memory.m_character_Record.end();
	++it
		)
	{
		if (it->second->m_status == arive_status::dead ||
			it->second->m_status == arive_status::i_dont_now)
			continue;

		if (!it->second->m_visibilily)
			continue;
		if (it->second->m_team_Type == ControllObjFunc::GetMatchData(character)->team_type)
			continue;

		float dist = (my_Pos - it->second->m_sensed_Pos).Length();
		int current_Charge = (int)((float)ChrFunc::GetUI(character)->GetColaGaugePower() * 100);
		float score = m_holder->m_fuzzy.Calculate_Sight_Target_Character(dist, (float)current_Charge);
		if (score > max_Score)
		{
			max_Score = score;
			m_holder->m_target.current = it->second->m_id;
		}
	}
	//�G�����Ȃ��ꍇ��Evalutor���w�肵���^�[�Q�b�g��ݒ肷��
	if (m_holder->m_target.current == game_id::id_error)
	{
		m_holder->m_target.current = m_holder->m_think->Get_FinalGoal()->Get_TargetID();
	}
}


//FinalEvalutor�̕]����������������̂ł�����͒P���ɋ����Ǝː������ł݂�
bool	NPC_Evalutor::Direction_AdvertisementPointObject(Character* character)
{
	float max_score = .0f;
	ENTITY_ID direction_ID = ENTITY_ID::id_error;
	Vector3 my_Pos = ControllObjFunc::GetPos(character);

	for (auto& cur_memory : m_holder->m_memory.memory.m_mapobject_Record)
	{
		if (cur_memory.second->m_object_type != mapobjcet_type::poster)
			continue;
		//�|�C���^�擾
		ShareRatioObjectBase* object_ptr = dynamic_cast<ShareRatioObjectBase*>(IDMANAGER.GetPointer(cur_memory.second->m_id));
		MyAssert(object_ptr, "�I�u�W�F�N�g�̃|�C���^���Ȃ��E�E�E");
		if (m_holder->GetMemory().m_goal_Result->Is_Register(cur_memory.second->m_id))
			continue;
		Vector3 object_Pos = cur_memory.second->m_sensed_Pos;
		Vector3 vec = object_Pos - my_Pos;
		float dist = vec.Length();
		//�X�e�[�W�̒[����[�̋���
		static float stage_max_dist = 100.0f;
		float dist_rate = stage_max_dist - dist;
		dist_rate = Max(dist_rate, .0f);
		float visibility_rate = (cur_memory.second->m_visibilily) ? 2.0f : .3f;
		float line_of_fire = (cur_memory.second->m_line_of_Fire) ? 1.0f : 0.2f;
		//�I�u�W�F�N�g�̌��݂�Rate���擾
		ShareRatioParam param = object_ptr->Get_ShareParam();
		//�G�̃`�[���^�C�v��D��
		int my_team_Type = ControllObjFunc::GetMatchData(character)->team_type;
		float team_typerate = (my_team_Type == param.team_type) ? 1.0f : 1.3f;
		//rate�ɂ��␳
		float poster_Rate;
		if (param.team_type == -1)	//�ǂ����Share���ĂȂ��ꍇ
			poster_Rate = 1.0f;
		else if (param.team_type == my_team_Type)
			poster_Rate = 0.2f;
		else
			poster_Rate = 1.3f;

		//�Ƃ肠�������̎��Ńe�X�g
		float score = visibility_rate * dist_rate * team_typerate * poster_Rate * line_of_fire;
		if (score > max_score)
		{
			max_score = score;
			direction_ID = cur_memory.second->m_id;
		}
	}
	if (direction_ID == ENTITY_ID::id_error)
		return false;
	Accesor_Brain::Set_Current_Target(m_holder, direction_ID);
	//m_holder->Set_CurrentTarget(direction_ID);
	return true;
}

bool	NPC_Evalutor::Expectation_TargetPos_Character(const Character_Record* memory,
	Vector3* ret_Pos, const float& memory_Reliability)
{
	//�L�������X�g���Ă���ꍇ
	if (!memory)
	{
#if ASSERT
		MyAssert(false, "Expectation_TargetPos_Character��memory�����X�g���Ă��܂�");
#endif
		*ret_Pos = Vector3(.0f, .0f, .0f);
		return false;
	}
	//�L���̐M���l���Ⴂ�ꍇ�͈�ԋ߂��Ŕ�\���ʒu�Ƃ���
	if (memory_Reliability < 0.3f)
	{
#if ASSERT
		MyAssert(false, "Expectation_TargetPos_Character�ŋL���̐M���l���Ⴂ");
#endif
		*ret_Pos = Vector3(.0f, .0f, .0f);
		return false;
	}

	/*�G���ړ�����������\������*/
	int	progress_Frame = GAMEMATCHINFO.GetMatchTime() - memory->m_last_SensedTime;
	//���x�͕ς���Ă��Ȃ��Ɖ���
	float target_MoveDist = memory->m_sensed_Velocity.Length() * progress_Frame;

	std::list<Field_Node*> worklist;
	worklist.clear();
	Vector3 target_MoveVec = memory->m_sensed_Velocity;
	//�^�[�Q�b�g�������Ă��Ȃ��ꍇ�͑O�������Ƃ��Ă���
	if (target_MoveVec.LengthSq() <= 0.0001f)
	{
		//�^�[�Q�b�g�����񂾂��Ƃ�F���ł��ĂȂ��Ƃ��ɗ�������ۂ�
		target_MoveVec = memory->m_sensed_Angle;
	}
	target_MoveVec.Normalize();
	//�^�[�Q�b�g���Ō�ɏ������Ă����Ǝv����m�[�h
	Field_Node*sensed_Node = navigation_system->Find_Node_Pos(memory->m_sensed_Pos);

	//�i�s��������m�[�h���E���Ă���
	navigation_system->Find_Node_Container<std::list<Field_Node*>>(sensed_Node->pos, &target_MoveVec, target_MoveDist, &worklist);
	//�i�s�x�N�g����k�Ƃ��Ėk���A�k��������m�[�h������Ă���
	float move_Angle = atan2f(target_MoveVec.x, target_MoveVec.z);
	Vector3 work_Vec = Vector3(sinf(move_Angle - Direction::direction_space), .0f, cosf(move_Angle - Direction::direction_space));
	navigation_system->Find_Node_Container(memory->m_sensed_Pos, &work_Vec, target_MoveDist, &worklist, true);
	work_Vec.x = sinf(move_Angle + Direction::direction_space);
	work_Vec.z = cosf(move_Angle + Direction::direction_space);
	navigation_system->Find_Node_Container(memory->m_sensed_Pos, &work_Vec, target_MoveDist, &worklist, true);

	float target_MoveSq = target_MoveDist * target_MoveDist;
	//���������Ɣ�r���Ĉړ��ł��Ȃ��ł��낤�m�[�h���͂���
	for (auto it = worklist.begin(); it != worklist.end();)
	{
		float distsq = ((*it)->pos - sensed_Node->pos).LengthSq();
		if (target_MoveSq < distsq)
		{
			it = worklist.erase(it);
			continue;
		}
		it++;
	}
	//field_Info->Set_Debug_NodeList<std::list<Field_Node*>>(&worklist);
	//�c�����m�[�h�ɑ΂��ĕ]���t�����Ă���

	float max_Score = .0f;
	Field_Node*	expectation_Node = nullptr;

	for (auto it = worklist.begin(); it != worklist.end(); it++)
	{
		float score = 100;
		//�i�s��������ǂꂾ���p�x�����邩�`�F�b�N
		Vector3 vec = (*it)->pos - memory->m_sensed_Pos;
		vec.Normalize();
		float work = Harf_DotCos(target_MoveVec, vec, 0.25, 0.75);
		score *= work;

		if (score > max_Score)
		{
			max_Score = score;
			expectation_Node = (*it);
		}
	}
	//�m�[�h��nullptr�̏ꍇ�͂܂�sensed_Pos�̋߂��ɂ���Ƃ���memory->sensed_Pos��Ԃ�
	if (!expectation_Node)
		*ret_Pos = memory->m_sensed_Pos;

	*ret_Pos = expectation_Node->pos;
	return true;
}

bool	NPC_Evalutor::Expectation_TargetPos_MapObejct(const MapObject_Record* memory, Vector3* target_Pos, const float& memory_Retability)
{
	if (!memory)
	{
		*target_Pos = Vector3(0, 0, 0);
		return false;
	}
	*target_Pos = Vector3(0, 0, 0);
	return true;
}

static float target_height_adjust = 1.5f;

bool		NPC_Evalutor::Range_of_Traget(Character* character, float* target_dist,
															bool* line_of_fire,bool* in_fov)
{
	Vector3 target_Pos;
	float reliability;
	if (IS_MAPOBJ_ID(m_holder->m_target.current))
	{
		MapObject_Record* mem = m_holder->Get_MapObjectMemory(m_holder->m_target.current, &reliability);
		if (!mem)
			return false;
		if (!mem->m_line_of_Fire)
		{
			if (line_of_fire)
				*line_of_fire = false;
			return false;
		}
		target_Pos = mem->m_sensed_Pos;
	}
	else
	{
		Character_Record* mem = m_holder->Get_CharacterMemory(m_holder->m_target.current, &reliability);
		if (!mem)
			return false;
		if (!mem->m_line_of_Fire)
		{
			if (line_of_fire)
				*line_of_fire = false;
			return false;
		}
		target_Pos = mem->m_sensed_Pos;
		target_Pos.y += 0.7f;	//BodyCentor������
	}
	if (line_of_fire)
		*line_of_fire = true;

	int current_Chagerate = (int)((float)ChrFunc::GetUI(character)->GetColaGaugePower() * 100);
	if (current_Chagerate <= 0)
		return false;

	Vector3 my_pos = character->GetBodyCenter() +
									Vector3Normalize(ChrFunc::GetParam(character).m_view_vec)*0.2f;
	
	Vector3 vec = target_Pos - my_pos;
	float xz_dist = sqrtf(vec.x * vec.x + vec.z* vec.z);
	float move_power = ChrFunc::GetNowChargeRange(character) / 120.0f;
	float time = xz_dist / move_power;
	vec.Normalize();
	vec *= move_power;
	float gravity = 0.001f;
	//�����_�\��
	float y_pos = my_pos.y + vec.y * time + (0.5f * -gravity *time* time);
	if (y_pos + target_height_adjust >= target_Pos.y)
		return true;

	return false;

}

bool	NPC_Evalutor::Target_InSight()
{
	float reliability;
	if (IS_MAPOBJ_ID(m_holder->m_target.current))
	{
		MapObject_Record* target_Memory = m_holder->Get_MapObjectMemory(m_holder->m_target.current, &reliability);
		if (!target_Memory)
			return false;
		return target_Memory->m_visibilily;
	}
	Character_Record* target_Memory = m_holder->Get_CharacterMemory(m_holder->m_target.current, &reliability);
	if (!target_Memory)
		return false;
	return target_Memory->m_visibilily;
}

bool	NPC_Evalutor::Target_at_LoseSight(Character* character,
	Vector3* evalute_GoalPos, float* target_dist)
{
	Vector3  target_Pos;
	float reliability;
	if (Expectation_TargetPos_Character(m_holder->Get_CharacterMemory(Accesor_Brain::Current_Target(m_holder), &reliability)
		, &target_Pos, reliability))
	{
		return false;
	}
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	Vector3 vec = target_Pos - my_Pos;
	float dist = vec.Length();
	*target_dist = dist;
	int current_Charge = (int)((float)ChrFunc::GetUI(character)->GetColaGaugePower() * 100);
	
	const float desirable = Accesor_Brain::FuzzyTool(m_holder)->Calculate_Sight_Target_Character(dist, (float)current_Charge);
	*evalute_GoalPos = target_Pos;

	if (desirable >= 60)
		return true;
	return false;
}

bool	NPC_Evalutor::Direction_MedicalFriend(NPC_Brain* brain, Character* character)
{
	static float min_hp = 40;
	static float permit_reliability = 0.6f;
	static float permit_dist_sq = 400.0f;

	//���b�Z���W���[�Ƀ^�[�Q�b�g������ꍇ
	ENTITY_ID messanger_Target = ENTITY_ID::id_error;
	if (Accesor_Brain::Censor(brain)->m_friend_Message->GetFriendMessageType() == FriendMessageIcon::MsgType::_HELP)
	{
		messanger_Target = Accesor_Brain::Censor(brain)->m_friend_Message->Get_SenderID();
		if (messanger_Target == ENTITY_ID::id_error)
		{
			MyAssert(false, "����͂�������");
			return false;
		}
		Accesor_Brain::Set_Current_Target(brain, messanger_Target);
		return true;
	}

	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	TEAM_TYPE	my_team = ControllObjFunc::GetMatchData(character)->team_type;
	//���݂̍ŏI�^�[�Q�b�g����]������
	ENTITY_ID final_Target = Accesor_Brain::Final_GoalEvalutor(brain)->Get_TargetID();
	//�ŏI�^�[�Q�b�g���������Ă��Ĉ�苗���ɂ���ꍇ�͂������^�[�Q�b�g�ɂ���
	if (final_Target != ENTITY_ID::id_error)
	{
		float reliability;
		Character_Record* friend_memory = brain->Get_CharacterMemory(final_Target, &reliability);
		if (friend_memory)
		{
			if (friend_memory->m_hp <= 40 && reliability >= permit_reliability &&
				friend_memory->m_team_Type == my_team)
			{
				float dist_sq = (friend_memory->m_sensed_Pos - my_Pos).LengthSq();
				if (dist_sq <= permit_dist_sq)
				{
					Accesor_Brain::Set_Current_Target(brain, final_Target);
					return true;
				}
			}
		}
	}
		//�񕜑ΏۂƂȂ閡����ݒ肷��
		std::vector<Character_Record*> candidate_vector;
		MemoryFunction::Get_CharacterRecordList(brain->GetMemory(),
									memory_predicate::Candidate_MedicalFriend, &candidate_vector);
		if (candidate_vector.empty())
			return false;
		
		static float max_hp = 100.0f;
		final_Target = ENTITY_ID::id_error;
		float max_score = .0f;
		int	current_Frame = GAMEMATCHINFO.GetMatchTime();
		for (auto it : candidate_vector)
		{
			//�g�p�ł��邩�ǂ���
			float reliability;
			if (!it->Can_Use(current_Frame,
				Accesor_Brain::Param(brain).record_character_ReliabilityTime, &reliability))
				continue;
			//�K��l�ȉ��̐M���l
			if (reliability <= permit_reliability)
				continue;
			if (it->m_team_Type != my_team)
				continue;
			if (brain->GetMemory().m_goal_Result->Is_Register(it->m_id))
				continue;

			float hp_desire = max_hp - it->m_hp;
			float dist = (it->m_sensed_Pos - my_Pos).LengthSq();
			float dist_disire = 1.0f / dist;

			float score = hp_desire * dist_disire;


			if (score >= max_score)
			{
				final_Target = it->m_id;
				max_score = score;
			}
		}
		if (final_Target != ENTITY_ID::id_error)
		{
			Accesor_Brain::Set_Current_Target(brain, final_Target);
			return true;
		}

		return false;
	}


bool	NPC_Evalutor::Target_is_NearRange(NPC_Brain* brain, Character* character)
{
	ENTITY_ID current_Target = Accesor_Brain::Current_Target(brain);
	if (current_Target == ENTITY_ID::id_error)
	{
		MyAssert(false, "����͂�������");
		return false;
	}
	float reliability;
	Character_Record* target_record = brain->Get_CharacterMemory(current_Target, &reliability);
	if (!target_record)
	{
		return false;
	}
	if (!target_record->m_line_of_Fire)
		return false;

	//�����v��
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	float dist_sq = (target_record->m_sensed_Pos - my_Pos).LengthSq();
	
	if (dist_sq <= Accesor_Brain::Param(brain).m_feel_NearRangeDist)
		return true;

	return false;
}

bool	NPC_Evalutor::Direction_NearAttackTarget(NPC_Brain* brain, Character* character)
{
	ENTITY_ID target_id = Accesor_Brain::Final_GoalEvalutor(brain)->Get_TargetID();
	if (target_id != ENTITY_ID::id_error)
	{
		Accesor_Brain::Set_Current_Target(brain,target_id);
		return true;
	}
	MyAssert(false, "����͂�΂��H");
	return false;
}

bool	NPC_Evalutor::Direction_GrenadeAttackTarget(NPC_Brain* brain, Character* character)
{

	return false;
}

bool	NPC_Evalutor::Check_Danger_Object(NPC_Brain* brain, Character* character,
																	MapObject_Record* check_record)
{
	//���łɓ���S�[���̃^�[�Q�b�g�ɂȂ��Ă���ꍇ�͕]�����Ȃ�
	if (Accesor_Brain::ParticularGoal_Manager(brain)->Get_TargetID() ==
		check_record->m_id)
		return false;

	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	Vector3 vec = my_Pos - check_record->m_sensed_Pos;
	float dist = vec.Length();
	if (dist >= Accesor_Brain::Param(brain).m_far_of_Vessel)
		return false;

	//�����̌��݂̑��x��MapObject�̑��x�̓��ς���ȈՓI�ȏՓ˃V�~�����[�V�������s��
	Vector3 my_Velocity = ChrFunc::GetParam(character).m_move;
	float my_speed = my_Velocity.Length();
	Vector3 target_Velocity = check_record->m_sensed_Velocity;
	float target_speed = target_Velocity.Length();

	float cos = Vector3Dot(my_Velocity, target_Velocity) / (my_speed * target_speed);
	
	if (cos <= -0.9f && cos >= -1.0f)
		return true;

	return true;
}

bool	NPC_Evalutor::Direction_AvoidancePoint(NPC_Brain* brain, Character* character,
																			MapObject_Record*	target_record, Vector3* ret_Pos)
{
	//�������������Ă���Node���擾
	Field_Node*	mypos_Node = navigation_system->Find_Node_Pos(ControllObjFunc::GetPos((character)));
	if (!mypos_Node)
	{
		MyAssert(false, "���肦�Ȃ����Ƃ�������");
		return false;
	}
	//�^�[�Q�b�g�̑��x������p�l���Z�o
	Vector3 target_Velocity = target_record->m_sensed_Velocity;
	target_Velocity.y = .0f;
	target_Velocity.Normalize();
	
	direction target_direction = navigation_system->Vector3_Convert_Direction(target_Velocity);
	if (target_direction == direction::num_direction)
	{
		MyAssert(false, "�Ƃ߂�I");
	}
	//�Ƃ肠�������E�̃m�[�h���瓦���邱�Ƃ̂ł���m�[�h�����邩���ׂ�
	int work = ((int)target_direction + 2) % 8;
	direction right = (direction)work;
	work = ((int)target_direction + 6) % 8;
	direction left = (direction)work;

	for (auto it : mypos_Node->edge_List)
	{
		Edge_Base*work = it;
		Field_Edge*	edge = (Field_Edge*)(work);
		if (edge->m_direction == right ||
			edge->m_direction == left)
		{
			Field_Node* to = dynamic_cast<Field_Node*>(edge->to);
			if (!to)
				MyAssert(false, "�Ƃ߂�I");
			*ret_Pos = to->pos;
			return true;
		}
	}
	return false;
}
