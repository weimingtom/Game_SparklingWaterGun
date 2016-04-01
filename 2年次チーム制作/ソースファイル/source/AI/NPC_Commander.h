#pragma once

class NPC_Brain;
class NPC_CommanderManager;
struct FriendIconMsgInfo;

class NPC_Commander
{
	friend class NPC_CommanderManager;
private:
	int				m_teamNo;
	BlackBoard*	m_black_Board;
	std::map<NPC_Brain*, ENTITY_ID>	m_entity_IDMap;

private:
	NPC_Commander(const int team_No);

private :
	

	/**
	*@brief HELPMessageに対して一番適したEntityに対してMessage送信
	*/
	bool	DisPatch_HelpMessage(GameMessage* msg);
	/**
	*@brief COME_ONMessageに対して一番適したEntityに対してMessage送信
	*/
	bool	DisPatch_ComeON_Message(GameMessage* msg);
	/**
	*@Messageを変換
	*/
	message_priority	Convert_Message(FriendIconMsgInfo* msg);

public:
	~NPC_Commander();
	WriteSpace*	Get_WriteSpace(NPC_Brain* owner_Brain);
	ENTITY_ID	Get_EntityID(NPC_Brain* owner_Brain);
	bool				Set_Entity_ID(NPC_Brain* owner_Brain, const ENTITY_ID id);
	bool				Registar_BlackBoard(const NPC_Brain* owner_Brain);
	bool				HandleMessage(GameMessage* msg);
};

class NPC_CommanderManager
{
private:
	int														m_num_Team;
	std::vector<NPC_Commander*>		m_commander_vecter;
	
private :
	static NPC_CommanderManager*	m_instance;
	NPC_CommanderManager();
	~NPC_CommanderManager();
public :
	static void	Create_Instance();
	static NPC_CommanderManager* Get_Instance();
	static void	Release_Instance();
	
public :
	void	Initialize(const int num_team);
	bool	Set_ENTITYID(NPC_Brain* set_EntityBrain,const int team_type, const ENTITY_ID id);
	/**
	*@brief Brainクラスを登録（初期設定）
	*/
	bool	Register_NPC(NPC_Brain* owner_Brain, const ENTITY_ID default_ID, const int team_No);
	WriteSpace*	Get_WriteData(NPC_Brain* onwer_Brain, const int team_No, 
									ENTITY_ID* ret_ENTITYID = nullptr);
	bool	HandleMessage(const int send_team, GameMessage* msg);
	void Clear_MyWriteSpece(NPC_Brain* owner, const int team_no);
	void	Test();
};

#define npc_commander (NPC_CommanderManager::Get_Instance())
