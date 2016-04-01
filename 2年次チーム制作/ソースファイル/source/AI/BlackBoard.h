#pragma once

/**
*@note �������Ă���Message�̗D�揇��enum_class
*@note �傫���قǗD�揇�ʂ������Ȃ�
*/
enum class message_priority
{
	none,
	follow_friend,
	rescue_friend,
};

struct WriteSpace
{
	ENTITY_ID				m_rescue_Friend;		//MedicalFriend�̃^�[�Q�b�g�ƂȂ��Ă���EntityID
	ENTITY_ID				m_follow_Friend;			//Follow���Ă��閡��
	message_priority		m_message_priority;	//���b�Z�[�W�̗D�揇��

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
//	int										m_num_Team;	//�`�[����
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
//	*@brief �`�[��������BlackBoard�쐬
//	*@param	�o�^����`�[����
//	*/
//	bool	Create_BlackBoard(const int num_Team);
//	/**
//	*@brief BltackBoard��NPC�o�^
//	*@param �o�^����NPC_Brain,
//	*@param �`�[��No
//	*/
//	bool	Register_NPC(const NPC_Brain* brain, const int team_num);
//	/**
//	*@brief BlackBoard�S�J��
//	*/
//	void	Release();
//	WriteSpace*	GetWriteSpace(const NPC_Brain* owner_Brain, const int team_num);
//
//	void	Test();
//};

#define blackboard (BlackBoardManager::GetInstance())
