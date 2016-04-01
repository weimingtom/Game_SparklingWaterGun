#ifndef __PATH_SEARCHALG_H__
#define __PATH_SEARCHALG_H__
#include	 "Template_Graph.h"

class Action_Node;
class Action_Edge;

class ActionSearch : public PathSearch_Base<Action_Node, Action_Edge>
{
public:
	bool	Set_GoalNode(Action_Node* node);
	bool	Is_Goal(Action_Node* node);
	float	Calculation_Score(Action_Node*node,
		Action_Edge* edge, const float& score);
};

class Dijkstra_Standard : public PathSearch_Base<Field_Node,Field_Edge>
{
public :
	bool	Set_GoalNode(Field_Node* node);
	bool	Is_Goal(Field_Node* node);
	float	Calculation_Score(Field_Node*node,
		Field_Edge* edge, const float& score);
};

class Astar_Standard : public PathSearch_Base<Field_Node, Field_Edge>
{
public :
	bool	Set_GoalNode(Field_Node* node);
	bool	Is_Goal(Field_Node* node);
	float	Calculation_Score(Field_Node*node,
		Field_Edge* edge, const float& score);
};

#endif
