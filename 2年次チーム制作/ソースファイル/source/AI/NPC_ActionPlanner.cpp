#include	"..\All.h"

#define CONSOLE_ACTIONPLANNER 1

Action_Planner::Action_Planner(NPC_Brain* b,char* cost_Filename) : Graph()
{
	dummy_Node = new Action_Node();
	m_holder = b;
	//Setting_MidlleGoalList();
	Load_GoalCost(cost_Filename);
}

void	Action_Planner::Setting_MidlleGoalList()
{

}

void	Action_Planner::Load_GoalCost(char* cost_Filename)
{
	textLoader loader(cost_Filename);
	
	char work[64];
	for (int y = 0; y < static_cast<int>(goal_tag::NumGoal); y++)
	{
		loader._Loadstring(work);
		for (int x = 0; x < static_cast<int>(goal_tag::NumGoal); x++)
		{
			m_default_CostList[y][x] =loader.LoadFloat();
		}
	}
}

Action_Planner::~Action_Planner()
{
	if (!m_node_List.empty())
	{
		All_Release();
	}
	for (std::vector<Goal*>::iterator it = m_midlle_Goallist.begin();
		it != m_midlle_Goallist.end();
		++it)
	{
		SAFE_DELETE((*it));
	}
	m_midlle_Goallist.clear();
	delete dummy_Node;
	//SAFE_DELETE(dummy_Node);
}

bool	Action_Planner::Settting_Goal(std::deque<Goal*>* goal_Stack)
{
	m_holder->Log(FormatString("�������ꂽ�S�[����\r\n"));
	Action_Node* S = m_node_List.front();
	Action_Node* G = dummy_Node;
	Action_Node* work = S;
	for (;;)
	{
		if (work == G)
			break;
		goal_Stack->push_front(work->goal);
#if CONSOLE_ACTIONPLANNER
		CONSOLE_PRINT("%s\n",work->goal->goal_Name);
		//console::Print<char*>(work->goal->goal_Name);
#endif
		m_holder->Log(FormatString("%s", work->goal->goal_Name));

		work = static_cast<Action_Node*>(work->parent);
	}
	return true;
}

bool	Action_Planner::Plannning(const Symbol& symbol, Character* character, std::deque<Goal*>* goal_Stack, Action_Node* final_Goal)
{
	//�S�[����ݒ�
	m_search_Alg.Set_GoalNode(final_Goal);
	dummy_Node->Reset();
	dummy_Node->Release_Edge();
	Add_Node(final_Goal);
	Chain_Goal(symbol,character);
	//������Ȃ������ꍇ�͍ēx�v�����j���O����
	if (!Search(dummy_Node, &m_search_Alg))
	{
		int i = 0;
	}
	Settting_Goal(goal_Stack);
	All_Reset();

	/*�T����̓O���t�����*/
	//�_�~�[�m�[�h�����o���Ă���
	auto it = std::find(m_node_List.begin(), m_node_List.end(), dummy_Node);
	if (it != m_node_List.end())
	{
		m_node_List.erase(it);
	}
	All_Release();
	dummy_Node->Release();
	m_search_Alg.Clear();

	return false;
}

bool	Action_Planner::Chain_Goal(const Symbol& symbol,Character* character)
{
#if NPC_ASSART
	MyAssert(!m_node_List.empty(), "�m�[�h���X�g����");
#endif
	//�C�e���[�^���ƃm�[�h��ǉ��������ɃC�e���[�^�����������̂ŁiList�͂�����vector�͂Ȃ񂩖����������j
	//�z��v�f�ɒ��ڃA�N�Z�X����
	//for (auto N = node_List.begin(); N != node_List.end(); N++)
	for (int n = 0; n < m_num_Node; n++)
	{
		planningkey work_Key = m_node_List[n]->goal->Prerequisite(symbol);
		if (work_Key == planningkey::error)
		{
			MessageBoxPlus("PlanningError\n	%s", m_node_List[n]->goal->goal_Name);
			return false;
		}
		//�L�[����v�����ꍇ�̓_�~�[�Ɛڑ�
		if (work_Key == planningkey::OK)
		{
			//�Ƃ肠�����e�X�g
			float cost = goalcost_calculater::Test(m_node_List[n]->goal, m_holder,character, 2.0f);
			Action_Edge* insert = new Action_Edge();
			insert->Seting_Route(dummy_Node, (m_node_List[n]));
			insert->Set_DefaultCost(cost);
			dummy_Node->Insert_Edge(insert);
		}
		//��v���Ȃ������ꍇ��midlleGoallist������ʂƑO���������v������̂�T��
		for (auto search_Goal = m_midlle_Goallist.begin();
			search_Goal != m_midlle_Goallist.end(); search_Goal++)
		{
			//�G�t�F�N�g�擾
			planningkey effect = (*search_Goal)->Effect(symbol);
			//��v�����ꍇ�͐ڑ�
			if (effect == work_Key)
			{
				float cost = (*search_Goal)->m_cost_Calculater(m_node_List[n]->goal,m_holder,character,
					m_default_CostList[static_cast<int>(m_node_List[n]->goal->m_tag)][static_cast<int>((*search_Goal)->m_tag)]);
				
				Action_Node* work = new Action_Node();
				 work->goal = (*search_Goal);
				 Action_Edge* insert = new Action_Edge();
				 insert->Seting_Route(work, m_node_List[n]);
				 insert->  Set_DefaultCost(cost);
				 work->Insert_Edge(insert);
				 Add_Node(work);
			}
		}
	}
	//�_�~�[�m�[�h���Z�b�g
	Add_Node(dummy_Node);
	return true;
}


void	Action_Planner::Clear_GoalStatus()
{
	for (auto it : m_midlle_Goallist)
	{
		it->status = goal_status::inactive;
	}
}