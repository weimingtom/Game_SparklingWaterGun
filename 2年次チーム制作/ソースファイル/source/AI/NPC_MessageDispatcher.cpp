#include	"..\All.h"

AI_MessageDispatcher*	AI_MessageDispatcher::m_instance = nullptr;

AI_MessageDispatcher::AI_MessageDispatcher()
{

}

AI_MessageDispatcher::~AI_MessageDispatcher()
{
	while (!m_delay_ai_Que.empty())
		m_delay_ai_Que.pop();
	while (!m_delay_game_Que.empty())
		m_delay_game_Que.pop();
	
}

void	AI_MessageDispatcher::Initialize()
{

}

void	AI_MessageDispatcher::Release_Instance()
{
	SAFE_DELETE(m_instance);
}

void	AI_MessageDispatcher::Release()
{
}

void	AI_MessageDispatcher::Create_Insatnce()
{
	if (!m_instance)
	{
		m_instance = new AI_MessageDispatcher();
		m_instance->Initialize();
	}
}

AI_MessageDispatcher* AI_MessageDispatcher::Get_Instance()
{
	return m_instance;
}

void AI_MessageDispatcher::Plannning(NPC_Brain* planner_Brain, Character* character,
					std::string* function_Name,int delay)
{
	std::string name(*function_Name + "がプランニングを要求しました\n");
	CONSOLE_PRINT("%s", name.c_str());
	planner_Brain->Log(FormatString("%s", name.c_str()));

	GameMessage msg;
	msg.type = msg_t::replanning;
	msg.from = character->GetId();
	msg.to = msg.from;
	msg.exinfo = (void*)character;
	if (delay <= 0)
	{
		planner_Brain->HandleMessage(&msg);
		return;
	}
	int current_Frame = GAMEMATCHINFO.GetMatchTime();
	m_delay_game_Que.push(DelayMesssageObject<GameMessage>(planner_Brain, msg, receiver_type::brain,current_Frame+ delay,character->GetId()));
	planner_Brain->Log(FormatString("%dフレームで%s宛にDelayMessageが格納されました\n msg_type : %s",
			current_Frame, planner_Brain->character_Name, Convert_Enum(msg.type)));

}

void AI_MessageDispatcher::Demand_Search_Path(NPC_Brain* brain, Character* character,
	const goal_tag& current_Goaltag, int	delay)
{
	PathSearchObject obj;
	obj.request_Brain = brain;
	obj.id = character->GetId();
	obj.request_Frame = GAMEMATCHINFO.GetMatchTime();
	obj.m_request_Goaltag = current_Goaltag;
	
	AI_MESSAGE msg(ai_messagetype::demand_search_path, (void*)&obj);
	navigation_system->HandleMessage(&msg);
	
}

bool	AI_MessageDispatcher::Send_BehaviorMessage(NPC_Brain* send_Brain, Character* character,
	const ai_messagetype& type, void* extra, int	delay)
{
	AI_MESSAGE msg(type, extra);
	if (delay <= 0)
	{
		/*Goal* send_Goal = send_Brain->m_goal_Deque.front();
		int current_Behavior = send_Goal->current_Behavior;
		int num_Behavior = send_Goal->num_Behavior;*/
		if (send_Brain->m_behavior_Manager->Behavior_IsActive())
		{
			return 	send_Brain->m_behavior_Manager->Get_Current_Behavior()->HandleMessage(&msg);
		}
	}
	m_delay_ai_Que.push( DelayMesssageObject<AI_MESSAGE>(send_Brain, msg, 
								receiver_type::behavior, delay + GAMEMATCHINFO.GetMatchTime(),character->GetId()));
	return true;
}

bool	AI_MessageDispatcher::Send_GoalMessage(Goal* send_Goal,
								const ai_messagetype& type, void* extra, int	delay)
{
	AI_MESSAGE msg(type, extra);
	return send_Goal->HandleMessage(&msg);
}

bool	AI_MessageDispatcher::Send_MemoryMessage(NPC_Brain* send_Brain,
	const ai_messagetype& type, void* extra, int	delay)
{
	AI_MESSAGE msg;
	msg.type = type;
	msg.extra_Info = extra;

	return send_Brain->m_memory.HandleMessage(&msg);
}

bool	AI_MessageDispatcher::Send_BrainMessage(NPC_Brain* send_Brain,const ENTITY_ID& from,
	const ENTITY_ID& to, const MSG_TYPE& msg_type, void* extra, int	delay)
{
	GameMessage msg;
	msg.type = msg_type;
	msg.from = from;
	msg.to = to;
	msg.exinfo = extra;
	if (delay <= 0)
	{
		return send_Brain->HandleMessage(&msg);
	}
	int current_Frame = GAMEMATCHINFO.GetMatchTime();
	m_delay_game_Que.push(DelayMesssageObject<GameMessage>(send_Brain, msg, receiver_type::brain, current_Frame + delay,from));
	return true;
}

bool	AI_MessageDispatcher::Send_GoalThinkMessage(NPC_Brain* send_Brain, Character* character,
	const ai_messagetype& type, void* extra_Info, int	delay)
{
	AI_MESSAGE msg;
	msg.type = type;
	msg.extra_Info = extra_Info;

	if (delay <= 0)
	{
		return send_Brain->m_think->HandleMessage(&msg);
	}
	m_delay_ai_Que.push(DelayMesssageObject<AI_MESSAGE>(send_Brain, msg, receiver_type::goal_think, delay + GAMEMATCHINFO.GetMatchTime(),character->GetId()));
	return true;
}

void	AI_MessageDispatcher::Update()
{
	int current_Frame = GAMEMATCHINFO.GetMatchTime();
	while (!m_delay_ai_Que.empty() && m_delay_ai_Que.top().m_dispatch_Time == current_Frame)
	{
		DelayMesssageObject<AI_MESSAGE>* send_Info = &m_delay_ai_Que.top();
		//IDのerrorチェック
		Character* character = dynamic_cast<Character*>(IDMANAGER.GetPointer(send_Info->m_id));
		if (!character)
		{
			break;
		}
		switch (send_Info->m_type)
		{
		case  receiver_type::behavior : 
			Send(send_Info->send_Brain->m_behavior_Manager->Get_Current_Behavior(),
				&send_Info->m_msg);
			break;
		case receiver_type::goal :
			Send(send_Info->send_Brain->m_goal_Deque.front(), &send_Info->m_msg);
			break;

		case receiver_type::memory :
			Send(&send_Info->send_Brain->m_memory, &send_Info->m_msg);
			break;

		case receiver_type::goal_think :
			Send(send_Info->send_Brain->m_think, &send_Info->m_msg);
			break;
		}
	/*	send_Info->send_Brain->Log(FormatString("%dフレームで%sにDelayMessageが送信されました\nMessageType : %s",
			current_Frame, send_Info->send_Brain->character_Name, Convert_Enum(send_Info->m_msg.type)));*/
		m_delay_ai_Que.pop();
	}


	while (!m_delay_game_Que.empty() && m_delay_game_Que.top().m_dispatch_Time == current_Frame)
	{
		DelayMesssageObject<GameMessage>* send_Info = &m_delay_game_Que.top();

		switch (send_Info->m_type)
		{
		case  receiver_type::brain :
			Send(send_Info->send_Brain,
				&send_Info->m_msg);
			break;
	
		default :

			break;
		}
		send_Info->send_Brain->Log(FormatString("%dフレームで%sにDelayMessageが送信されました\nMessageType : %s",
			current_Frame, send_Info->send_Brain->character_Name, Convert_Enum(send_Info->m_msg.type)));
		m_delay_game_Que.pop();
	}
}

bool	AI_MessageDispatcher::Send_PathPlannerMessage(NPC_Brain* send_Brain, AI_MESSAGE* msg)
{
	return send_Brain->m_path_Planner->HandleMessage(msg);
}

char* AI_MessageDispatcher::Convert_Enum(const ai_messagetype& type)
{
	if (type == ai_messagetype::error)
		MyAssert(false, "エラーMSG送信しようとしました");

	static char* enum_char[] =
	{
		"search_path_ok",
		"demand_search_path",
		"set_route",
		"set_currentGoalPos",
		"set_LookVec",
		"update_memory",
		"lock_goal",
		"chracter_Die",
		"evalute_currentGoalPos",
	};
	return enum_char[enum_cast(type)];
}

char* AI_MessageDispatcher::Convert_Enum(const MSG_TYPE& type)
{
	if (type == MSG_TYPE::error_msg)
		MyAssert(false, "エラーMSG送信しようとしました");

	static char* enum_char[] =
	{
		"env_sound",
		"search_path_ok",
		"search_path_retry",
		"memory_update",
		"chr_table_state_change",
		"chr_table_state_change_low_lvl",
		"lock_goal",
		"to_chr_state",
		"add_bullet",
		"replanning",
		"damage_hit"
	};
	return enum_char[enum_cast(type)];
}
