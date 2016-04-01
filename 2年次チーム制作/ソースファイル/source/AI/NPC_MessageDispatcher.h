#ifndef __AIMESSAGEDISPATCHER_H__
#define __AIMESSAGEDISPATCHER_H__

#include	<queue>
class Character;
class NPC_Brain;
class Goal;
struct MemoryMessage;
class GameMessage;
_ENUM_BEGIN(receiver_type,RECEIVER_TYPE)
behavior,
brain,
goal,
goal_think,
memory

_ENUM_END(receiver_type, RECEIVER_TYPE)

template<class M>
class DelayMesssageObject
{
public:
	receiver_type	m_type;
	M						m_msg;
	int					m_dispatch_Time;
	NPC_Brain*		send_Brain;
	ENTITY_ID		m_id;
		DelayMesssageObject(NPC_Brain* brain, const M& msg, const receiver_type& type, const int send_Time,const ENTITY_ID id)
			: m_msg(msg), m_type(type), m_dispatch_Time(send_Time), send_Brain(brain), m_id(id)
	{	}

	//bool	operator < (const DelayMesssageObject& obj)
	//{
	//	return (m_dispatch_Time < obj.m_dispatch_Time) ? true : false;
	//}
};

////比較用関数オブジェクト
template<class M>
struct DelayMsg_Sort
{
	bool	operator () (const DelayMesssageObject<M>& left, const DelayMesssageObject<M>& right)
	{
		return (left.m_dispatch_Time < right.m_dispatch_Time);
	}
};

class AI_MessageDispatcher
{
private :
	static AI_MessageDispatcher* m_instance;
	std::priority_queue<DelayMesssageObject<AI_MESSAGE>,
		std::vector<DelayMesssageObject<AI_MESSAGE>>, DelayMsg_Sort<AI_MESSAGE>>
		m_delay_ai_Que;
	
	std::priority_queue<DelayMesssageObject<GameMessage>,
		std::vector<DelayMesssageObject<GameMessage>>, DelayMsg_Sort<GameMessage>>
		m_delay_game_Que;
	
private :
	AI_MessageDispatcher();
	
	void	Initialize();
	char* Convert_Enum(const ai_messagetype& type);
	char* Convert_Enum(const MSG_TYPE& type);
public :
	static void	Create_Insatnce();
	void		Release();
	static	void	Release_Instance();
	static	AI_MessageDispatcher*	Get_Instance();
	~AI_MessageDispatcher();

public :
	void		Update();

public :	//Dispatch
	 void		Plannning(NPC_Brain* plannner_Brain, Character* character,
								std::string* function_Name,int	delay = 0);

	 void		Demand_Search_Path(NPC_Brain* brain, Character* character,
		 const goal_tag& current_Goaltag, int	delay = 0);

	 bool		Send_BehaviorMessage(NPC_Brain* send_Brain, Character* character,
			 const ai_messagetype& type, void* extra, int	delay = 0);

	 bool		Send_GoalMessage(Goal* send_Goal, const ai_messagetype& type, void* extra, int	delay = 0);

	 bool	Send_MemoryMessage(NPC_Brain* send_Brain, const ai_messagetype& type,
											void* extra, int	delay = 0);
	
	 bool	Send_BrainMessage(NPC_Brain* send_Brain, const ENTITY_ID& from,
								 const ENTITY_ID& to, const MSG_TYPE& msg_type, void* extra, int	delay = 0);
	
	 bool	Send_GoalThinkMessage(NPC_Brain* send_Brain, Character* character,
								const ai_messagetype& type, void* extra_Info, int	delay = 0);

	 bool	Send_PathPlannerMessage(NPC_Brain* send_Brain , AI_MESSAGE*	msg);

};

#define ai_message (AI_MessageDispatcher::Get_Instance())

template<class T,class M>
bool	Send(T*receiver, M* msg)
{
	return receiver->HandleMessage(msg);
}

#endif
