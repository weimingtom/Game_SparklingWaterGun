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
	m_holder->Log(FormatString("生成されたゴール列\r\n"));
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
	//ゴールを設定
	m_search_Alg.Set_GoalNode(final_Goal);
	dummy_Node->Reset();
	dummy_Node->Release_Edge();
	Add_Node(final_Goal);
	Chain_Goal(symbol,character);
	//見つからなかった場合は再度プランニングする
	if (!Search(dummy_Node, &m_search_Alg))
	{
		int i = 0;
	}
	Settting_Goal(goal_Stack);
	All_Reset();

	/*探索後はグラフを空に*/
	//ダミーノードを取り出しておく
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
	MyAssert(!m_node_List.empty(), "ノードリストが空");
#endif
	//イテレータだとノードを追加した時にイテレータが消失したので（Listはいけたvectorはなんか無理だった）
	//配列要素に直接アクセスする
	//for (auto N = node_List.begin(); N != node_List.end(); N++)
	for (int n = 0; n < m_num_Node; n++)
	{
		planningkey work_Key = m_node_List[n]->goal->Prerequisite(symbol);
		if (work_Key == planningkey::error)
		{
			MessageBoxPlus("PlanningError\n	%s", m_node_List[n]->goal->goal_Name);
			return false;
		}
		//キーが一致した場合はダミーと接続
		if (work_Key == planningkey::OK)
		{
			//とりあえずテスト
			float cost = goalcost_calculater::Test(m_node_List[n]->goal, m_holder,character, 2.0f);
			Action_Edge* insert = new Action_Edge();
			insert->Seting_Route(dummy_Node, (m_node_List[n]));
			insert->Set_DefaultCost(cost);
			dummy_Node->Insert_Edge(insert);
		}
		//一致しなかった場合はmidlleGoallistから効果と前提条件が一致するものを探す
		for (auto search_Goal = m_midlle_Goallist.begin();
			search_Goal != m_midlle_Goallist.end(); search_Goal++)
		{
			//エフェクト取得
			planningkey effect = (*search_Goal)->Effect(symbol);
			//一致した場合は接続
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
	//ダミーノードをセット
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