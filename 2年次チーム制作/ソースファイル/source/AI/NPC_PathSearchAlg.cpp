#include	"..\All.h"


//*******************************************************************************************************
//planning—p’Tõ
//*******************************************************************************************************
bool	ActionSearch::Set_GoalNode(Action_Node* node)
{
	goal_List.push_back(node);
	return true;
}

bool	ActionSearch::Is_Goal(Action_Node* node)
{
	for (auto it = goal_List.begin(); it != goal_List.end(); it++)
	{
		if (node == (*it))
			return true;
	}
	return false;
}

float	ActionSearch::Calculation_Score(Action_Node*node,
						Action_Edge* edge, const float& score)
{
	return edge->default_Cost +score;
}

//*******************************************************************************************************
//•’Ê‚ÌƒpƒXŒŸõ
//*******************************************************************************************************
bool	Dijkstra_Standard::Set_GoalNode(Field_Node* node)
{
	goal_List.push_back(node);
	return true;
}

bool	Dijkstra_Standard::Is_Goal(Field_Node* node)
{
	for (auto it = goal_List.begin(); it != goal_List.end(); it++)
	{
		if ((*it) == node)
			return true;
	}
	return false;
}

float	Dijkstra_Standard::Calculation_Score(Field_Node*node,
																	Field_Edge* edge, const float& score)
{
	float ret;
	if (edge)
	{
		ret = edge->default_Cost + score + node->visibility_Weight;
	}
	else
	{
		ret = edge->default_Cost + score;
	}
	return ret;
	
}

bool	Astar_Standard::Set_GoalNode(Field_Node* node)
{
	if (goal_List.empty())
	{
		goal_List.push_back(node);
		return true;
	}
	return false;
}

bool	Astar_Standard::Is_Goal(Field_Node* node)
{
	return (node == goal_List.front()) ? true : false;
}

float	Astar_Standard::Calculation_Score(Field_Node*node,
																			Field_Edge* edge, const float& score)
{
	float heuristic = (goal_List.front()->pos - node->pos).LengthSq();
	float ret;
	if (edge)
	{
		ret = score + edge->default_Cost + node->visibility_Weight + heuristic;
	}
	else
	{
		ret = score + heuristic;
	}
	return ret;
}