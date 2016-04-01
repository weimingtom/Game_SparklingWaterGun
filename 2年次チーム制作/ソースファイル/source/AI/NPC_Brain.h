#ifndef __BRAIN_H__
#define __BRAIN_H__

#include <deque>
#include	<map>
#include	<string>
#include	"Template_Graph.h"
#include	"../EnumCP11.h"
#include	<array>
//console出現マクロ
#define CONSOLE 0

//AIキャラクターデバッグマクロ
#define NPC_DEBUG 0

class Goal;
class NPC_Brain;
class CharacterBrainBase;
class Symbol;
class Working_Memory;
class Character_Record;
class MapObject_Record;
class Sound_Record;
class NPC_Censor;
class Censor_AttackMe;
class Censor_Sight;
class Censor_FriendMessage;
struct DamageParam;
struct ExpectationAct;
class DebugLogOutput;
class Behavior_Manager;
class NPCFriendMessage;

#pragma region ActionPlanner

class Action_Node;
class Action_Edge : public Edge_Base
{
public:
	Action_Edge() :Edge_Base(){}
};

class Action_Node : public Node_Base
{
public:
	Goal* goal;
	Action_Node(Goal* goal) : Node_Base(), goal(goal){}
	Action_Node() : Node_Base(){}
};

class ActionSearch;
class Action_Planner : public Graph<Action_Node,Action_Edge>
{
	friend class NPC_Factory;
private :
	std::vector<Goal*>	m_midlle_Goallist;
	Action_Node*			dummy_Node;				//プラン探索時にスタート位置になるダミーノード	
	NPC_Brain*				m_holder;							//このクラスを所持しているNPC_Brain
	ActionSearch			m_search_Alg;				//探索アルゴリズム
	float							m_default_CostList[static_cast<int>(goal_tag::NumGoal)][static_cast<int>(goal_tag::NumGoal)];	//ゴール間のdefaultcostリスト（edgeに挿入）	

private :
	void							Setting_MidlleGoalList();
	void							Load_GoalCost(char* param_Filename);
	bool							Settting_Goal(std::deque<Goal*>* goal_Stack);
	bool							Chain_Goal(const Symbol& symbol, Character* character);
public :
	Action_Planner(NPC_Brain* b, char* cost_Filename);
	~Action_Planner();

	/**
	*@brief 現在のグラフから一連の行動をプランニング
	*/
	bool	Plannning(const Symbol& symbol, Character* character,std::deque<Goal*>* stack, Action_Node* final_Goal);
	/**
	*@brief全ゴールのステータス初期化
	*/
	void	Clear_GoalStatus();
};
#pragma endregion

//****************************************************//
//Fuzzy関連
//****************************************************//
class NPC_FuzzyTool
{
	friend class NPC_Brain;
private :
	std::map<std::string, FuzzyModule*>	module_Map;
	NPC_FuzzyTool(){}

public :
	~NPC_FuzzyTool();
	void	Initialize();
	
	float	Calculate_Sight_Target_Character(const float& dist, const float& ammo);
};

struct DeFuzzify_Param
{
	float m_sight_target;

	void	Load(char* filename);
};

class Target
{
public :
	ENTITY_ID current;
	Target() : current(game_id::id_error)
	{	}
};

struct StatusParam
{
	bool	m_is_Arive;					//生存フラグ
	bool	m_is_Active;				//行動許可
	int	m_active_Timer;
	StatusParam() : m_is_Active(true), m_is_Arive(true), m_active_Timer(0)
	{	}
};

enum test;

class Goal_Think
{
	friend class NPC_Factory;
private :
	std::vector<FinalGoal_Evalutor*>	m_goal_Evalutor;
	FinalGoal_Evalutor*	m_current_FinalGoal;	//現在の最終ゴール
	NPC_Brain * m_holder;

private: 
	bool	Set_FriendMessageBias(const int messagetype);
	
public :
	Goal_Think(NPC_Brain* holder);
	~Goal_Think();
	void	Initialize();

public :
	Action_Node* Thinking(Character* character);
	bool	HandleMessage(AI_MESSAGE* msg);
	inline void	Reset_CurrentTarget_FinalGoal(){ m_current_FinalGoal->Reset_Target(); }
	FinalGoal_Evalutor* Get_FinalGoal(){ return m_current_FinalGoal; }
	__forceinline void	 Reset_MessageBias()
	{
		for (auto it : m_goal_Evalutor)
			it->Clear_MessageBias();
	}


};

#pragma region Brain

class NPCParam
{
public:
	float	fov;			//視野角
	float	sight;		//視力(sq)
	int		m_chara_SightInterval;			//キャラクターに対する更新間隔
	int		m_map_SightInterval;				//マップオブジェクトに対する更新間隔
	int		ammo_Interval;
	int		sound_Interval;
	float		sight_Guarantee;						//今現在オブジェクトが見えていることを保証する信頼値()（0~1.0）
	float		max_HearingRange;				//ゲーム中一番大きい音が聞こえる最大距離（EnvManagerから音を取ってくるときに必要）
	float		reaction_FootSound;				//足音に反応する距離
	float		reaction_GunSound;				//銃声に反応する距離
	int		record_character_ReliabilityTime;	//CharacterRecordの信頼値が０になる時間	
	int		record_mapobject_ReliabilityTime;	//MapObjectRecordの信頼値が0になる時間
	int		record_ExtAct_ReliabilityTime;		//行動予測を記録しておくフレーム数
	int		feel_few_WeaponCharge;				//charge率が低いと感じる値（主にSymbol::WeaponChargeで使用）
	float		m_feelAttack;								//何かを攻撃していると感じる距離
	float		m_feel_high_SharePoint;				//広告ゴールにおいてshare率が高いと感じる値（0~1）
	float		m_max_rotateAngle_Speed;
	float		m_enemy_AttackMe_Fov;			//敵が自分を攻撃しようとしていると判断する視野角
	float		m_feel_NearRangeDist;				//近接攻撃いけると感じる距離(sq)
	int		m_interval_characterTarget_SearchPath;
	int		m_interval_advertisementpoint_SearchPath;
	float		m_far_of_Vessel;							//Goal_AvoidanceVesselを判断するときに乗り物が遠いと感じる距離
	int		m_probability_grenede_attack;	//グレネード攻撃を行う確率
	int		m_response_Sight;						//censor_sightのPlanning反応速度
	int		m_responce_AttackMe;				//censor_attackmeのPlanning反応速度
	int		m_responce_FriendMessage;		//censor_FriendMessageのPlanning反応速度
	float		m_knockdownEnemy_distBias_Begin;	//KnockDownEnemy評価関数において評価値を減少させはじめる基準距離
	float		m_knockdownEnemy_distBias_End;	//KnockDownEnemy評価関数において評価値を0にする距離
	float		m_friendMessageBias_MedicalFriend;	//MedicalFriendにおける評価関数のMessageBias
	NPCParam();

	bool	Load(char* filename);

};

/**
*@brief 特殊ゴール管理クラス
*/
class ParticularGoal_Manager
{
private :
	std::vector<Goal*> m_particularGoal_vector;
	ENTITY_ID				m_target_ID;		//特殊ゴール専用のターゲットID
	NPC_Brain*				m_holder;
	bool							m_active;
private :
	void	Initialize_Goal();
public :
	ParticularGoal_Manager( NPC_Brain* holder);
	~ParticularGoal_Manager();

public :
	inline void Active(){ m_active = true;}
	inline void	 Exit(){ m_active = false; }
	inline bool Is_Active(){ return m_active; }
	bool	Insert_Goal(Character* character, const partculargoal_type& insert_goal);
	inline const ENTITY_ID& Get_TargetID(){ return m_target_ID; }
	inline void	Set_TargetID(const ENTITY_ID& id){ m_target_ID = id; }

};



struct RAP_CENSROR
{
	Censor_Sight*					m_sight;
	Censor_AttackMe*			m_attack_Me;
	Censor_FriendMessage*	m_friend_Message;
	RAP_CENSROR() : m_sight(nullptr), m_attack_Me(nullptr), m_friend_Message(nullptr)
	{	}
	void	Release()
	{
		SAFE_DELETE(m_sight);
		SAFE_DELETE(m_attack_Me);
	}
};

class NPC_Evalutor;
class PathPlanner;

class NPC_Brain :public CharacterBrainBase
{
private :
	std::vector<Goal*>			m_final_Goallist;
	std::deque<Goal*>			m_goal_Deque;	//ゴール列
	RAP_CENSROR					m_censor;
	NPCParam						m_chara_Param;
	StatusParam						m_status_Param;
	WorkingMemory				m_memory;
	Symbol								m_symbol;
	NPC_FuzzyTool					m_fuzzy;
	Goal_Think*						m_think;
	Action_Planner*				m_planner;		//グラフ操作で行動を決定する
	Target								m_target;
	DeFuzzify_Param				m_defuzzify_param;
	DebugLogOutput*			m_console;
	std::string						m_logfilename;
	Behavior_Manager*			m_behavior_Manager;
	NPC_Evalutor*					m_evalutor;
	PathPlanner*					m_path_Planner;
	ParticularGoal_Manager*	m_particularGoal_Manager;
	NPCFriendMessage*			m_message_Icon;

private :
	/**
	*@note 関数ポインタの代入も同時に行う
	*/
	void	Intialize_Censor();
	void	Set_FinalGoal();
	void	Goal_Completed(Character* p);
	void	Goal_Process(Character* p);
	void	Goal_Failed(Character* p);
	void	All_Reset(Character* character);
	bool	Planning(Character* charcter);
	bool	Is_Arive(Character* character);
	void	Clear_FinalGoalStatus();
	/**
	*@brief 記憶更新時の処理
	*/
	bool	Management_Message_Memory(GameMessage* msg);
	void	Update_Censor(Character* character);

public	:
	 NPC_Brain(const char* name,const int Controllder_Num,const int team_type);
	 ~NPC_Brain();
	 void Update(Character* p);
	bool HandleMessage(LPGAMEMESSAGE msg);
	/**
	*@brief 引数のゴールを最前列に挿入する
	*/
	bool	Insert_Goal(Character* character,Goal*insert_Goal);

	/**
	*@brief	mapobjectの記憶を取得 
	*@return 使用可能ならばRecordのポインタと第二引数に信頼値が格納される,使えない場合はnullptr
	*/
	MapObject_Record*	Get_MapObjectMemory(const ENTITY_ID& ID, float*reliability);
	/**
	*@brief	characterの記憶を取得
	*@return 使用可能ならばRecordのポインタと第二引数に信頼値が格納される,使えない場合はnullptr
	*/
	Character_Record*	Get_CharacterMemory(const ENTITY_ID& ID, float*reliability);
	inline  MEMORY& GetMemory() { return (m_memory.memory); }
	//テスト
	const char*							character_Name;
	void								Log(const std::string& str);
private :
	friend class NPC_Factory;
	friend class Accesor_Brain;
	friend class NPC_Evalutor;
	friend class Goal_Think;
	friend class AI_MessageDispatcher;
};


#include	"NPC_Evalutor.h"
#include	"Accessor_Brain.h"
#include	"BlackBoard.h"

#pragma endregion

#endif
