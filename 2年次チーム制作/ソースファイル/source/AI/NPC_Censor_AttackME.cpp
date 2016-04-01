#include	"..\All.h"

//100�s�ڂŃ��������[�N


Censor_AttackMe::Censor_AttackMe(const int reaction_Speed) : NPC_Censor(1, reaction_Speed)
{
	m_reaction_TargetList.clear();
}

Censor_AttackMe::~Censor_AttackMe()
{
	for (std::list<Msg_AttackMe*>::iterator it = m_reaction_TargetList.begin();
		it != m_reaction_TargetList.end();
		++it)
	{
		if (*it)
			SAFE_DELETE(*it);
	}
	m_reaction_TargetList.clear();
}

void	Censor_AttackMe::Release()
{
	for (auto it = m_reaction_TargetList.begin(); it != m_reaction_TargetList.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	m_reaction_TargetList.clear();
}

void	Censor_AttackMe::Rewrite_Memory(NPC_Brain* brain, const Msg_AttackMe& msg)
{
	if (IS_CONTROLLOBJ_ID(msg.damage_Param.parent))
	{
		MEMORY*const memory = &brain->GetMemory();
		ENTITY_ID target = msg.damage_Param.parent;
		memory->Registar_Threat_OutSight(target, msg.damage_Param.vec);
	}

}

bool	Censor_AttackMe::Judge_Dispatch_PlanningMsg(NPC_Brain* brain, Character* character, Msg_AttackMe* judge_Msg)
{
	//�s���\��Ԃł͂Ȃ��ꍇ��return
	if (!Accesor_Brain::StatusParam(brain).m_is_Active)
		return false;

	//PlanningMemory�ɓo�^����Ă���ꍇ�̓v�����j���O���s��Ȃ�
	if(MemoryFunction::Is_Register_PlanningMemory(brain, censor_type::sight, judge_Msg->damage_Param.parent) == -1)
		return false;
	
	//��������G�����݂̃^�[�Q�b�g�A�ŏI�^�[�Q�b�g�ɂȂ��Ă���ꍇ�͑��M���Ȃ�
	if (judge_Msg->damage_Param.parent != Accesor_Brain::Current_Target(brain) &&
		judge_Msg->damage_Param.parent != Accesor_Brain::Final_GoalEvalutor(brain)->Get_TargetID())
	{
		Rewrite_Memory(brain, *judge_Msg);
		ai_message->Plannning(brain, character, &std::string("Censor_AttackMe::Judge_Dispatch_PlanningMsg"));
		
		return true;
	}
	return false;
}

bool	Censor_AttackMe::Check(const DamageParam& param)
{
	for (auto it = m_reaction_TargetList.begin();
		it != m_reaction_TargetList.end();
		++it)
	{
		//�d���`�F�b�N
		if ((*it)->damage_Param.parent == param.parent)
			return false;
	}
	return true;
}

void	Censor_AttackMe::Register(NPC_Brain* brain, const DamageParam* param)
{
	//���E�O����̍U���Ȃ̂����`�F�b�N����
	float reliability;

	if (param->vec.Length() <= 0.001)
	{
		//MyAssert(false, "�OVector");
		return;
	}
	Character_Record* mem = brain->Get_CharacterMemory(param->parent,&reliability);
	MEMORY*const memory = &brain->GetMemory();
	if (!mem)
	{
		 if (!memory->Is_Record_Threat_OutSight(param->parent))
		 {
			 m_reaction_TargetList.push_back(new Msg_AttackMe(*param));
		 }
		 return;
	}
	if (!mem->m_visibilily)
	{
		if (!memory->Is_Record_Threat_OutSight(param->parent))
		{
			if (Check(*param))
				m_reaction_TargetList.push_back(new Msg_AttackMe(*param));
			return;
		}
	}
}

void	Censor_AttackMe::Update_Counter(NPC_Brain* brain, Character* character)
{
	if (m_reaction_TargetList.empty())
		return;

	for (auto it = m_reaction_TargetList.begin(); it != m_reaction_TargetList.end();)
	{
		(*it)->Update();
		if ((*it)->m_current_Frame >= m_reaction_Speed)
		{
			Judge_Dispatch_PlanningMsg(brain, character, (*it));
			SAFE_DELETE((*it));
			it = m_reaction_TargetList.erase(it);
			continue;
		}
		it++;
	}
}

void	Censor_AttackMe::Update(NPC_Brain* brain, Character* character)
{
	Update_Counter(brain, character);
}


