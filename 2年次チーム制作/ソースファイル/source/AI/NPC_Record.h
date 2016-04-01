#ifndef __RECORD_H__
#define __RECORD_H__
#include	<map>
#include	<list>
#include	"SoundMessage.h"
#include	<array>
struct ExpectationAct
{
	expectation_act		m_tag;		//行動のタグ
	/**
	*@note				行動に対応するベクトル情報
	*@attack_me		攻撃の方向
	*@move_field		移動方向
	*/
	Vector3					m_vec;	
	inline ExpectationAct() : m_tag(expectation_act::none), m_vec(Vector3(FLT_MAX, FLT_MAX, FLT_MAX))
	{	}

};

_ENUM_BEGIN(arive_status,ARIVE_STATUS)
arive,
dead,
dying,
i_dont_now
_ENUM_END(arive_status, ARIVE_STATUS)

class Character_Record
{
private :
	const	int					m_delete_Time = 300;	//とりあえず100フレームの猶予を持って削除する
	bool								m_delete_Flg;
	int								m_timer;

private :
	float								Calculate_Confidence(const int& current_Frame, const float&ReliabilityTime);
public:
	bool								m_update;					//SencorSightによって更新されたかどうか
	int								m_last_SensedTime;	//最後に視認したフレーム(最大でSight_Interval分の誤差がある
	int								m_became_Visible;	//そのオブジェクトが見え始めた時間
	int								m_team_Type;
	Vector3						m_became_Pos;			//見え始めた時の座標
	Vector3						m_sensed_Pos;			//最後に視認した座標
	Vector3						m_sensed_Velocity;		//移動方向
	Vector3						m_sensed_Angle;		//向いていた方向
	bool								m_visibilily;					//視認しているかどうか
	bool								m_line_of_Fire;			//射線が通るかどうか
	bool								m_in_Fov;					//視界内にはいっているかどうか
	ENTITY_ID					m_id;							//ID
	ExpectationAct				m_action;						//何をしていたかの行動予測（最後に確認した行動）
	int								m_expectation_Time;	//行動予測をした最後の時間
	float								m_hp;							//HP
	arive_status					m_status;						//生存状態
public:
	inline Character_Record() : m_last_SensedTime(-1), m_became_Visible(-1), m_team_Type(-1), m_became_Pos(Vector3(.0f, .0f, .0f)),
		m_sensed_Angle(Vector3(.0f, .0f, .0f)), m_sensed_Pos(Vector3(.0f, .0f, .0f)), m_sensed_Velocity(Vector3(.0f, .0f, .0f)), m_visibilily(false),
		m_line_of_Fire(false),m_id(game_id::id_error), m_action(), m_expectation_Time(-1), m_status(arive_status::i_dont_now)
		, m_delete_Flg(false), m_timer(-1), m_hp(-1.0f), m_in_Fov(false)
	{	}

	bool	Can_Use(const int& current_Frame, const int& ReliabilityTime, float* Reliability);
	bool	Update()
	{
		if (!m_delete_Flg)
			return false;
		m_timer--;
		if (m_timer <= 0)
			return true;
		
		return false;
	}
	void	Delete()
	{
		m_delete_Flg = true;
		m_timer = m_delete_Time;
	}
	/**
	*@brief 削除フラグがたっているかどうか
	*/
	inline bool	Is_Delete(){ return m_delete_Flg; }
};



class MapObject_Record
{
public:
	int						m_last_SensedTime;	//最後に視認したフレーム(最大でSight_Interval分の誤差がある
	int						m_became_Visible;		//そのオブジェクトが見え始めた時間
	Vector3				m_became_Pos;			//見え始めた時の座標
	Vector3				m_sensed_Pos;			//最後に視認した座標
	Vector3				m_sensed_Velocity;		//移動方向
	Vector3				m_sensed_Angle;		//向いていた方向
	bool						m_visibilily;					//視認しているかどうか
	bool						m_line_of_Fire;			//射線が通るかどうか
	bool						m_in_Fov;					//視界内にはいっているかどうか
	ENTITY_ID			m_id;							//ID
	mapobjcet_type	m_object_type;			//type
private :
	float	Calculate_Confidence(const int& current_Frame, const float&ReliabilityTime);
public:
	bool	Can_Use(const int& current_Frame, const float& ReliabilityTime, float* Reliability);
	MapObject_Record() : m_last_SensedTime(-1), m_became_Visible(-1), m_sensed_Pos(Vector3(0, 0, 0)),
		m_became_Pos(Vector3(0, 0, 0)), m_sensed_Angle(Vector3(0, 0, 0)), m_sensed_Velocity(Vector3(0, 0, 0)),
		m_visibilily(false), m_id(game_id::id_error), m_object_type(mapobjcet_type::error)
	{	}
	bool	Can_Use(const int& current_Frame, const int& ReliabilityTime, float* Reliability);
};

struct Threat_OutSight
{
	ENTITY_ID m_id;
	Vector3	m_vec;
	Threat_OutSight(const ENTITY_ID& id, const  Vector3& vec) : m_id(id), m_vec(vec){}
};
enum class censor_type :int
{
	none = -1,
	sight,
	hearing,
	attack_me,
	num_censor,
};
/**
*@brief プランニングを行ったcensorObjectと対象EntityIDを一時的に保存しておくクラス
*@note 短い時間でPlanning関数が呼ばれ過ぎないようにするための一時記憶
*/
class Temporary_PlanningMemory
{
	static  const int max_memory = 30;
	struct MemoryObject
	{
		bool				m_active;
		censor_type m_sender_type;		//プランニングを要求した送り主
		ENTITY_ID	m_rock_ID;
		int				m_registar_Frame;	//登録されたフレーム
		MemoryObject() : m_sender_type(censor_type::none), m_rock_ID(game_id::id_error),
			m_registar_Frame(-1), m_active(false)
		{}
	};
private:
	std::array<MemoryObject, max_memory> m_memory_Array;
	int																m_delete_Frame[(int)censor_type::num_censor];	//
	int																m_current_Regist;
	const NPC_Brain*										m_holder;

public:
	Temporary_PlanningMemory(const NPC_Brain*);
	bool	Registar(const censor_type& type, const ENTITY_ID& register_ID);
	void	Update(const int& current_Frame);
	/**
	*@brief 重複チェック
	*@return 重複していれば-1 登録できる場合は一番近い空きのIndex
	*/
	int	Is_Register(const censor_type& type, const ENTITY_ID& register_ID);

	bool	Remove_Array(const ENTITY_ID& remove_ID);
};

/**
*@brief 一連の行動の成功、失敗にかんする記憶
*/
class Goal_Result
{
	enum p_result{NONE =-1,COMPLETED,FAILED};
	static const int max_memory = 30;
	struct MemroyObject
	{
		bool				m_active;
		ENTITY_ID	m_id;
		p_result		m_result;
		int				m_registar_Frame;
		MemroyObject() :m_active(false),m_id(ENTITY_ID::id_error),
								m_result(p_result::NONE), m_registar_Frame(-1)
		{}
	};

private :
	std::array<MemroyObject, max_memory> m_memory_Array;
	int																m_active_Frame;
	int																m_current_Regist;
	const NPC_Brain*										m_holder;
public :
	Goal_Result(NPC_Brain* holder, const int& active_frame);
	/**
	*@param1 nullptrではない場合＆＆空きがある場合に要素番号を返す
	*@return 登録されていたらtrue
	*/
	bool	Is_Register(const ENTITY_ID& check_id, int* iterator = nullptr);
	/**
	*@brief 登録する
	*@param[bool] 成功がtrue　失敗がfalse
	*/
	bool Register(const ENTITY_ID& register_id, const bool&	result);
	void	Update(const int& current_Frame);
	/**
	*@brief 登録解除
	*/
	bool	Remove_Array(const ENTITY_ID& remove_id);
};

class MEMORY
{
private :
	
public:
	std::map<ENTITY_ID, Character_Record*>					m_character_Record;		//フィールドのオブジェクトに対する情報
	std::map<ENTITY_ID, MapObject_Record*>					m_mapobject_Record;		//マップのオブジェクトに対する記憶
	std::vector<Threat_OutSight>										m_threat_Outsight;			//視界外の脅威に対する記憶container
	Temporary_PlanningMemory*											m_planning_Memory;		//プランニングした結果ターゲットとなったENTITYを一時的に格納しておく記憶(Censor_Sightなどで使用)
	Goal_Result*																	m_goal_Result;				//最終ゴールが失敗、成功したときの情報を一時的に格納しておく記憶領域（経路探索が失敗したときなどにそのENTITY_IDを格納しておくなど :GoalEvalutorなどで使用）
public : 
	//std::list<Attribute<goal_Tag>>		past_Goal;				//以前のゴール（最終か逐一記録するかは未定）
	void	All_Clear();
	void	All_Release();
public:
	/**
	*@brief		第一引数のキャラクターを記憶しているかどうか確認、
	記憶していない場合は領域を確保する
	*@param1	確認するキャラクター情報
	*/
	void	Is_Record_Character(const ENTITY_ID& check_object);
	void	Is_Record_MapObject(const ENTITY_ID& check_object);
	bool Is_Record_Threat_OutSight(const ENTITY_ID& check_Object);

	MapObject_Record* Get_MapObjectMemory(const ENTITY_ID& id, const int& reliability_Time, float*reliability);
	Character_Record* Get_CharacterMemory(const ENTITY_ID& id, const int& reliability_Time, float*reliability);
	bool	Registar_Threat_OutSight(const ENTITY_ID& id, const Vector3& threar_Vec);
	bool	Erase_Threar_OutSight(const ENTITY_ID& erase_Id);
	std::vector<Threat_OutSight>*const  Get_Threat_OutSightContainer(){ return &m_threat_Outsight; }
	Threat_OutSight*const  Get_Threat_OutSight (const ENTITY_ID& id);
	/**
	*@brief キャラクターの更新フラグをfalseに
	*/
	void	Reset_UpdateFlg_CharacterRecord();

};


#endif
