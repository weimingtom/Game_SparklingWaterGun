#include	"..\All.h"

PathPlanner::PathPlanner(NPC_Brain* holder) :m_holder(holder),m_active(false),
m_request(false), m_search_Interval(-1), m_parent_goal(), m_progress_Frame(0)
{
	
}

PathPlanner::~PathPlanner()
{

}

bool	PathPlanner::Evalute_Current_GoalPos(Character* character)
{
	Vector3 target_Pos;
	//�^�[�Q�b�g�����F�ł��Ă邩���m�F����
	if (Accesor_Brain::Evalutor(m_holder)->Target_InSight())
	{
		float reliability;
		if (IS_CONTROLLOBJ_ID(Accesor_Brain::Current_Target(m_holder)))
			target_Pos = m_holder->Get_CharacterMemory(Accesor_Brain::Current_Target(m_holder), &reliability)->m_sensed_Pos;
		else
			target_Pos = m_holder->Get_MapObjectMemory(Accesor_Brain::Current_Target(m_holder), &reliability)->m_sensed_Pos;
	}
	else
	{
		//���F�ł��Ȃ��ꍇ�͗\������
		float reliability;
		if (IS_CONTROLLOBJ_ID(Accesor_Brain::Current_Target(m_holder)))
		{
			Character_Record* target_record = m_holder->Get_CharacterMemory(
				Accesor_Brain::Current_Target(m_holder), &reliability);
			if (Accesor_Brain::Evalutor(m_holder)->Expectation_TargetPos_Character(target_record,
				&target_Pos, reliability))
				return false;
		}
		else
		{
			MapObject_Record* target_record = m_holder->Get_MapObjectMemory(
				Accesor_Brain::Current_Target(m_holder), &reliability);
			if (Accesor_Brain::Evalutor(m_holder)->Expectation_TargetPos_MapObejct(target_record,
				&target_Pos, reliability))
				return false;
		}
	}
	//�m�[�h�ɕϊ�
	Field_Node*	current_GoalNode = navigation_system->Find_Node_Pos(m_current_GoalPos);
	Field_Node*	target_Node = navigation_system->Find_Node_Pos(target_Pos);
	if (!current_GoalNode || !target_Node)
	{
#if NPC_ASSART
		MyAssert(false, "�m�[�h���Ȃ���");
#endif
		return false;
	}
	if (target_Node->index != current_GoalNode->index)
	{
		m_current_GoalPos = target_Pos;
		return true;
	}
	return false;
}

bool	PathPlanner::Activate(Character* character, const goal_tag& parent_goal, const Vector3& goal_pos, int search_interval)
{
	m_current_GoalPos = goal_pos;
	m_parent_goal = parent_goal;
	m_active = true;
	m_progress_Frame = 0;
	m_search_Interval = search_interval;
	return Request_PathSearch(character);
}

void	PathPlanner::Exit()
{
	m_parent_goal = goal_tag::error;
	m_active = false;
	m_progress_Frame = 0;
	m_search_Interval = -1;
}

bool	PathPlanner::Search(PathSearchObject*	search_Object)
{
	//�L�����N�^�[�̃|�C���^���L�����ǂ����`�F�b�N
	Character* search_Character = dynamic_cast<Character*>(IDMANAGER.GetPointer(search_Object->id));
	if (!search_Character)
		return false;

	//���݂̃S�[���Ɛ\�����̃S�[�����������ǂ���
	if (m_parent_goal != search_Object->m_request_Goaltag)
		return false;
	//���݂̐U�镑����MoveEdge���ǂ���
	if (Accesor_Brain::BehaviorManager(m_holder)->Get_Current_Tag() != behavior_tag::move_edge)
		return false;
	//�o�ߎ��Ԃ��`�F�b�N
	int current_Frame = GAMEMATCHINFO.GetMatchTime();
	if ((current_Frame - search_Object->request_Frame) >=
																						m_search_errorFrame)
	{
#if NPC_ASSART
		MyAssert(false, "��΂����ۂ�");
#endif
		return false;
	}
	//���W���d�����Ă��Ȃ������`�F�b�N
	Vector3 my_Pos = ControllObjFunc::GetPos(search_Character);
	Field_Node*	start_Node = navigation_system->Find_Node_Pos(my_Pos);
	Field_Node*	goal_Node = navigation_system->Find_Node_Pos(m_current_GoalPos);
	if (!start_Node || !goal_Node)
	{
#if NPC_ASSART
		MyAssert(false, "�m�[�h���Ȃ���");
#endif
		return false;
	}
	navigation_system->All_Reset();
	Astar_Standard search;
	search.Set_GoalNode(goal_Node);
	search.Set_StartNode(start_Node);
	//Move_Edge* current_Behavior = dynamic_cast<Move_Edge*>(Accesor_Brain::BehaviorManager(m_holder)->Get_Current_Behavior());
	if (navigation_system->Search(start_Node, (PathSearch_Base<Field_Node, Field_Edge>*)&search))
	{
		ai_message->Send_BehaviorMessage(m_holder, nullptr, ai_messagetype::set_route, &search);
		//navigation_system->Set_Route(goal_Node, start_Node, &current_Behavior->route_List);
		//current_Behavior->m_wait = false;
		return true;
	}
#if NPC_ASSART
	MyAssert(false, "�o�H�T�����s,startNode %d : GoalNode %d",start_Node->index,goal_Node->index);
#endif
	return false;
}

bool	PathPlanner::Request_PathSearch(Character* character)
{
	if (Accesor_Brain::BehaviorManager(m_holder)->Get_Current_Tag() == behavior_tag::move_edge)
	{
		ai_message->Demand_Search_Path(m_holder, character, m_parent_goal);
		ai_message->Send_BehaviorMessage(m_holder, character, ai_messagetype::wait, nullptr);
		return true;
	}
	return false;
}

void	PathPlanner::Update(Character* character)
{
	if (!m_active)
		return;
	
	m_progress_Frame++;
	if (m_search_Interval != m_not_Research)
	{
		if (m_progress_Frame % m_search_Interval)
		{
			if (Evalute_Current_GoalPos(character))
				Request_PathSearch(character);
		}
	}
}

bool	PathPlanner::HandleMessage(AI_MESSAGE*	msg)
{
	switch (msg->type)
	{
	case ai_messagetype::search_path_ok:
		Search(static_cast<PathSearchObject*>(msg->extra_Info));
		return true;
		break;

	default:
		MyAssert(false, "�����ɂ���̂͂��肦�Ȃ��͂�...");
		break;
	}
	return false;
}
