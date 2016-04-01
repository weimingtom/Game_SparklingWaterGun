#ifndef __NPC_CENSOR_H__
#define __NPC_CENSOR_H__

class NPC_Brain;
class Character;
class WorkingMemory;
class ControllObject;
struct DamageParam;
struct ExpectationAct;
class Env_Sound;
class NPCParam;

#include "iextreme.h"
#include "../GameSystem/GameBase.h"

class NPC_Censor
{
protected :
	const int		m_update_Interval;		//更新間隔
	const int		m_reaction_Speed;		//反応速度

public :
	NPC_Censor(const int update_Interval, const int reaction_Speed) :
		m_update_Interval(update_Interval), m_reaction_Speed(reaction_Speed)
	{	}
	virtual ~NPC_Censor(){}
	virtual void	Update(NPC_Brain* brain, Character* character) = 0;
};


struct Msg_AttackMe
{
	DamageParam damage_Param;
	int				m_current_Frame;
	Msg_AttackMe(const DamageParam& param) : damage_Param(param), m_current_Frame(0)
	{	}
	void	Update(){ m_current_Frame++; }
	
};

class Censor_AttackMe : public NPC_Censor
{
	friend class NPC_Brain;
private :
	std::list<Msg_AttackMe*>		m_reaction_TargetList;
private :
	/**
	*@brief NPC_Brainにプランニング要求のMessageを送信するかどうかを判定する
	*/
	bool	Judge_Dispatch_PlanningMsg(NPC_Brain* brain, Character* character,Msg_AttackMe* judge_Msg);
	/**
	*@brief IDの重複をチェックする
	*/
	bool	Check(const DamageParam& param);
	/**
	*@brief 自分がダメージを受けた敵を登録する
	*@param 攻撃してきた敵のID
	*/
	void	Register(NPC_Brain* brain, const DamageParam* target_ID);
	/**
	*@記憶情報(action)を書き換える
	*/
	void	Rewrite_Memory(NPC_Brain* brain,const Msg_AttackMe& msg);
	void	Update_Counter(NPC_Brain* brain, Character* character);
public :
	Censor_AttackMe(const int reaction_Speed);
	~Censor_AttackMe();
	void			Release();
public :
	void		Update(NPC_Brain* brain, Character* character);
};

class Censor_Sight : public NPC_Censor
{
	friend class NPC_Brain;
	/**
	*@brief 透視できるかどうか判定する関数（たぶんそのうち変更する）
	*/
	bool	Is_See_Through(const MAP_OBJ_TYPE& type){ return true; }
private :
	const float		m_fov;					//視野角
	const float		m_sight_Range;	//視認距離
	bool					look;

private :
	/**
	*@brief フィールドに存在しなくなったキャラクターに対して記憶の削除Flgを起動する
	*/
	void		Delete_NotField_CharacterRecord(NPC_Brain* brain);
	bool		Look_Character(NPC_Brain* brain, Character* chacracter);
	bool		Look_MapObject(NPC_Brain* brain, Character* character);
	void		Update_TemporaryMemory_Character(NPC_Brain* brain, Character* character);
	bool		Judge_Attack(NPC_Brain* brain, Character* character, Character_Record* record);
	bool		Judge_Medical(NPC_Brain* brain, Character* character, Character_Record* record);
	bool		Judge_Dispatch_PlanningMsg(NPC_Brain* brain, Character* character,Character_Record* record);
	bool		Judge_Dispatch_PlanningMsg(NPC_Brain* brain, Character* character, MapObject_Record* record);

	arive_status Cul_Status(Character* target);
	void	Update_Sight(NPC_Brain* brain, Character* character);

public :
	Censor_Sight(const int update_Interval, const float fov,const float sight_Range,const int reaction_Speed);
	~Censor_Sight();
	inline	void	Look(){ look = true; }
public :
	void		Update(NPC_Brain* brain, Character* chacracter);
	bool		SightCheck_and_Line_of_Fire(Character* character, const Vector3& target, bool* visibility, 
																bool* line_of_Fire,bool* in_Fov, int team_type);
};



class Censor_FriendMessage : public NPC_Censor
{
private:
	bool											m_isActive;							//受け取ったメッセージが現在のゴールに適応されている場合trueとなる
	ENTITY_ID								m_send_Entity;					//メッセージを送信してきたEntity
	FriendMessageIcon::MsgType	m_process_MessageType;	//	処理しているメッセージタイプ

public:
	Censor_FriendMessage(const int reaction_Speed);
	~Censor_FriendMessage();

public :
	void	Update(NPC_Brain* brain, Character* character);
	bool	Activate();
	bool	ProcessMessage(NPC_Brain* brain, GameMessage* msg);
	/**
	*@brief メッセージに適応されたゴールが設定されたかどうかを調べる
	*@param 調停された最終ゴール
	*/
	bool	Check_ThinkingResult(NPC_Brain* brain,FinalGoal_Evalutor* final_goal);
	__forceinline ENTITY_ID	Get_SenderID(){ return m_send_Entity; }
	__forceinline const FriendMessageIcon::MsgType	GetFriendMessageType(){ return m_process_MessageType; }
	__forceinline void				Clear(){ m_isActive = false; m_send_Entity = game_id::id_error; m_process_MessageType = FriendMessageIcon::MsgType::_ERROR; }
};


#pragma region NotUse

//struct Important_Sound
//{
//	SOUND_TYPE					m_type;					//タイプ
//	Vector3							m_source_Pos;		//発生源
//	float									m_power;				//音の大きさ
//	Important_Sound(const SOUND_TYPE& type, const Vector3& pos, const float& power) :
//		m_type(type), m_source_Pos(pos), m_power(power)
//	{	}
//	inline Important_Sound() : m_type(SOUND_TYPE::error), m_source_Pos(Vector3(.0f, .0f, .0f)),
//		m_power(-1.0f)
//	{	}
//	inline void	Set(const SOUND_TYPE& type, const Vector3& pos, const float& power)
//	{
//		m_power = power; m_source_Pos = pos ; m_type = type;
//	}
//
//	inline void	Clear()
//	{
//		m_source_Pos = Vector3(.0f, .0f, .0f); m_power = -1.0f; m_type = SOUND_TYPE::error;
//	}
//};

//class Censor_Hearing : public NPC_Censor
//{
//private :
//	std::vector<Env_Sound*>	m_transact_vector;	//新しく聞いた音を処理するためのコンテナ
//	Important_Sound					m_most_ImportantSound;
//	bool										m_activate;				//プランニング要求msg
//	int										m_dispatch_Timer;
//private :
//	void	Hearing(NPC_Brain* brain, Character* chacracter);
//	bool	Transact(NPC_Brain* brain, Character* chacracter, const Vector3& my_Pos);
//	float	Get_Range(const SOUND_TYPE& type, const NPCParam& param);
//	float Get_Rate(const SOUND_TYPE& type,NPC_Brain* brain);
//	char* Convert_SoundType(const SOUND_TYPE& type);
//public :
//	Censor_Hearing(const int update_Interval, const int reaction_Speed);
//	~Censor_Hearing();
//
//	void Update(NPC_Brain* brain, Character* chacracter);
//	inline  Important_Sound& Get_MostImportant()  { return m_most_ImportantSound; }
//};


#pragma endregion

#endif
