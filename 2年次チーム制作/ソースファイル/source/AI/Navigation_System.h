#ifndef __NPC_PATHPLANNER_H__
#define __NPC_PATHPLANNER_H__
#include"Template_Graph.h"
#include	<vector>

#pragma region Direction&Visibility
//方角計算用一時define
#define _NORTH .0f
#define _NORTH_EAST 2* PI / 8
#define _EAST 2*PI / 4
#define _SOUTH_EAST (2*PI / 8) + ( 2*PI / 4)
#define _SOUTH PI
#define _SOUTH_WEST PI + (2*PI / 8)
#define _WEST PI + (2*PI / 4)
#define _NORTH_WEST (PI + (PI *3/4))
//方角名前空間
namespace Direction
{
	const float north = _NORTH;
	const float north_East = _NORTH_EAST;
	const float east = _EAST;
	const float south_East = _SOUTH_EAST;
	const float south = _SOUTH;
	const float south_West = _SOUTH_WEST;
	const float west = _WEST;
	const float north_West = _NORTH_WEST;
	
	const float	direction_space = _NORTH_EAST;	//方角の間隔
}

enum class direction;

enum class direction
{
	none = -1,
	north,
	north_east,
	east,
	south_east,
	south,
	south_west,
	west,
	north_west,
	num_direction
};

#pragma endregion
class Field_NodeInformation;
class WayPointInformation;

#pragma region FIELD

#pragma region NODE&EDGE
class Field_Node : public Node_Base
{
public :
	Vector3 pos;
	float	visibility_Weight;	//可視状態時にかかるウェイト
	Tree_Object<Field_Node>*  division_Ptr;	//空間分割用
	
public :
	Field_Node() : Node_Base(){visibility_Weight = .0f; division_Ptr = nullptr; }
	~Field_Node(){}
	void	Release()
	{
		for (auto it = edge_List.begin(); it != edge_List.end(); it++)
		{
			SAFE_DELETE((*it));
		}
		SAFE_DELETE(division_Ptr);
	}

	void	Reset()
	{
		Node_Base::Reset();
		visibility_Weight = .0f;
	}
};

class Field_Edge : public Edge_Base
{
public :
	direction		m_direction;					//エッジの方角
	Field_Edge() : Edge_Base(), m_direction(direction::none)
	{}

};
#pragma endregion

struct PathSearchObject
{
	ENTITY_ID		id;
	NPC_Brain*		request_Brain;
	int					request_Frame;
	goal_tag			m_request_Goaltag;
};

class NavigationSystem : public Graph<Field_Node, Field_Edge>
{
	friend class Field_Debugger;
private :
	std::vector<Field_Node*>								m_working_Nodelist;	//空間分割クラスから返却されるノードを格納する作業用コンテナ
	iex2DObj*														m_node_Tex;
	std::vector<Field_Node*>								m_static_Object;						//移動しないオブジェクト情報（自販機とか）
	FieldDivision_Manager<Field_Node>				m_division;
	std::list<PathSearchObject>							m_wait_PathSearchCharacter;	//経路探索待ちリスト
	Vector3															m_direction[8];
	Field_NodeInformation*									m_node_Information;
	//デバッグ用
private :
	std::vector<Field_Node*>	debug_NodeList;
	
private :
	NavigationSystem() : Graph(){}
	/**
	*@brief 経路探索申請処理
	*/
	bool	Add_WaitCaracter(PathSearchObject& brain);
	/**
	*@brief 経路探索申請キャラクターを処理する
	*/
	void	Process();

public :
	static	NavigationSystem* Get_Instance()

	{
		static NavigationSystem	I;
		return &I;
	}
	void	Initialize(char* node_filename, char* nodeinfo_filename);
	void	Release();
	void	Update();
	bool	HandleMessage(AI_MESSAGE* msg);
	/**
	*@brief 全ノードコスト初期化
	*/
	void	All_Reset();
	/**
	*@brief
	/**
	*@brief ノード読み込み
	*/
	bool	Load_Node(char* filename);

	void	Render_Node();
	/**
	*@brief 静的オブジェクト読み込み
	*/
	bool	Load_StaticObject(char* filename);

	///**
	//*@brief 静的オブジェクトがゴールの場合のゴールを探す
	//*@param1 探索アルゴリズムへのポインタ
	//*@param2 ゴールオブジェクトのタイプ
	//*/
//	bool	Search_GoalNode_Static(PathSearch_Base<Field_Node, Field_Edge>* search_Alg,
	//			const int& object_Type);
	/**
	*@brief 引数座標から一番近いノードを探す
	*/
	Field_Node* Find_Node_Pos(const Vector3& pos);
	template<class C>
	/**
	*@brief 引数情報からノードを探す
	*@param1 探索開始座標
	*@param2 探索方向
	*@param3 探索距離
	*@param4 ノードを格納するコンテナポインタ
	*@param5 trueで開始座標を含めず探索
	*/
	inline bool	Find_Node_Container(const Vector3& pos, const Vector3* vec, const float& dist,
		C* ret_vecter, bool skip_F = false)
	{
		return m_division.Search_Object_Near(pos.x, pos.z, ret_vecter);
	}
	/**
	*@brief ノードの埋め込み情報取得
	*/
	const WayPointInformation* Get_NodeInformation(const Vector3& pos);
	const WayPointInformation* Get_NodeInformation(const int& id);
	template<class Predicate>
	/**
	*@brief  指定したプレディケートが適合するノードを返す
	*/
	inline bool	Search_StaticObject(Predicate& predicate, std::vector<Field_Node*>* ret_List)
	{
		bool ret = false;
		for (auto it = m_static_Object.begin(); it != m_static_Object.end(); it++)
		{
			if (predicate((*it)))
			{
				ret_List->push_back((*it));
				ret = true;
			}
		}
		return ret;
	}
	inline const Vector3 Get_Direction(direction& d){ return m_direction[enum_cast(d)]; }

	bool	Get_Debug_NodeList(float pos_X, float pos_Z);
	void	Render_DebugNode();
	template<class C>
	void	Set_Debug_NodeList(C* container)
	{
		if (!debug_NodeList.empty())
			debug_NodeList.clear();
		
		for (auto it = container->begin(); it != container->end(); it++)
		{
			debug_NodeList.push_back((*it));
		}
	}

	/**
	*@brief 二つのノードが被っているかをチェックする
	*@return かぶってたら true
	*/
	bool	Duplication_Check(const Vector3& a, const Vector3& b);
	/**
	*@brief 引数ベクトルから方角を返却
	*@return errorでnum_direction(8)
	*/
	direction Vector3_Convert_Direction(const Vector3& vec);
	
	int	GetSize(){ return m_wait_PathSearchCharacter.size(); }

};
#define navigation_system (NavigationSystem::Get_Instance())

#pragma endregion

#pragma region DEBUGNODE
class NodeConnecter
{
	friend class Field_Debugger;
private :
	DWORD character_Color;
	ENTITY_ID holder;
private :
	NodeConnecter(const ENTITY_ID& id, const DWORD& color);
	std::list<Field_Edge*> route_Edge;
public :
	bool	Erase_Edge(Field_Edge* edge);
	void	Set_Route(std::list<Field_Edge*> w_stack);
	inline	void	Route_Clear(){ route_Edge.clear(); }
	void	Render();
};

class GoalPoint
{
	friend class Field_Debugger;
private :
	DWORD character_Color;
	ENTITY_ID		holder;
public :
	GoalPoint(const ENTITY_ID& id, const DWORD& color);
	Field_Node*	node;
	inline void	Set_GoalPoint(Field_Node* goal){ node = goal; }
	void		Render(iex2DObj* tex, const float& width, const float& height);
};

class Field_Debugger
{
	friend class NavigationSystem;
private :
	std::vector<GoalPoint*>				goal_List;
	std::vector<NodeConnecter*>	edge_Renderer;
	iex2DObj*											node_Tex;
	static Field_Debugger*					instance;
	Field_Debugger(){}
	int														num_NPC;
	float													node_TexWidth;
	float													node_TexHeight;

public :
	void	Initialize(const int&	num_NPC,const float& width,const float& height);
	bool	Register(const ENTITY_ID& id, const DWORD& color);
	~Field_Debugger();
	static	void	Create_Instance()
	{
		if (!instance)
			instance = new Field_Debugger();
	}
	static Field_Debugger* Get_Instance()
	{
		return instance;
	}
	static void	Release()
	{
		SAFE_DELETE(instance);
	}
	void	Render_GoalNode();
	void	Reder_GoalRoute(const ENTITY_ID& id = game_id::id_error);
	bool	Send_Goal(const ENTITY_ID& entity, Field_Node*node);
	bool	Send_Route(const ENTITY_ID& entity, const std::list<Field_Edge*>& route);
	void	Render_AllEdge();
	/**
	*@brief		特定のEdgeを削除
	*@param1	キャラクターのEintityID
	*@param2	削除するEdgeのポインタ
	*/
	bool	Erase_Debug_Edge(const ENTITY_ID& holder_ID, Field_Edge* erase_Edge);

	void	Render_Edge(const Vector3& pos);

	void	Render_Division(const Vector3& pos);
};

#define field_Debug (Field_Debugger::Get_Instance())

#pragma endregion

#pragma region NODEINFORMATION

class WayPointInformation
{
	static const int	num_direction = 8;
	static const int	max_hiddenPoint = 5;
	
public :
	float m_max_Dist[num_direction];
	std::vector<Field_Node*>	m_hidden_Point[num_direction];
	std::vector<Field_Node*>	m_even_with_Circle;
	
	WayPointInformation()
	{
		for (int i = 0; i < num_direction; i++)
		{
			m_hidden_Point[i].reserve(max_hiddenPoint);
			m_max_Dist[i] = .0f;
		}
		m_even_with_Circle.reserve(20);
	}

};

class Field_NodeInformation
{
private:
	bool							m_Init_Flg;
	WayPointInformation*	m_nodeinfo_Array;
public:
	Field_NodeInformation();
	~Field_NodeInformation();


public :
	bool	Load(char* filename,const int& num_node);

	inline const WayPointInformation* Get_NodeInfo(const int& node_index)
	{
		return &m_nodeinfo_Array[node_index];
	}

};
#pragma endregion 

#endif


