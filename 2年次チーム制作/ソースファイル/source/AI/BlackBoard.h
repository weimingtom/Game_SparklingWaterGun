#pragma once

/**
*@note 処理しているMessageの優先順位enum_class
*@note 大きいほど優先順位が高くなる
*/
enum class message_priority
{
	none,
	follow_friend,
	rescue_friend,
};

struct WriteSpace
{
	ENTITY_ID				m_rescue_Friend;		//MedicalFriendのターゲットとなっているEntityID
	ENTITY_ID				m_follow_Friend;			//Followしている味方
	message_priority		m_message_priority;	//メッセージの優先順位

	FORCEINLINE WriteSpace() :m_rescue_Friend(ENTITY_ID::id_error), 
		m_follow_Friend(ENTITY_ID::id_error), m_message_priority(message_priority::none)
	{}
	FORCEINLINE void	Clear(){ m_follow_Friend = ENTITY_ID::id_error; m_rescue_Friend = ENTITY_ID::id_error; }
};

class BlackBoard
{
	friend class NPC_Commander;
private :
	int																	m_num_Member;
	std::map<const NPC_Brain*, WriteSpace*>	m_board;
	
private :
	BlackBoard();
public :
	~BlackBoard();
	bool	Register(const NPC_Brain* regist_NPC);
	WriteSpace*	Get_WriteSpace(const NPC_Brain* owner_Brain);
	FORCEINLINE void	ClearSpace(NPC_Brain* owner){ m_board[owner]->Clear(); }
};

//class BlackBoardManager
//{
//private :
//	int										m_num_Team;	//チーム数
//	std::vector<BlackBoard*>	m_blackBoard_vector;
//
//private :
//	BlackBoardManager();
//	static BlackBoardManager *m_instance;
//
//public :
//	static void	Create_Instance();
//	static void	Release_Instance();
//	static BlackBoardManager*	GetInstance();
//	~BlackBoardManager();
//
//public :
//	/**
//	*@brief チーム数分のBlackBoard作成
//	*@param	登録するチーム数
//	*/
//	bool	Create_BlackBoard(const int num_Team);
//	/**
//	*@brief BltackBoardにNPC登録
//	*@param 登録するNPC_Brain,
//	*@param チームNo
//	*/
//	bool	Register_NPC(const NPC_Brain* brain, const int team_num);
//	/**
//	*@brief BlackBoard全開放
//	*/
//	void	Release();
//	WriteSpace*	GetWriteSpace(const NPC_Brain* owner_Brain, const int team_num);
//
//	void	Test();
//};

#define blackboard (BlackBoardManager::GetInstance())
