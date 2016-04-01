#include	"..\All.h"
#include	"..\FriendMessage.h"
#pragma region Commander

NPC_Commander::NPC_Commander(const int team_No) : m_teamNo(team_No)
{
	m_black_Board = new BlackBoard();
}

NPC_Commander::~NPC_Commander()
{
	SAFE_DELETE(m_black_Board);
}

bool	NPC_Commander::Registar_BlackBoard(const NPC_Brain* owner_Brain)
{
	return m_black_Board->Register(owner_Brain);
}

WriteSpace* NPC_Commander::Get_WriteSpace(NPC_Brain* owner_Brain)
{
	return m_black_Board->Get_WriteSpace(owner_Brain);
}

bool	NPC_Commander::Set_Entity_ID(NPC_Brain* owner_Brain, const ENTITY_ID id)
{
	auto& it = m_entity_IDMap.find(owner_Brain);
	if (it == m_entity_IDMap.end())
	{
		m_entity_IDMap.insert(std::pair<NPC_Brain*, ENTITY_ID>(owner_Brain, id));
		return true;
	}
	it->second = id;
	return true;
}

ENTITY_ID NPC_Commander::Get_EntityID(NPC_Brain* owner_Brain)
{
	auto& it = m_entity_IDMap.find(owner_Brain);
	if (it == m_entity_IDMap.end())
	{
		MyAssert(false, "Commander::GetEntityID");
		return game_id::id_error;
	}
	return it->second;
}

bool	NPC_Commander::HandleMessage(GameMessage* msg)
{
	FriendIconMsgInfo* msg_info = static_cast<FriendIconMsgInfo*>(msg->exinfo);
	switch (msg_info->type)
	{
	case FriendMessageIcon::MsgType::_HELP :
		return DisPatch_HelpMessage(msg);
		break;
		
	case FriendMessageIcon::MsgType::_COME_ON :
		return  DisPatch_ComeON_Message(msg);
		break;

	default :

		break;
	}
	return false;
}

message_priority	NPC_Commander::Convert_Message(FriendIconMsgInfo* msg)
{
	switch (msg->type)
	{
	case FriendMessageIcon::MsgType::_HELP : 
	return message_priority::rescue_friend;
			break;
			
	case FriendMessageIcon::MsgType::_COME_ON : 
		return message_priority::follow_friend;
		break;
	}
	return message_priority::none;
}

bool	NPC_Commander::DisPatch_HelpMessage(GameMessage* msg)
{
	FriendIconMsgInfo* msg_info = static_cast<FriendIconMsgInfo*>(msg->exinfo);
	message_priority priority = Convert_Message(msg_info);
	Character* from_Chara = dynamic_cast<Character*>
												(IDMANAGER.GetPointer(msg_info->sender));
	if (!from_Chara)
		return false;
	float max_Score = .0f;
	float most_min_Dist = FLT_MAX;

	NPC_Brain* send_Brain = nullptr;
	ENTITY_ID send_Entity = game_id::id_error;
	for (auto& it : m_entity_IDMap)
	{
		Character* check_Chraracter = dynamic_cast<Character*>(IDMANAGER.GetPointer(it.second));
		if (!check_Chraracter)
			continue;
		if (from_Chara == check_Chraracter)
			continue;

		//óDêÊìxÉ`ÉFÉbÉN
		WriteSpace* write_space = m_black_Board->Get_WriteSpace(it.first);
		if (write_space->m_message_priority > priority)
			continue;
		//Ç∆ÇËÇ†Ç¶Ç∏ãóó£óDêÊ
		Vector3 sender_Pos = ControllObjFunc::GetPos(from_Chara);
		Vector3 check_Pos = ControllObjFunc::GetPos(check_Chraracter);
		float dist_sq = (check_Pos - sender_Pos).LengthSq();
		if (dist_sq <= most_min_Dist)
		{
			send_Brain = it.first;
			most_min_Dist = dist_sq;
			send_Entity = it.second;
		}
	}
	if (send_Brain)
	{
		msg->to = send_Entity;
		send_Brain->HandleMessage(msg);
		return true;
	}
	return false;
}

bool	NPC_Commander::DisPatch_ComeON_Message(GameMessage* msg)
{
	FriendIconMsgInfo* msg_info = static_cast<FriendIconMsgInfo*>(msg->exinfo);
	message_priority priority = Convert_Message(msg_info);
	Character* from_Chara = dynamic_cast<Character*>
		(IDMANAGER.GetPointer(msg_info->sender));
	if (!from_Chara)
		return false;
	float max_Score = .0f;
	float most_min_Dist = FLT_MAX;

	NPC_Brain* send_Brain = nullptr;
	ENTITY_ID send_Entity = game_id::id_error;
	for (auto& it : m_entity_IDMap)
	{
		Character* check_Chraracter = dynamic_cast<Character*>(IDMANAGER.GetPointer(it.second));
		if (!check_Chraracter)
			continue;
		if (from_Chara == check_Chraracter)
			continue;

		//óDêÊìxÉ`ÉFÉbÉN
		WriteSpace* write_space = m_black_Board->Get_WriteSpace(it.first);
		if (write_space->m_message_priority > priority)
			continue;
		//Ç∆ÇËÇ†Ç¶Ç∏ãóó£óDêÊ
		Vector3 sender_Pos = ControllObjFunc::GetPos(from_Chara);
		Vector3 check_Pos = ControllObjFunc::GetPos(check_Chraracter);
		float dist_sq = (check_Pos - sender_Pos).LengthSq();
		if (dist_sq <= most_min_Dist)
		{
			send_Brain = it.first;
			most_min_Dist = dist_sq;
			send_Entity = it.second;
		}
	}
	if (send_Brain)
	{
		msg->to = send_Entity;
		send_Brain->HandleMessage(msg);
		return true;
	}
	return false;
}

#pragma endregion

#pragma region Manager

#pragma region Sigleton

NPC_CommanderManager* NPC_CommanderManager::m_instance = nullptr;

NPC_CommanderManager::NPC_CommanderManager() : 
m_num_Team(0)
{

}

void NPC_CommanderManager::Create_Instance()
{
	if (!m_instance)
		m_instance = new NPC_CommanderManager();
}

NPC_CommanderManager::~NPC_CommanderManager()
{
	for (auto it : m_commander_vecter)
	{
		SAFE_DELETE(it);
	}
}

NPC_CommanderManager* NPC_CommanderManager::Get_Instance()
{
	return m_instance;
}

void	NPC_CommanderManager::Release_Instance()
{
	SAFE_DELETE(m_instance);
}

#pragma endregion

void	NPC_CommanderManager::Initialize(const int num_team)
{
	m_num_Team = num_team;
	for (int i = 0; i < m_num_Team; i++)
	{
		m_commander_vecter.push_back(new NPC_Commander(i));
	}
}

bool	NPC_CommanderManager::Set_ENTITYID(NPC_Brain* set_EntityBrain, const int team_num, const ENTITY_ID id)
{
	if (m_num_Team <= team_num)
		return false;

	return m_commander_vecter[team_num]->Set_Entity_ID(set_EntityBrain, id);
}

bool	NPC_CommanderManager::Register_NPC(NPC_Brain* owner_Brain,
													const ENTITY_ID default_ID,const int team_No)
{
	if (m_num_Team <= team_No)
		return false;

	if (!m_commander_vecter[team_No]->Registar_BlackBoard(owner_Brain))
	{
		MyAssert(false,"Register_NPCÇÃBlackBoardìoò^ê›íË");
	}
	return m_commander_vecter[team_No]->Set_Entity_ID(owner_Brain, default_ID);
}

WriteSpace*	NPC_CommanderManager::Get_WriteData(NPC_Brain* owner_Brain,
								 const int team_No,ENTITY_ID* ret_ENTITYID)
{
	if (m_num_Team <= team_No)
		return false;
	
	if (ret_ENTITYID)
		*ret_ENTITYID = m_commander_vecter[team_No]->Get_EntityID(owner_Brain);
	return m_commander_vecter[team_No]->Get_WriteSpace(owner_Brain);
}

bool	NPC_CommanderManager::HandleMessage(const int send_team, GameMessage* msg)
{
	if (m_num_Team <= send_team)
		return false;
	return m_commander_vecter[send_team]->HandleMessage(msg);
}

void	NPC_CommanderManager::Clear_MyWriteSpece(NPC_Brain* owner, const int team_no)
{
	if (m_num_Team <= team_no)
	{
		MyAssert(false, "Ç±ÇÍÇÕÇ‚ÇŒÇ¢");
		return;
	}
	return m_commander_vecter[team_no]->m_black_Board->ClearSpace(owner);
}

void	NPC_CommanderManager::Test()
{
	int  a = 0;
}

#pragma endregion
