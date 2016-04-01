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

//�m�[�h�ƃG�b�W�̃C���^�[�t�F�[�X
class Edge_Base;
class Node_Base
{
public:
	int						index;
	float					cost;		//�G�b�W�Ԃ݂̂̃R�X�g
	float					score;		//heuristic���̃X�R�A���
	Node_Base*		parent;	//�e
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
		//Edge���폜
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

//���̂���ExtraCost�v�Z�p�̊֐��|�C���^�݂����Ȃ̂�����ׂ������H
class Edge_Base
{
public:
	Node_Base* from;
	Node_Base* to;
	float				default_Cost;		//��_�Ԃ̋����Ƃ��P���Ȃ��
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
	int				index_Counter;	//index���蓖�ėp
	std::vector<Node_Type*>	node_List;
	int				num_Edge;
	int				num_Node;
protected :
	//�ł���Έ�萔�̊֐��B������
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

		//�ڑ��֌W������(���₵��)
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
		//�m�[�h���폜
		delete erase_Node;
		//node_List.erase(std::remove_if(node_List.begin(), node_List.end(), erase_Node),node_List.end());
		node_List.erase(it);
		num_Node--;
	}
	/**
	*@brief �o�H�T��
	*@param�P�@�X�^�[�g�m�[�h
	*@param2		�R�X�g�v�Z�ƃS�[������N���X�̃|�C���^
	*/
	bool	Search(Node_Type* start_Node,
		PathSearch_Base<Node_Type, Edge_Type>* search_Alg)
	{
		//�G���[�`�F�b�N
		if (node_List.empty())
			return false;
		auto it = std::find(node_List.begin(), node_List.end(), start_Node);
		if (it == node_List.end())
			return false;

		//�T���҂����X�g�쐬
		std::list<Node_Type*> open_List;
		open_List.clear();

		//�����_���쐬
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

		//��ԃX�R�A�̒Ⴂ�m�[�h�����o��
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
				//�X�R�A�v�Z�isearch_Alg�ɑS���C����j
				float score = search_Alg->Calculation_Score(connect, reinterpret_cast<Edge_Type*>(*it), N->score);

				//��r���Ă���
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
			//�S�[������
			if (search_Alg->Is_Goal(N))
			{
				//�S�[���m�[�h�m��
				search_Alg->dicision_Goal = N;
				console::Print("�o�H�T������\nGoalNodeNo :");
				console::Print(N->index);
				console::Print("StartNodeNo : ");
				console::Print(start_Node->index);

				return true;
			}
		}
		//�o�H��������Ȃ��ꍇ
		All_Reset();
		CONSOLE_PRINT("�o�H�T���Ɏ��s���܂���:StartPos %5f,%5f,%5f\n", 10);
		
		return false;
	}

	/**
	*@breif �m�[�h����
	*@param �m�[�h��ID
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
	*@breif �ŒZ�o�H�܂ł̃G�b�W�����X�g�ɑ}������
	*@param1 �S�[���m�[�h
	*@param2 �X�^�[�g�m�[�h
	*@param3 �i�[����std::list�̃|�C���^
	*/
	bool	Set_Route(Node_Type* goal_Node, Node_Type* start_Node, std::list<Edge_Type*>* route_Edge)
	{
		Node_Type* work = goal_Node;
		for (; work != start_Node;)
		{
			Node_Type* parent = reinterpret_cast<Node_Type*>(work->parent);
			//�e��to�ɂȂ��Ă���Edge����������
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
	//�ł���΃e���v���[�g�ɂ�����
	//bool	Set_Route(Node_Type* goal_Node, Node_Type* start_Node, std::stack<Edge_Type*>* route_Edge)
	//{
	//	Node_Type* work = goal_Node;
	//	for (; work != start_Node;)
	//	{			Node_Type* parent = reinterpret_cast<Node_Type*>(work->parent);

	//		//�e��to�ɂȂ��Ă���Edge����������
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
	std::list<Node_Type*> goal_List;				//�S�[�����̃��X�g
public:
	Node_Type*						dicision_Goal;	//�o�H�T����̃S�[���m�[�h
	Node_Type*						start_Node;		//�X�^�[�g�m�[�h
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
	*@breif	�S�[���m�[�h��ǉ�����
	*@param1 �S�[���m�[�h
	*@return ������true
	*@note�@Astar�̏ꍇ�̓S�[���͈��
	*/
	virtual bool	Set_GoalNode(Node_Type* node) = 0;
	/**
	*@brief �S�[������
	*@param ���肵�����m�[�h
	*@return true�ŃS�[��
	*/
	virtual bool	Is_Goal(Node_Type* node) = 0;
	/**
	*@breif �m�[�h�̃R�X�g�v�Z
	*@param1 �v�Z�������m�[�h
	*@param2 ���̃m�[�h�ɐڑ�����Ă���G�b�W
	*@param3 ���̃m�[�h�܂ł�score
	*/
	virtual float	Calculation_Score(Node_Type*node,
		Edge_Type* edge, const float& score) = 0;

};

#endif
