#include	"..\All.h"


#pragma region BlackBoard

BlackBoard::BlackBoard() : m_num_Member(0)
{

}

BlackBoard::~BlackBoard()
{
	for (auto& it : m_board)
	{
		SAFE_DELETE(it.second);
	}
}

bool	BlackBoard::Register(const NPC_Brain* regist_NPC)
{
	if (m_board.find(regist_NPC) != m_board.end())
		return false;
	m_board.insert(std::pair<const NPC_Brain*, WriteSpace*>
								(regist_NPC, new WriteSpace));
	return true;

}

WriteSpace*	BlackBoard::Get_WriteSpace(const NPC_Brain* owner_Brain)
{
	if (m_board.find(owner_Brain) == m_board.end())
		return nullptr;

	return m_board[owner_Brain];
}

#pragma endregion

//#pragma region Manager
//
//BlackBoardManager* BlackBoardManager::m_instance = nullptr;
//
//BlackBoardManager::BlackBoardManager() : m_num_Team(0)
//{
//
//}
//
//void	BlackBoardManager::Create_Instance()
//{
//	if (!m_instance)
//		m_instance = new BlackBoardManager();
//}
//
//void	BlackBoardManager::Release_Instance()
//{
//	SAFE_DELETE(m_instance);
//}
//
//BlackBoardManager* BlackBoardManager::GetInstance()
//{
//	return m_instance;
//}
//
//BlackBoardManager::~BlackBoardManager()
//{
//	for (auto it : m_blackBoard_vector)
//	{
//		SAFE_DELETE(it);
//	}
//}
//
//bool	BlackBoardManager::Create_BlackBoard(const int num_team)
//{
//	if (num_team <= 0)
//		return false;
//	m_blackBoard_vector.clear();
//	m_num_Team = num_team;
//	m_blackBoard_vector.reserve(m_num_Team);
//	for (int i = 0; i < m_num_Team; i++)
//	{
//		m_blackBoard_vector.push_back(new BlackBoard());
//	}
//	return true;
//}
//
//bool	BlackBoardManager::Register_NPC(const NPC_Brain* brain, const int team_num)
//{
//	return m_blackBoard_vector[team_num]->Register(brain);
//}
//
//void	BlackBoardManager::Release()
//{
//	for (auto it : m_blackBoard_vector)
//	{
//		SAFE_DELETE(it);
//	}
//	m_num_Team = 0;
//}
//
//WriteSpace*	BlackBoardManager::GetWriteSpace(const NPC_Brain* owner_Brain, const int team_num)
//{
//	if (team_num >= m_num_Team)
//		return nullptr;
//	return m_blackBoard_vector[team_num]->Get_WriteSpace(owner_Brain);
//}
//
//void	BlackBoardManager::Test()
//{
//}

//#pragma endregion
