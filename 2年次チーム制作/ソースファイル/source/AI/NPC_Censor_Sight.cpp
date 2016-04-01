#include	"..\All.h"

Censor_Sight::Censor_Sight(const int update_Interval, const float fov, const float sight_Range, const int reaction_Speed) :
NPC_Censor(update_Interval, reaction_Speed), m_fov(fov), m_sight_Range(sight_Range), look(false)
{	}

Censor_Sight::~Censor_Sight()
{

}

// ���̏ꍇMapObject�Ƃ�Ray�Ɏ��s���邩��
bool	Censor_Sight::SightCheck_and_Line_of_Fire(Character* character, const Vector3& target,
									bool* visibility, bool* line_of_Fire,bool* in_Fov, const int team_type )
{
	Vector3 m_Pos = ControllObjFunc::GetPos(character);
	m_Pos.y += .5f;
	Vector3 target_Pos = target;
	target_Pos.y += .5f;
	float angle = CharacterUtilityFunctions::GetAngle(character);
	Vector3 front(sinf(angle), .0f, cosf(angle));
	Vector3 my_Target = target_Pos - m_Pos;
	float dist_sq = my_Target.LengthSq();	//����������p

	//��������
	if (dist_sq > m_sight_Range)
		return false;

	my_Target.Normalize();
	float cos = Vector3Dot(front, my_Target);
	//���씻��
	if (cos < m_fov)
		return false;
	
	//��������
	int my_team = ControllObjFunc::GetMatchData(character)->team_type;
	if (team_type ==my_team)
	{
		*visibility = true;
		*in_Fov = true;
	}
	//ray���΂�
	Vector3 out;
	float dist = sqrtf(dist_sq);
	MapObjectInterface* object = MAP_MANAGER.RayPick(&out, &m_Pos, &my_Target, &dist);
	if (object)
	{
		*line_of_Fire = false;
		if (team_type == my_team)
			*visibility = false;
		return true;
	}
	*in_Fov = true;
	*line_of_Fire = true;
	*visibility = true;
	return true;
}

bool	Censor_Sight::Judge_Dispatch_PlanningMsg(NPC_Brain* brain, Character* character, MapObject_Record* record)
{
	return false;
}

bool	Censor_Sight::Judge_Attack(NPC_Brain* brain, Character* character, Character_Record* record)
{
	//���F�����G���^�[�Q�b�g�ɂ��邩�ǂ����𒲍�
	//���݂̃^�[�Q�b�g�Ɠ����ꍇ�͕]�����Ȃ�
	FinalGoal_Evalutor* current_FinalGoal = Accesor_Brain::Final_GoalEvalutor(brain);

	ENTITY_ID current_FinalTarget = game_id::id_error;
	if (current_FinalGoal)
		current_FinalTarget = current_FinalGoal->Get_TargetID();

	if (record->m_id == Accesor_Brain::Current_Target(brain) ||
		record->m_id == current_FinalTarget)
		return false;

	//�������U�����Ă����ꍇ��fuzzyTool�����������ꍇ�̓v�����j���O���s��
	if (record->m_action.m_tag == expectation_act::attack_me)
		return true;

	return true;

	//������charge��Ԃ��t�@�W�[�����APlanning���s�����𔻒f����
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	float target_Dist = (record->m_sensed_Pos - my_Pos).Length();
	int current_Charge = (int)((float)ChrFunc::GetUI(character)->GetColaGaugePower() * 100);
	float score = Accesor_Brain::FuzzyTool(brain)->Calculate_Sight_Target_Character(target_Dist, (float)current_Charge);

	if (score >= Accesor_Brain::FuzzyParam(brain).m_sight_target)
		return true;

	return false;
}

bool	Censor_Sight::Judge_Medical(NPC_Brain* brain, Character* character, Character_Record* record)
{
	static float min_hp = 40;
	
	if (record->m_hp <= min_hp)
		return true;

	return false;
}

bool	Censor_Sight::Judge_Dispatch_PlanningMsg(NPC_Brain* brain, Character* character, Character_Record* record)
{
	//�s���\��Ԃł͂Ȃ��ꍇ��return
	
	if (!Accesor_Brain::StatusParam(brain).m_is_Active)
		return false;
	//�L���ɂȂ��ꍇ��return
	if (!record)
		return false;

	//���F���Ă��Ȃ��ꍇ��return
	if (!record->m_visibilily)
		return false;

	//����ł���ꍇ��return
	if (record->m_status == arive_status::dead)
		return false;

	//PlanningMemory�ɓo�^����Ă���ꍇ�̓v�����j���O���s��Ȃ�
	if(MemoryFunction::Is_Register_PlanningMemory(brain,censor_type::sight, record->m_id) == -1)
		return false;

	//�����`�[���̏ꍇ��
	if (record->m_team_Type == ControllObjFunc::GetMatchData(character)->team_type)
		return Judge_Medical(brain,character,record);

	return Judge_Attack(brain, character, record);
}

arive_status	Censor_Sight::Cul_Status(Character* character)
{
	if (ChrFunc::isDead(character))
		return arive_status::dead;
	if (ChrFunc::isDying(character))
		return arive_status::dying;

	return arive_status::arive;
}

bool	Censor_Sight::Look_MapObject(NPC_Brain* brain, Character* character)
{
	int player_num = ControllObjFunc::GetMatchData(character)->player_num;


	bool	ret = false;
	int current_Frame = GAMEMATCHINFO.GetMatchTime();
	MapObjectManager::MAPOBJ_ARRAY work_Array;
	MAP_MANAGER.Get_ShareTypeObject(work_Array);
	MAP_MANAGER.Get_TypeObject(work_Array, MAP_OBJ_TYPE::_truck);
	for (auto it : work_Array)
	{
		//�I�u�W�F�N�g�̃^�C�v�擾�i�������Ă���I�u�W�F�N�g�����邽��)
		MAP_OBJ_TYPE type = it->Get_Type();
		bool see_through = Is_See_Through(it->Get_Type());
		Vector3 target_Pos = it->Get_Param().pos;
		bool visibility = false, lineof_fire = false, in_Fov = false;
		if (it->GetId() == 53 || it->GetId() == 54)
		{
			debug_particle::point(it->Get_Param().pos, Vector3(0, 0, 0), 1.0f,dword_color::blue);
		}
		//�����Ǝː�����
		SightCheck_and_Line_of_Fire(character, target_Pos, &visibility, &lineof_fire,&in_Fov,see_through);
		if (visibility)
		{
			ENTITY_ID  target_id = it->GetId();
			//�L���̈�ɑ��݂��邩
			brain->GetMemory().Is_Record_MapObject(target_id);

			MapObject_Record* record_ptr = brain->GetMemory().m_mapobject_Record[target_id];

			//����܂ŉ��ł��Ă��Ȃ��ꍇ�̏��
			if (!record_ptr->m_visibilily)
			{
				record_ptr->m_became_Visible = current_Frame;
				record_ptr->m_became_Pos = target_Pos;
			}
			record_ptr->m_visibilily = true;
			record_ptr->m_line_of_Fire = lineof_fire;
			record_ptr->m_in_Fov = in_Fov;
			record_ptr->m_last_SensedTime = current_Frame;
			record_ptr->m_object_type = Convert_MapObject_MyEnum(type);

			record_ptr->m_sensed_Angle = it->Get_Param().angle;
			//�Ƃ肠����
			Vector3 before_Pos = record_ptr->m_sensed_Pos;
			record_ptr->m_sensed_Velocity = it->Get_MoveParameter();
			//record_ptr->m_sensed_Velocity = target_Pos - before_Pos;
			record_ptr->m_sensed_Pos = target_Pos;
			if (record_ptr->m_object_type == mapobjcet_type::denger_object)
			{
				if (Accesor_Brain::Evalutor(brain)->Check_Danger_Object(brain, character, record_ptr))
				{
					Accesor_Brain::ParticularGoal_Manager(brain)->Set_TargetID(record_ptr->m_id);
					Accesor_Brain::ParticularGoal_Manager(brain)->Insert_Goal(character, partculargoal_type::avoidancevessel);
				}
			}

			if (Judge_Dispatch_PlanningMsg(brain, character, record_ptr))
			{
				MemoryFunction::Register_PlanningMemory(brain, censor_type::sight, record_ptr->m_id);
				ret = true;
			}
		}
		else//���F�ł��Ȃ������ꍇ
		{
			//�L�����N�^�[���L�����ɂ���ꍇ
			auto record = brain->GetMemory().m_mapobject_Record.find(it->GetId());
			if (record != brain->GetMemory().m_mapobject_Record.end())
			{
				record->second->m_visibilily = visibility;
				record->second->m_line_of_Fire = lineof_fire;
				record->second->m_in_Fov = in_Fov;
			}
		}
	}
	return ret;
}

bool	Censor_Sight::Look_Character(NPC_Brain* brain, Character* character)
{
	int player_num = ControllObjFunc::GetMatchData(character)->player_num;


	bool	ret = false;
	int current_Frame = GAMEMATCHINFO.GetMatchTime();

	/*�Ƃ肠�����ŏ���Ray���΂�*/
	std::list<LPCONTROLLOBJCT> work_List = CONTROLLOBJMGR.GetObjectList(npc_predicate::C_Test);
	for (auto it = work_List.begin(); it != work_List.end(); it++)
	{
		//�������g�͔�΂�
		if ((*it) == character) continue;

		Character* target_ptr = dynamic_cast<Character*>((*it));
		if (!target_ptr)
			continue;
		Vector3 target_Pos = ControllObjFunc::GetPos((*it));
		//�����t���O�������Ă���ꍇ�͖������Ŏ��F���Ă��邱�Ƃɂ���
		bool visibility = false, lineof_fire = false, in_Fov = false;
		SightCheck_and_Line_of_Fire(character, target_Pos, &visibility, &lineof_fire,&in_Fov,
													ControllObjFunc::GetMatchData(target_ptr)->team_type);
		if (visibility)
		{
			//�L���̈�ɑ��݂��邩�ǂ����`�F�b�N
			brain->GetMemory().Is_Record_Character((*it)->GetId());
			Character_Record* record_ptr = brain->GetMemory().m_character_Record[(*it)->GetId()];
			
			//����܂ŉ��ł��Ă��Ȃ��ꍇ�̏��
			ENTITY_ID target = (*it)->GetId();
			if (!record_ptr->m_visibilily)
			{
				record_ptr->m_became_Visible = current_Frame;
				record_ptr->m_became_Pos = target_Pos;
			}
			record_ptr->m_visibilily = visibility;
			record_ptr->m_line_of_Fire = lineof_fire;
			record_ptr->m_in_Fov = in_Fov;
			//�e�X�g												 
			record_ptr->m_team_Type = ControllObjFunc::GetMatchData((*it))->team_type;
			record_ptr->m_last_SensedTime = current_Frame;
			record_ptr->m_sensed_Angle = CharacterUtilityFunctions::GetFront(target_ptr);
			record_ptr->m_sensed_Pos = target_Pos;
			record_ptr->m_sensed_Velocity = CharacterUtilityFunctions::GetParam(target_ptr).m_move;
		
			record_ptr->m_action = Accesor_Brain::Evalutor(brain)->Expectation_Action(character, target_ptr, record_ptr);
			record_ptr->m_expectation_Time = current_Frame;
			record_ptr->m_status = Cul_Status(target_ptr);
			record_ptr->m_hp = ChrFunc::GetParam(target_ptr).m_LifePoint;
			record_ptr->m_update = true;
			//����ł���ꍇ��DELETE�t���O��active�ɂ���
			if (record_ptr->m_status == arive_status::dead)
				record_ptr->Delete();

			if (Judge_Dispatch_PlanningMsg(brain, character, record_ptr))
			{
				MemoryFunction::Register_PlanningMemory(brain, censor_type::sight, record_ptr->m_id);
				ret = true;
			}
			////�v�����j���O���s�����ǂ��������肷��
			//if (!ret)
			//{
			//	ret = Judge_Dispatch_PlanningMsg(brain, character, record_ptr);
			//	if (ret)
			//		MemoryFunction::Register_PlanningMemory(brain, censor_type::sight, record_ptr->m_id);
			//		//brain->Register_PlanningMemory(censor_type::sight, record_ptr->m_id);
			//}
		}
		else//���F�ł��Ȃ������ꍇ
		{
			//�L�����N�^�[���L�����ɂ���ꍇ
			auto record = brain->GetMemory().m_character_Record.find((*it)->GetId());
			if (record != brain->GetMemory().m_character_Record.end())
			{
				record->second->m_update = true;
				record->second->m_visibilily = visibility;
				record->second->m_line_of_Fire = lineof_fire;
				record->second->m_in_Fov = in_Fov;
				record->second->m_status = Cul_Status(dynamic_cast<Character*>(IDMANAGER.GetPointer((*it)->GetId())));
				if (current_Frame - record->second->m_expectation_Time >= Accesor_Brain::Param(brain).record_ExtAct_ReliabilityTime)
				{
					record->second->m_action.m_tag = expectation_act::i_dont_know;
				}
			}
			else//���܂ň�x�����F�ł��Ă��Ȃ��ꍇ
	//�L�����X�V�������Ƃ�`����
				continue;
		}
	}
	ai_message->Send_BrainMessage(brain, character->GetId(),
		character->GetId(), msg_t::memory_update, (void*)character);
	return ret;
}

void	Censor_Sight::Update_Sight(NPC_Brain* brain, Character* character)
{
	 int current_Frame = GAMEMATCHINFO.GetMatchTime();
	 int chara_update_Interval = Accesor_Brain::Param(brain).m_chara_SightInterval;
	 if (current_Frame  % chara_update_Interval == 0 && Accesor_Brain::StatusParam(brain).m_is_Arive)
	{
		 //�X�V�t���O��false��
		 brain->GetMemory().Reset_UpdateFlg_CharacterRecord();
		if (Look_Character(brain, character))
		{
			ai_message->Plannning(brain, character, &std::string("Censor_Sight::Character"), m_reaction_Speed);
		}
		//�t�B�[���h���炢�Ȃ��Ȃ��Ă��܂������̂ɑ΂��鏈��
		Delete_NotField_CharacterRecord(brain);
	}
	else if (look)
	{
		//�X�V�t���O��false��
		brain->GetMemory().Reset_UpdateFlg_CharacterRecord();
		if (Look_Character(brain, character))
		{
			ai_message->Plannning(brain, character, &std::string("Censor_Sight::Character"), m_reaction_Speed);
		}
		look = false;
		//�t�B�[���h���炢�Ȃ��Ȃ��Ă��܂������̂ɑ΂��鏈��
		Delete_NotField_CharacterRecord(brain);
		
	}
	 int map_object_Interval = Accesor_Brain::Param(brain).m_map_SightInterval;
	 if (current_Frame  % map_object_Interval == 0 && Accesor_Brain::StatusParam(brain).m_is_Arive)
	 {
		 if (Look_MapObject(brain, character))
		 {
			 ai_message->Plannning(brain, character, &std::string("Censor_Sight::MapObject"), m_reaction_Speed);
		 }
	 }
}

void	Censor_Sight::Update(NPC_Brain* brain, Character* character)
{
	Update_Sight(brain, character);
}

void	Censor_Sight::Delete_NotField_CharacterRecord(NPC_Brain* brain)
{
	auto* array_ptr = &brain->GetMemory().m_character_Record;
	
	for (auto& it : *array_ptr)
	{
		if (!it.second->m_update && !it.second->Is_Delete())
			it.second->Delete();
	}
}
