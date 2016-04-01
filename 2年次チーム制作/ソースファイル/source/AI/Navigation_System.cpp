#include	"..\All.h"

void	NavigationSystem::Initialize(char* node_filename, char* nodeinfo_filename)
{
	m_node_Tex = new iex2DObj("DATA\\NPC\\arrow.PNG");
	m_division.Initialize(Vector3(.0f, .0f, 1.0f), 150, 4);
	m_wait_PathSearchCharacter.clear();
	
	m_working_Nodelist.reserve(500);
	m_working_Nodelist.clear();
	m_static_Object.reserve(10);

	//ノードのロード
	Load_Node(node_filename);
	m_node_Information = new Field_NodeInformation();
	//ノード情報ロード
	m_node_Information->Load(nodeinfo_filename, m_node_List.size());

	//方角初期化
	m_direction[0] = Vector3(sinf(Direction::north),.0f,cosf(Direction::north));
	m_direction[1] = Vector3(sinf(Direction::north_East), .0f, cosf(Direction::north_East));
	m_direction[2] = Vector3(sinf(Direction::east), .0f, cosf(Direction::east));
	m_direction[3] = Vector3(sinf(Direction::south_East), .0f, cosf(Direction::south_East));
	m_direction[4] = Vector3(sinf(Direction::south), .0f, cosf(Direction::south));
	m_direction[5] = Vector3(sinf(Direction::south_West), .0f, cosf(Direction::south_West));
	m_direction[6] = Vector3(sinf(Direction::west), .0f, cosf(Direction::west));
	m_direction[7] = Vector3(sinf(Direction::north_West), .0f, cosf(Direction::north_West));
	for (int i = 0; i < 8; i++)
		m_direction[i].Normalize();

}

void	NavigationSystem::Release()
{
	m_wait_PathSearchCharacter.clear();
	m_division.Release();
	All_Release();
	SAFE_DELETE(m_node_Information);
	SAFE_DELETE(m_node_Tex);
}

void	NavigationSystem::Process()
{
	if (m_wait_PathSearchCharacter.empty())
		return;
	
	
	if (m_wait_PathSearchCharacter.size() >= 10)
	{
		MyAssert(false, "多すぎない？");
	}


	//メッセージ送信
	AI_MESSAGE msg;
	msg.type = ai_messagetype::search_path_ok;
	msg.extra_Info = &m_wait_PathSearchCharacter.front();
	ai_message->Send_PathPlannerMessage(m_wait_PathSearchCharacter.front().request_Brain, &msg);
	m_wait_PathSearchCharacter.pop_front();
}

void	NavigationSystem::Update()
{
	Process();
#ifdef NPC_DEBUG

	/*MyDebugMgrStr(dword_color::white, "経路探索待ちキャラ %d", wait_PathSearchCharacter.size());
	for (auto it = wait_PathSearchCharacter.begin(); it != wait_PathSearchCharacter.end(); it++)
	{
		MyDebugMgrStr(dword_color::yellow, "%s", dynamic_cast<NPC_Brain*>(it->brain)->character_Name);
		MyDebugMgrStr(dword_color::yellow, "RESEARCH_FLUG %d", it->research);
	}*/
	
	//MyDebugMgrStr(dword_color::yellow, "空間に登録されているノード数");
	//MyDebugMgrStr(dword_color::yellow, "%d",division.AllObjectSize());

#endif

}

void	NavigationSystem::All_Reset()
{
	for (auto it = m_node_List.begin(); it != m_node_List.end(); it++)
	{
		(*it)->Reset();
	}
}

bool	NavigationSystem::Add_WaitCaracter(PathSearchObject& insert)
{	
	m_wait_PathSearchCharacter.push_back(insert);
	return true;
}

bool	NavigationSystem::Load_Node(char* filename)
{
	textLoader loader(filename);
	loader.Search("NODE");
	int num_node = loader.LoadInt();
	loader.In();
	char buffer[256];
	for (;;)
	{
		loader._Loadstring(buffer);
		if (strcmp(buffer, "}") == 0) break;
		else
		{
			int id = atoi(buffer);
			float x = loader.LoadFloat();
			float y = loader.LoadFloat();
			float z = loader.LoadFloat();
			Field_Node* node = new Field_Node();
			node->pos = Vector3(x, y, z);
			Add_Node(node);
			//空間分割
			node->division_Ptr = new Tree_Object<Field_Node>(node);
			node->division_Ptr->Registar_Field(node->pos.x, node->pos.z, &m_division);
		}
	}

	//Edge情報読みこみ
	loader.Search("EDGE");
	loader.In();
	for (;;)
	{
		loader._Loadstring(buffer);
		if (strcmp(buffer, "}") == 0) break;
		else
		{
			int id = atoi(buffer);
			int num_Edge = loader.LoadInt();
			for (int i = 0; i < num_Edge; i++)
			{
				int to_Node = loader.LoadInt();
				float dist = loader.LoadFloat();
				Field_Edge* insert = new Field_Edge();
				insert->from = m_node_List[id];
				insert->to = m_node_List[to_Node];
				insert->Set_DefaultCost(dist);
				insert->m_direction = (direction)(loader.LoadInt());
				//読み飛ばし
				//段差フラグと座標
				loader.LoadInt();
				loader.LoadFloat();
				loader.LoadFloat();
				loader.LoadFloat();
 				Insert_Edge(m_node_List[id], insert);
			}
		}
	}
	loader._Loadstring(buffer);

	m_num_Edge = atoi(buffer);
	//printf("ノード読みこみ成功\n");
	

	return true;
}

void	NavigationSystem::Render_Node()
{
	if (m_node_List.empty())
		return;

	float tu[2] = { .0f, 1.0f };
	float tv[2] = { .0f, 1.0f };

	for (auto it = m_node_List.begin(); it != m_node_List.end(); it++)
	{
		Vector3 pos = (*it)->pos;
		pos.y += 0.5f;
		Billboard::Draw3D(pos, m_node_Tex, 0.1f, 0.1f, tu, tv, RS_ADD);
	}
}

bool NavigationSystem::Load_StaticObject(char* filename)
{
	/*ラムダ式作成*/


	textLoader loader(filename);
	char buffer[64];
	for (;;)
	{
		loader._Loadstring(buffer);
		if (strcmp(buffer, "VENDING_MACHINE") == 0)
		{

		}
	}
}

Field_Node* NavigationSystem::Find_Node_Pos(const Vector3& pos)
{
	if (!m_working_Nodelist.empty())
		m_working_Nodelist.clear();

	//空間クラスからノードを取得
	m_division.Search_Object_Near(pos.x, pos.z, &m_working_Nodelist);
	
	float dist = FLT_MAX;
	Field_Node* ret = nullptr;

	for (auto it = m_working_Nodelist.begin(); it != m_working_Nodelist.end(); it++)
	{
		float work_Dist = (pos - (*it)->pos).LengthSq();
		if (work_Dist < dist)
		{
			ret = (*it);
			dist = work_Dist;
		}
	}
	return ret;
}

bool	NavigationSystem::Duplication_Check(const Vector3& a, const Vector3& b)
{
	Field_Node* a_node = Find_Node_Pos(a);
	Field_Node* b_node = Find_Node_Pos(b);
	if (!a_node || !b_node)
	{
#if NPC_ASSART
		MyAssert(false, "ノードがないよ : Duplication_Check");
#endif
		return false;
	}
	return (a_node->index == b_node->index) ? true : false;
}

bool	NavigationSystem::Get_Debug_NodeList(float pos_X, float pos_Z)
{
	if (!debug_NodeList.empty())
		debug_NodeList.clear();

	Vector3 vec(sinf(Direction::north), .0f, cosf(Direction::north));
	return m_division.Search_Object_Near<std::vector<Field_Node*>>
															(pos_X, pos_Z,&debug_NodeList);
}

void	NavigationSystem::Render_DebugNode()
{
	if (debug_NodeList.empty())
		return;

	float test[2] = { .0f, 1.0f };
	for (auto it = debug_NodeList.begin(); it != debug_NodeList.end(); it++)
	{
		Billboard::Draw3D((*it)->pos, m_node_Tex, 0.5, 0.5, test, test, RS_COPY);
	}
}

bool	NavigationSystem::HandleMessage(AI_MESSAGE* msg)
{
	switch (msg->type)
	{
	case ai_messagetype::demand_search_path :
		Add_WaitCaracter(*(static_cast<PathSearchObject*>(msg->extra_Info)));
		//printf("経路探索申請\n");
		break;
	}
	return true;
}

const WayPointInformation* NavigationSystem::Get_NodeInformation(const Vector3& pos)
{
	if (!m_working_Nodelist.empty())
		m_working_Nodelist.clear();
	
	m_division.Search_Object_Near(pos.x, pos.z, &m_working_Nodelist);
	
	//一番近いノードを返却
	Field_Node* target = nullptr;
	float min_dist = FLT_MAX;
	for (auto node : m_working_Nodelist)
	{
		Vector3 vec = pos - node->pos;
		float dist = vec.LengthSq();
		if (min_dist > dist)
		{
			target = node;
			min_dist = dist;
		}
	}
	return m_node_Information->Get_NodeInfo(target->index);
}

const WayPointInformation* NavigationSystem::Get_NodeInformation(const int& id)
{
	return m_node_Information->Get_NodeInfo(id);
}


direction	NavigationSystem::Vector3_Convert_Direction(const Vector3& vec)
{
	float most_min_cos = FLT_MAX;
	direction ret = direction::num_direction;

	for (int i = 0; i < 8; i++)
	{
		Vector3 base = m_direction[i];
		float cos = 1.0f - Vector3Dot(base, vec);
		if (cos <= most_min_cos)
		{
			most_min_cos = cos;
			ret = (direction)i;
		}
	}
	return ret;
}