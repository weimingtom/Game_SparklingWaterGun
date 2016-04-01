#ifndef __NPC_BEHAVIOR_H__
#define __NPC_BEHAVIOR_H__

#include "../EnumCP11.h"

class Field_Edge;
class NPC_Brain;
class Character;
class Goal;
class WayPointInformation;
class NPC_Aim;

class Field_Node;
class Field_Edge;

template<class Node_Type, class Edge_Type>
class PathSearch_Base;

#pragma region enum

_ENUM_BEGIN(ai_messagetype, AI_MESSAGETYPE)

error = -1,
//beahvior_msg
search_path_ok,				//経路探索Ok
demand_search_path,		//経路探索申請
set_route,						//LISTにルートを流し込む
set_currentGoalPos,			//ターゲット座標設定
set_LookVec,					//targetRotateVecをセット（Anime_LookBack）
update_memory,				//記憶が更新された
lock_goal,						//ゴールのロック
chracter_Die,
wait,
set_messsage_bias,			//最終ゴール評価関数に補正値を加算
//goal_msg
evalute_currentGoalPos,	//

_ENUM_END(ai_messagetype, AI_MESSAGETYPE)

struct AI_MESSAGE
{
	ai_messagetype type;
	void*	extra_Info;

	AI_MESSAGE(const ai_messagetype& type, void* extra_Info) :
		type(type), extra_Info(extra_Info){}
	AI_MESSAGE(){}

};


_ENUM_BEGIN(behavior_tag, BEHAVIOR_TAG)

error = -1,
move_edge,
move_point,
shot_weapon_stand,
shot_weapon_circle,
rolling_attack,
charge_weapon,
look_back,
grenede_throw,
air_borne,

num_behavior

_ENUM_END(behavior_tag, BEHAVIOR_TAG)

inline char* message_char(const ai_messagetype & tag)
{
	static char* ret[] =
	{
		"search_path_ok",				//経路探索Ok
		"demand_search_path",		//経路探索申請
		"set_route",						//LISTにルートを流し込む
		"set_currentGoalPos",			//ターゲット座標設定
		"set_LookVec",					//targetRotateVecをセット（Anime_LookBack）
		"update_memory",				//記憶が更新された
	};
	return ret[enum_cast(tag)];
}

#pragma endregion

//経路再検索時に必要な情報ラッピング
struct R_Targetwrapping
{
	Character*	search_Character;	//検索するキャラクター
	bool				see_Through;
	Vector3		current_GoalPos;
	R_Targetwrapping(){}
	R_Targetwrapping(Character* c, const bool& s,const Vector3& current_Goal/*,const OBJECTTYPE& type*/) :
		search_Character(c), see_Through(s),current_GoalPos(current_Goal)/*,object_Type(type)*/{}
	R_Targetwrapping(const R_Targetwrapping&);

};


class AI_BehaviorBase
{
	friend class Behavior_Manager;
protected :
	Goal*										m_parent;	//	振る舞いの保持者
	goal_status								m_status;
	B_ProcessFunc							m_process_Function;
public :
	const behavior_tag tag;
public:
	AI_BehaviorBase(const behavior_tag& init_tag, const char* name) : 
		tag(init_tag), behavior_Name(name),m_parent(nullptr),m_process_Function(nullptr){}
	virtual ~AI_BehaviorBase(){}
	const char* behavior_Name;
public:
	virtual void Enter(NPC_Brain* brain, Character* character) = 0;
	virtual goal_status Execute(NPC_Brain* brain, Character* character) = 0;
	virtual void Exit(NPC_Brain* brain, Character* character) = 0;
	virtual bool HandleMessage(AI_MESSAGE* msg) = 0;
	inline const goal_status Get_Status(){ return m_status; }
};

#pragma region Goto(MoveEdge,Point)

class PathPlanner;

//エッジの移動
class Move_Edge : public AI_BehaviorBase
{
	friend class PathPlanner;
	static const int move_error_time = 60;
	static const int	wait_error_time = 120;
private :
	std::list<Field_Edge*>			route_List;
	bool											m_wait;
	bool											m_jump;
	float											m_max_rotate_Speed;
	Vector3									m_before_Pos;	//１フレーム前の座標
	int											m_move_ErrorTimer;	
	int											m_wait_ErrorTimer;
	
private :
	/**
	*@brief	現在の経路を平滑化する
	*@note	平滑化するのは一か所だけ
	*/
	bool	Smoothing_Route(Character* character);
	/**
	*@brief 
	*/
	bool	Check_Erase_Edge(Character* character, const Vector3& current_Pos,
											const Vector3& tonode_Pos,const float& dist_sqs);
	goal_status	Move(Character* character, const Vector3& current_Pos, const Vector3& tonode_Pos);
	/**
	*@brief 段差チェック
	*/
	bool	Check_Difference_In_Level(Character* character,const Vector3& current_Pos,const Vector3& to_NodePos);
	goal_status	Jump(Character* character, const Vector3& current_Pos, const Vector3& tonode_Pos,
		 Field_Edge* current_Edge);
	
	void	SetRoute(PathSearch_Base<Field_Node, Field_Edge>* alg);
	/**
	*@brief 障害物に引っかかってないかチェックする
	*@return 引っかかっている場合はfalse
	*/
	bool	MoveCheck(const Vector3& current_Pos);

	/**
	*@brief 経路探索待機中、何らかの原因で探索できなくなってしまったときのための回避策
	*/
	bool	WaitErrorCheck();
	/**
	*@brief AIが認識していないMapObject（クレーン等）により崖などから落ちてしまった時の対応策
	*/
	bool	MoveErrorCheck(Character* character,const Vector3& current_Pos, const Vector3& current_TargetPos);

public :
	Move_Edge();
	~Move_Edge();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);

public :
	void	Set_DebugEdge();
};

//指定した座標に移動
class Move_Point : public AI_BehaviorBase
{
	static const int move_error_time = 60;
private:
	bool						m_Active;	//ゴール座標がセットされているかどうかのフラグ
	bool						m_jump;
	Vector3				m_goal_Pos;
	Vector3				m_before_Pos;	//一フレーム前の座標
	int						m_move_ErrorTimer;

private :
	void	Move(NPC_Brain* brain, Character* character,const Vector3& cur_Pos);
	void	Jump(NPC_Brain* brain, Character* character,const Vector3& cur_Pos);
	bool	Check_Difference_In_Level(Character* character,const Vector3& cur_Pos);
	bool	MoveCheck(const Vector3& cur_Pos);
	bool	GoalCheck(const Vector3& cur_Pos);

public :
	Move_Point();
	~Move_Point();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);

};
#pragma endregion

//******************************************************
//	AnimationState
//******************************************************
#pragma region SHOT_WEAPON(この辺ひどいからそのうち直したい)

/**********武器で攻撃(不動)*********/
class Anime_ShotWeapon_Stand : public AI_BehaviorBase
{
private:
	ENTITY_ID								m_target_Id;
	ENTITY_ID								m_holder_Id;
	NPC_Aim*								m_aim;
private :
	//void	Aim(NPC_Brain* brain,Character* character);
public:
	Anime_ShotWeapon_Stand(NPC_Aim* use_aimObject);
	~Anime_ShotWeapon_Stand();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

class Anime_ShotWeapon_Circle: public AI_BehaviorBase
{
	static const int							m_timer = 120;	//なんらかの原因でその場から動かないまま走ってた場合のタイマー
private:
	ENTITY_ID								m_target_Id;
	ENTITY_ID								m_holder_Id;
	Vector3									m_current_Centor_Pos;						//現在の円の中心座標
	const WayPointInformation*	m_current_CentorNodeInformation;	//中心のノードの情報
	int											m_current_MoveTargetIndex;				//container上の添字std::vector<Field_Node*> m_even_with_Circle[]		[] <-ｺｺの数字
	int											m_current_MoveCircleSize;					//std::vector<Field_Node*> m_even_with_Circleのサイズ
	NPC_Aim*								m_aim;
	Vector3									m_before_Pos;	//前のフレームの座標
	int											m_current_Time;

private:
	void	Culculate_WayPointInformation(const Vector3& pos);
	void	Move(NPC_Brain* brain, Character* character);
	void	Shuffle_Point();
public:
	Anime_ShotWeapon_Circle(NPC_Aim* use_aimObject);
	~Anime_ShotWeapon_Circle();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

#pragma endregion 

class Anime_ChargeWeapon : public AI_BehaviorBase
{
private :
	int	m_need_Charge;
public :
	Anime_ChargeWeapon();
	~Anime_ChargeWeapon();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

class Anime_RollingAttack : public AI_BehaviorBase
{
public:
	Anime_RollingAttack();
	~Anime_RollingAttack();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

class Anime_ThrowGrenade : public AI_BehaviorBase
{
public:
	Anime_ThrowGrenade();
	~Anime_ThrowGrenade();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

//振り向く
class Anime_LookBack : public AI_BehaviorBase
{
	static const int m_lookBack_Time = 200;
private :
	Vector3	m_look_Vec;	//向くべき方向
	bool			m_update_Memory;	//振り向いた先で記憶が更新されたかどうか
	bool			m_wait;			//振り向いてからの待機フラグ
	int			m_timer;
public:
	Anime_LookBack();
	~Anime_LookBack();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

class Anime_AirBorne : public AI_BehaviorBase
{
	static const int m_changevec_intarval = 250;
	
private :
	int			m_timer;
	Vector3	m_move_vec;	//移動方向
	float m_radius;
private :
	void	Direction_MoveVec();
	void	Radius_Check(const Vector3& player_pos);

public:
	Anime_AirBorne();
	~Anime_AirBorne();
	void Enter(NPC_Brain* brain, Character* character);
	goal_status Execute(NPC_Brain* brain, Character* character);
	void Exit(NPC_Brain* brain, Character* character);
	bool HandleMessage(AI_MESSAGE* msg);
};

#pragma region Manager

class Behavior_Manager
{
	friend class AI_BehaviorBase;
private :
	AI_BehaviorBase*	m_current_Behavior;
	behavior_tag			m_current_tag;
	NPC_Brain*				m_holder;
	std::vector<AI_BehaviorBase*>	m_behavior_Container;

private :
	void	InitializeContainer();

public :
	Behavior_Manager(NPC_Brain* holder);
	~Behavior_Manager();

public :
	bool	Change_Behavior(Character* character,const behavior_tag& next_behavior,
										Goal* parent,B_ProcessFunc use_process_Function);
	const goal_status			Get_BehaviorStatus();
	void	Set_MoveEdgeSearch_Interval(const int& interval);
	void	Exit_Current_Behavior(Character* character);
	goal_status Execute_Current_Behavior(Character* character);
	bool	Behavior_IsActive();
	inline	AI_BehaviorBase*		Get_Current_Behavior(){ return m_current_Behavior; }
	inline	const behavior_tag&	Get_Current_Tag(){ return m_current_tag; }
};

#pragma endregion

#endif
