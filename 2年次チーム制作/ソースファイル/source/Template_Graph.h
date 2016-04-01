#ifndef __TEMPLATE_GRAPH_H__
#define __TEMPLATE_GRAPH_H__

#include	<list>
#include	<vector>
#include	<algorithm>
#include	"Console.h"

template<class T, class E>
class PathSearch_Base;

namespace node_status
{
	enum node_s
	{
		NONE,
		OPEN,
		CLOSE,
	};
};
typedef node_status::node_s NODESTATUS;

//ノードとエッジのインターフェース
class Edge_Base;
class Node_Base
{
public:
	int						index;
	float					cost;		//エッジ間のみのコスト
	float					score;		//heuristic込のスコア情報
	Node_Base*		parent;	//親
	NODESTATUS	status;
	std::list<Edge_Base*> edge_List;
	Node_Base(){ index = -1; cost = .0f; score = .0; parent = nullptr; status = node_status::NONE; }
	virtual void	Release()
	{
		for (auto it = edge_List.begin(); it != edge_List.end(); it++)
		{
			delete (*it);
		}
		edge_List.clear();
	}

	virtual void	Reset()
	{
		cost = .0f; score = .0f; parent = nullptr; status = node_status::NONE;
	}

	void Release_Edge()
	{
		//cost = .0f; score = .0f; parent = nullptr; status = node_status::NONE;
		//Edgeを削除
		for (auto it = edge_List.begin(); it != edge_List.end(); it++)
		{
			delete (*it);
		}
		edge_List.clear();

	}

	void	Insert_Edge(Edge_Base* new_Edge)
	{
		edge_List.push_back(new_Edge);
	}

	void	Erase_Edge(Edge_Base* erase_Edge)
	{
		for (auto it = edge_List.begin(); it != edge_List.end(); it++)
		{
			if ((*it) == erase_Edge)
			{
				//it = edge_List.erase(std::remove_if(edge_List.begin(), edge_List.end(), it));
				it = edge_List.erase(it);
				break;
			}
		}
	}

};

//そのうちExtraCost計算用の関数ポインタみたいなのをつくるべきかも？
class Edge_Base
{
public:
	Node_Base* from;
	Node_Base* to;
	float				default_Cost;		//二点間の距離とか単純なやつ
	Edge_Base(){}
	Edge_Base(Node_Base* f, Node_Base* t)
	{
		from = f; to = t;
	}
	void	Set_DefaultCost(const float& c)
	{
		default_Cost = c;
	}

	void	Seting_Route(Node_Base* from, Node_Base* to)
	{
		this->from = from;	this->to = to;
	}
};

template<class Node_Type, class Edge_Type>
class Graph
{
protected:
	int				index_Counter;	//index割り当て用
	std::vector<Node_Type*>	node_List;
	int				num_Edge;
	int				num_Node;
protected :
	//できれば一定数の関数隠したい
public:
	Graph(){ node_List.clear(); index_Counter = 0; num_Edge = 0; num_Node = 0; }
	virtual ~Graph(){}

	void	All_Release()
	{
		for (auto it = node_List.begin(); it != node_List.end(); it++)
		{
			(*it)->Release();
			delete (*it);
		}
		node_List.clear();
		num_Node = 0;
	}
	void	Reset_Index(){ index_Counter = 0; }
	void	Insert_Edge(Node_Type* node, Edge_Type* insert_Edge)
	{
		node->Insert_Edge(insert_Edge);
		num_Edge++;
	}

	virtual void	All_Reset()
	{
		for (auto it = node_List.begin(); it != node_List.end(); it++)
		{
			(*it)->Reset();
		}
	}
	void	Add_Node(Node_Type* new_Node)
	{
		if (new_Node->index <0)
			new_Node->index = index_Counter;
		index_Counter++;
		num_Node++;
		node_List.push_back(new_Node);
	}

	bool	Erase_Node(Node_Type* erase_Node)
	{
		auto it = std::find(node_List.begin(), node_List.end(), erase_Node);
		if (it == node_List.end())
		{
			return false;
		}

		//接続関係を消去(あやしい)
		for (auto co = erase_Node->edge_List.begin();
			co != erase_Node->edge_List.end(); co++)
		{
			for (auto work = (*co)->to->edge_List.begin();
				work != (*co)->to->edge_List.end(); work++)
			{
				if ((*work)->to == erase_Node)
				{
					(*work)->from->Erase_Edge((*work));
					break;
				}
			}
		}
		//ノードを削除
		delete erase_Node;
		//node_List.erase(std::remove_if(node_List.begin(), node_List.end(), erase_Node),node_List.end());
		node_List.erase(it);
		num_Node--;
	}
	/**
	*@brief 経路探索
	*@param１　スタートノード
	*@param2		コスト計算とゴール判定クラスのポインタ
	*/
	bool	Search(Node_Type* start_Node,
		PathSearch_Base<Node_Type, Edge_Type>* search_Alg)
	{
		//エラーチェック
		if (node_List.empty())
			return false;
		auto it = std::find(node_List.begin(), node_List.end(), start_Node);
		if (it == node_List.end())
			return false;

		//探索待ちリスト作成
		std::list<Node_Type*> open_List;
		open_List.clear();

		//ラムダ式作成
		auto Open = [](Node_Type* insert, std::list<Node_Type*>* list)
		{
			insert->status = node_status::OPEN;
			list->push_back(insert);
		};

		auto Close = [](Node_Type* erase_Node, std::list<Node_Type*>* list)
		{
			auto it = std::find(list->begin(), list->end(), erase_Node);
			erase_Node->status = node_status::CLOSE;
			//list->erase(std::remove_if(list->begin(), list->end(), erase_Node),list->end());
			list->erase(it);
		};
		Open(start_Node, &open_List);

		//一番スコアの低いノードを取り出す
		for (; !open_List.empty();)
		{
			Node_Type* N;
			float score = FLT_MAX;
			for (auto it = open_List.begin(); it != open_List.end(); it++)
			{
				if (score > (*it)->score)
				{
					score = (*it)->score;
					N = *it;
				}
			}
			Close(N, &open_List);

			for (auto it = N->edge_List.begin(); it != N->edge_List.end(); it++)
			{
				Node_Type* connect = reinterpret_cast<Node_Type*>((*it)->to);
				//スコア計算（search_Algに全部任せる）
				float score = search_Alg->Calculation_Score(connect, reinterpret_cast<Edge_Type*>(*it), N->score);

				//比較していく
				if (connect->status == node_status::NONE)
				{
					connect->cost = N->cost + (*it)->default_Cost;
					connect->score = score;
					connect->parent = N;
					Open(connect, &open_List);
					continue;
				}

				if (connect->status == node_status::OPEN)
				{
					if (connect->score > score)
					{
						connect->cost = N->cost + (*it)->default_Cost;
						connect->score = score;
						connect->parent = N;
						Open(connect, &open_List);
						continue;
					}
				}

				if (connect->status == node_status::CLOSE)
				{
					if (connect->score > score)
					{
						connect->cost = N->cost + (*it)->default_Cost;
						connect->score = score;
						connect->parent = N;
						Open(connect, &open_List);
						continue;
					}
				}
			}
			//ゴール判定
			if (search_Alg->Is_Goal(N))
			{
				//ゴールノード確定
				search_Alg->dicision_Goal = N;
				console::Print("経路探索成功\nGoalNodeNo :");
				console::Print(N->index);
				console::Print("StartNodeNo : ");
				console::Print(start_Node->index);

				return true;
			}
		}
		//経路が見つからない場合
		All_Reset();
		CONSOLE_PRINT("経路探索に失敗しました:StartPos %5f,%5f,%5f\n", 10);
		
		return false;
	}

	/**
	*@breif ノード検索
	*@param ノードのID
	*/
	Node_Type* Find_Node(const int& id)
	{
		for (auto it = node_List.begin(); it != node_List.end(); it++)
		{
			if (id == (*it)->index)
				return (*it);
		}
		return nullptr;
	}
	/**
	*@breif 最短経路までのエッジをリストに挿入する
	*@param1 ゴールノード
	*@param2 スタートノード
	*@param3 格納するstd::listのポインタ
	*/
	bool	Set_Route(Node_Type* goal_Node, Node_Type* start_Node, std::list<Edge_Type*>* route_Edge)
	{
		Node_Type* work = goal_Node;
		for (; work != start_Node;)
		{
			Node_Type* parent = reinterpret_cast<Node_Type*>(work->parent);
			//親がtoになっているEdgeを検索する
			for (auto it = work->edge_List.begin(); it != work->edge_List.end(); it++)
			{
				if ((*it)->to == parent)
				{
					route_Edge->push_front(reinterpret_cast<Edge_Type*>(*it));
					work = parent;
					break;
				}
			}
		}
		return true;
	}
	//できればテンプレートにしたい
	//bool	Set_Route(Node_Type* goal_Node, Node_Type* start_Node, std::stack<Edge_Type*>* route_Edge)
	//{
	//	Node_Type* work = goal_Node;
	//	for (; work != start_Node;)
	//	{			Node_Type* parent = reinterpret_cast<Node_Type*>(work->parent);

	//		//親がtoになっているEdgeを検索する
	//		for (auto it = work->edge_List.begin(); it != work->edge_List.end(); it++)
	//		{
	//			if ((*it)->to == parent)
	//			{
	//				route_Edge->push(reinterpret_cast<Edge_Type*>(*it));
	//				work = parent;
	//				break;
	//			}
	//		}
	//	}
	//	return true;
	//}

	int		Get_GraphSize(){ return node_List.size(); }
};

template<class Node_Type, class Edge_Type>
class PathSearch_Base
{
protected:
	std::list<Node_Type*> goal_List;				//ゴール候補のリスト
public:
	Node_Type*						dicision_Goal;	//経路探索後のゴールノード
	Node_Type*						start_Node;		//スタートノード
	PathSearch_Base() : dicision_Goal(nullptr),start_Node(nullptr){ goal_List.clear(); }
	void	Clear(){ goal_List.clear(); }

	bool	Set_StartNode(Node_Type* insert)
	{
		if (!start_Node)
		{
			start_Node = insert;
			return true;
		}
		return false;
	}
	//Node_Type* Get_StartNode()const
	//{ return start_Node; }
	/**
	*@breif	ゴールノードを追加する
	*@param1 ゴールノード
	*@return 成功でtrue
	*@note　Astarの場合はゴールは一つ
	*/
	virtual bool	Set_GoalNode(Node_Type* node) = 0;
	/**
	*@brief ゴール判定
	*@param 判定したいノード
	*@return trueでゴール
	*/
	virtual bool	Is_Goal(Node_Type* node) = 0;
	/**
	*@breif ノードのコスト計算
	*@param1 計算したいノード
	*@param2 そのノードに接続されているエッジ
	*@param3 そのノードまでのscore
	*/
	virtual float	Calculation_Score(Node_Type*node,
		Edge_Type* edge, const float& score) = 0;

};

#endif
