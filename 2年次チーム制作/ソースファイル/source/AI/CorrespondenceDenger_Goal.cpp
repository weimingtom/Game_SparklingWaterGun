#include	"..\All.h"

#pragma region Goal

Goal_CorrespondenceDenger::Goal_CorrespondenceDenger(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director) :
Goal(brain, "CorrespondenceDenger", evo, goal_tag::correspondecne_denger,use_director)
{
	m_behavior_Director->Set_Parent(this);

}

bool	Goal_CorrespondenceDenger::Active(Character* character)
{
	//������ׂ��Ɣ��f���ꂽ�^�[�Q�b�g��ID���擾
	Accesor_Brain::Set_Current_Target(m_holder,
		Accesor_Brain::Final_GoalEvalutor(m_holder)->Get_TargetID());
	//m_holder->Set_CurrentTarget(Accesor_Brain::Final_GoalEvalutor(m_holder)->Get_TargetID());
	//�G���[�`�F�b�N
	MEMORY* const  memory = &m_holder->GetMemory();
	Threat_OutSight* target = memory->Get_Threat_OutSight(Accesor_Brain::Current_Target(m_holder));
	if (!target)
		return false;
	//current_Behavior = b_num::none;
	m_behavior_Director->Direction(m_holder, character, true);
	return true;
}

goal_status	Goal_CorrespondenceDenger::Process(Character* character)
{
	return Accesor_Brain::BehaviorManager(m_holder)->Execute_Current_Behavior(character);
	//	return goal_status::completed;
	//
	//return goal_status::active;
}

void	Goal_CorrespondenceDenger::Completed(Character* character)
{
	//�L������폜����
	MEMORY* const memory = &m_holder->GetMemory();
	memory->Erase_Threar_OutSight(Accesor_Brain::Current_Target(m_holder));
	//if (memory->Erase_Threar_OutSight(m_holder->Get_CurrentTarget()))
	//{
	//	MessageBoxPlus("�폜����");
	//}
	//else
	//	MessageBoxPlus("�폜���s");

}

void	Goal_CorrespondenceDenger::Failed(Character* character)
{
	//�L������폜����
	MEMORY* const memory = &m_holder->GetMemory();
	memory->Erase_Threar_OutSight(Accesor_Brain::Current_Target(m_holder));
	//if (memory->Erase_Threar_OutSight(m_holder->Get_CurrentTarget()))
	//{
	//	MessageBoxPlus("�폜����");
	//}
	//else
	//	MessageBoxPlus("�폜���s");
}

void	Goal_CorrespondenceDenger::Initialize_FuzzyModule()
{

}

bool	Goal_CorrespondenceDenger::HandleMessage(AI_MESSAGE* msg)
{
	switch (msg->type)
	{
	case ai_messagetype::evalute_currentGoalPos:
		return Evalute_CurrentGoalPos(static_cast<R_Targetwrapping*>(msg->extra_Info));
	default:
		break;
	}
	return false;
}

bool	Goal_CorrespondenceDenger::Evalute_CurrentGoalPos(R_Targetwrapping* object)
{
	//���݂̃S�[�����W��������ꏊ�Ƃ��čœK���ǂ����𔻒f����i���̂����������f�p�̃N���X�����j
	return true;
}

#pragma endregion

#pragma region Evalutor

goal_status Behavior_Director_CorrespondenceDenger::Activate(NPC_Brain* brain, Character* character)
{
	Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::look_back,
		m_holder, nullptr);
	MEMORY* const  memory = &brain->GetMemory();
	Threat_OutSight* target = memory->Get_Threat_OutSight(Accesor_Brain::Current_Target(brain));
	MyAssert(target, "Active�ŃG���[�`�F�b�N���Ă�͂��Ȃ̂ɂ���͂�΂�");
	Vector3 target_vec = target->m_vec;
	//�U���������x�N�g�����]
	target_vec *= -1.0f;
	ai_message->Send_BehaviorMessage(brain, character, ai_messagetype::set_LookVec, &target_vec);
	return goal_status::active;
}

goal_status Behavior_Director_CorrespondenceDenger::Direction(NPC_Brain* brain, Character* character,
																										bool init)
{
	if (init)
		return Activate(brain,character);
	return goal_status::completed;
}

#pragma endregion
