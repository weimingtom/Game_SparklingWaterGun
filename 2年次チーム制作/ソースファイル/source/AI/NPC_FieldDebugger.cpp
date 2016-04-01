#include	"..\All.h"

#define CONNECT 1
#define CIRCLE 0

GoalPoint::GoalPoint(const ENTITY_ID& id, const DWORD& color) :
holder(id), character_Color(color), node(nullptr)
{
}

void	GoalPoint::Render(iex2DObj* tex,const float& width,const float& height )
{
	float t[2] = { .0f, 1.0f };
	if (!node) return;
	Vector3 pos = node->pos;
	pos.y += (height / 2);
	Billboard::Draw3D(node->pos, tex, width, height, t, t, RS_COPY);
}

NodeConnecter::NodeConnecter(const ENTITY_ID& id, const DWORD& color) :
holder(id), character_Color(color)
{
	route_Edge.clear();
}

void	NodeConnecter::Set_Route(std::list<Field_Edge*> w_stack)
{
	if (!route_Edge.empty())
		route_Edge.clear();

	for (; !w_stack.empty();)
	{
		route_Edge.push_front(w_stack.front());
		w_stack.pop_front();
	}
}

bool	NodeConnecter::Erase_Edge(Field_Edge* edge)
{
	for (auto cur_Edge = route_Edge.begin(); cur_Edge != route_Edge.end(); cur_Edge++)
	{
		if ((*cur_Edge) == edge)
		{
			route_Edge.erase(cur_Edge);
			return true;
		}
	}
	return false;
}

void	NodeConnecter::Render()
{
	
	for (auto it = route_Edge.begin(); it != route_Edge.end();it++)
	{

		Field_Node* from = dynamic_cast<Field_Node*>((*it)->from);
		Vector3 f_Pos = from->pos;
		f_Pos.y += .3f;
		it++;
		if (it == route_Edge.end())
		{
			it--;
		}
		Field_Node* to = dynamic_cast<Field_Node*>((*it)->from);
		Vector3 t_Pos = to->pos;
		t_Pos.y += .3f;
		DrawLine::Render(f_Pos, t_Pos, character_Color);
	}
}



Field_Debugger*	Field_Debugger::instance;

void	Field_Debugger::Initialize(const int&	num_COM, const float& width, const float& height)
{
	node_TexHeight = height;
	node_TexWidth = width;
	num_NPC = num_COM;
	goal_List.reserve(num_NPC);
	edge_Renderer.reserve(num_NPC);
	node_Tex = new iex2DObj("DATA\\NPC\\arrow.PNG");
}

Field_Debugger::~Field_Debugger()
{
	for (auto it = goal_List.begin(); it != goal_List.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	for (auto it = edge_Renderer.begin(); it != edge_Renderer.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	SAFE_DELETE(node_Tex);
}

bool	Field_Debugger::Register(const ENTITY_ID& id, const DWORD& color)
{
	//重複チェック
	for (auto it = goal_List.begin(); it != goal_List.end(); it++)
	{
		if (id == (*it)->holder)
			return false;
	}
	
	goal_List.push_back(new GoalPoint(id, color));
	edge_Renderer.push_back(new NodeConnecter(id, color));
	return true;
}

void	Field_Debugger::Render_GoalNode()
{
	for (auto it = goal_List.begin(); it != goal_List.end(); it++)
	{
		(*it)->Render(node_Tex, node_TexWidth, node_TexHeight);
	}
}

bool	Field_Debugger::Send_Goal(const ENTITY_ID& entity, Field_Node* node)
{
	for (auto it = goal_List.begin(); it != goal_List.end(); it++)
	{
		if (entity == (*it)->holder)
		{
			(*it)->node = node;
			return true;
		}
	}
	return false;
}

bool	Field_Debugger::Send_Route(const ENTITY_ID& entity, const std::list<Field_Edge*>& route)
{
	for (auto it = edge_Renderer.begin(); it != edge_Renderer.end(); it++)
	{
		if ((*it)->holder == entity)
		{
			(*it)->Set_Route(route);
			return true;
		}
	}
	return false;
}

void	Field_Debugger::Reder_GoalRoute(const ENTITY_ID& id)
{
	for (auto it = edge_Renderer.begin(); it != edge_Renderer.end(); it++)
	{
		if (id == (*it)->holder || id == game_id::id_error)
			(*it)->Render();
	}
}

void	Field_Debugger::Render_AllEdge()
{
	int counter = 0;
	for (auto it = navigation_system->m_node_List.begin(); it != navigation_system->m_node_List.end(); it++)
	{
		counter++;
		if (counter > 3000) break;
		for (auto co = (*it)->edge_List.begin();
			co != (*it)->edge_List.end(); co++)
		{
			Field_Node* from = dynamic_cast<Field_Node*>((*co)->from);
			Field_Node* to = dynamic_cast<Field_Node*>((*co)->to);

			DrawLine::Render(from->pos, to->pos);
		}
	}
}

bool	Field_Debugger::Erase_Debug_Edge(const ENTITY_ID& holder_ID, Field_Edge* erase_Edge)
{
	for (auto cur_Chara = edge_Renderer.begin(); cur_Chara != edge_Renderer.end(); cur_Chara++)
	{
		if ((*cur_Chara)->holder == holder_ID)
		{
			return (*cur_Chara)->Erase_Edge(erase_Edge);
		}
	}
	return false;
}

void	Field_Debugger::Render_Edge(const Vector3& pos)
{
	float tu[2] = { .0f, 1.0f };
#if CONNECT
	static bool flg = false;
	static int	rate = 40;

	Field_Node* render_node = navigation_system->Find_Node_Pos(pos);
	
	Billboard::Draw3D(render_node->pos, node_Tex, .15f, .15f, tu, tu, RS_COPY);
	if (render_node)
	{
		MyDebugMgrStr(dword_color::yellow, "index%d", render_node->index);

		for (auto co = render_node->edge_List.begin();
			co != render_node->edge_List.end(); co++)
		{
			Field_Node* from = dynamic_cast<Field_Node*>((*co)->from);
			Field_Node* to = dynamic_cast<Field_Node*>((*co)->to);

			Field_Edge* e = (Field_Edge*)((*co));
			DWORD color = dword_color::white;
			float height = fabs(from->pos.y - to->pos.y);
			if (height > 1.0f)
			{
				if (GAMEMATCHINFO.GetMatchTime() % rate == 0)
					flg ^= 0x01;
				if (flg)
					DrawLine::Render(from->pos, to->pos, color);
			}
			else
				DrawLine::Render(from->pos, to->pos, color);
			
			Billboard::Draw3D(to->pos, node_Tex, .15f, .15f, tu, tu, RS_COPY);
		}
	}
#endif

#if CIRCLE
	const WayPointInformation* info = field_Info->Get_NodeInformation(pos);
	
	for (auto node : info->m_even_with_Circle)
	{
		Billboard::Draw3D(node->pos, node_Tex, .15f, .15f, tu, tu, RS_COPY);
	}
#endif
}

void Field_Debugger::Render_Division(const Vector3& pos)
{
	std::vector<Field_Node*> work_list;
	work_list.clear();
	float tu[2] = { .0f, 1.0f };
	navigation_system->Find_Node_Container(pos, nullptr, .0f, &work_list);
	//MyDebugMgrStr(dword_color::red, "空間ノードの数%d", work_list.size());
	for (auto it : work_list)
	{
		Billboard::Draw3D(it->pos, node_Tex, .2f, .2f, tu, tu, RS_COPY);
	}
}