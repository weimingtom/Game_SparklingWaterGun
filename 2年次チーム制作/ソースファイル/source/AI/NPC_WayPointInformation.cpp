#include	"..\All.h"

Field_NodeInformation::Field_NodeInformation() :
m_Init_Flg(false), m_nodeinfo_Array(nullptr)
{

}

Field_NodeInformation::~Field_NodeInformation()
{
	delete[] m_nodeinfo_Array;
}

bool	Field_NodeInformation::Load(char* filename, const int& num_Node)
{
	int num_direction = enum_cast(direction::num_direction);
	textLoader loader(filename);
	if (!loader.Be_Init())
	{
		MessageBox(nullptr, "NodeInfomation_Creater::Load‚ªŽ¸”s‚µ‚Ü‚µ‚½", nullptr, MB_OK);
		return false;
	}

	int num_Size = loader.LoadInt();

	if (num_Size != num_Node)
	{
		MessageBoxPlus("ƒm[ƒh”‚ª‘Î‰ž‚µ‚Ä‚Ü‚¹‚ñAgraph_num_Node : %d , text_num_Node : %d", num_Node, num_Size);
		return false;
	}
	m_nodeinfo_Array = new WayPointInformation[num_Size];

	loader.Search("MaxVisibilityNodeIndex");
	bool flg = loader.LoadInt()>0;
	loader.In();
	for (int current = 0; current < num_Size; current++)
	{
		for (int d = 0; d < num_direction; d++)
		{
			//visibility_dist
			m_nodeinfo_Array[current].m_max_Dist[d] = loader.LoadFloat();
			//visibility_index‚Í“Ç‚Ý”ò‚Î‚·
			if (flg)
				 loader.LoadInt();
			//hidden_point
			int hidden_point_size = loader.LoadInt();
			for (int i = 0; i < hidden_point_size; i++)
			{
				int node_id = loader.LoadInt();
				m_nodeinfo_Array[current].m_hidden_Point[d].push_back(navigation_system->Find_Node(node_id));
			}
		}
		//action_circle
		int circle_size = loader.LoadInt();
		for (int i = 0; i < circle_size; i++)
		{
			int node_id = loader.LoadInt();
			m_nodeinfo_Array[current].m_even_with_Circle.push_back(navigation_system->Find_Node(node_id));
		}
	}
	return true;
	
	
}
