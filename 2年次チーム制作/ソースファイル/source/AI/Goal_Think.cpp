
#include	"../All.h"

#define GOLTHINK_CONSOLE 1

Goal_Think::Goal_Think(NPC_Brain* holder) :
m_holder(holder), m_current_FinalGoal(nullptr)
{
	m_goal_Evalutor.reserve(10);
}

Goal_Think::~Goal_Think()
{
	for (auto it :m_goal_Evalutor)
	{
		SAFE_DELETE(it);
	}
	m_goal_Evalutor.clear();
}

Action_Node* Goal_Think::Thinking(Character* character)
{
//	ProgramSpeedCheck::Begin();
#if GOLTHINK_CONSOLE
	CONSOLE_PRINT("%s‚ÌÅIƒS[ƒ‹‚ª•]‰¿‚³‚ê‚Ü‚µ‚½\n", m_holder->character_Name);
#endif
	m_holder->Log(FormatString("%s‚ÌÅIƒS[ƒ‹‚ª•]‰¿‚³‚ê‚Ü‚µ‚½\r\n", m_holder->character_Name));
	FinalGoal_Evalutor*direction;
	float max_Score = .0f;
	int current_Frame = GAMEMATCHINFO.GetMatchTime();
	ProgramSpeedCheck::Begin();
	for (auto it : m_goal_Evalutor)
	{
		
		if (it->m_lock.Is_Bind(current_Frame))
		{
#if GOLTHINK_CONSOLE
			CONSOLE_PRINT("%s‚ÍLock‚³‚ê‚Ä‚¢‚Ü‚·\n", it->Get_EvaluteGoalName());
#endif
			m_holder->Log(FormatString("%s‚ÍLock‚³‚ê‚Ä‚¢‚Ü‚·\r\n", it->Get_EvaluteGoalName()));
			continue;
		}

		//ƒ^[ƒQƒbƒg‚ðƒŠƒZƒbƒg‚·‚é
		it->Reset_Target();
		
		float score = it->Calculate_Score(m_holder, character);
		
		char* name = it->Get_EvaluteGoalName();
		ProgramSpeedCheck::Begin();
#if GOLTHINK_CONSOLE
		CONSOLE_PRINT("%s‚Ì•]‰¿’l %5f\n",name, score);
#endif
		DWORD time = ProgramSpeedCheck::GetTime();
		m_holder->Log(FormatString("%s‚Ì•]‰¿’l %5f\r\n", it->Get_EvaluteGoalName(), score));
		if (max_Score < score)
		{
			direction = it;
			max_Score = score;
		}
		
	}
	DWORD time = ProgramSpeedCheck::GetTime();
	if (max_Score == 0)
	{
		CONSOLE_PRINT("‘S‚Ä‚ÌƒS[ƒ‹‚Ì•]‰¿’l‚ª0‚Å‚µ‚½");
		m_holder->Log(FormatString("‘S‚Ä‚ÌƒS[ƒ‹‚Ì•]‰¿’l‚ª0‚Å‚µ‚½\r\n"));
		return nullptr;
	}
	//ÅIƒS[ƒ‹ƒZƒbƒg
	m_current_FinalGoal = direction;
	//DWORD time = (float)ProgramSpeedCheck::GetTime() ;
	return new Action_Node(direction->Get_EvaluteGoal());
}

bool	Goal_Think::HandleMessage(AI_MESSAGE * msg)
{
	switch (msg->type)
	{
	case ai_messagetype::lock_goal:
		m_current_FinalGoal->m_lock.Bind(GAMEMATCHINFO.GetMatchTime());
		return true;
		break;

	case ai_messagetype::set_messsage_bias : 
		Set_FriendMessageBias(*static_cast<int*>(msg->extra_Info));
		return true;
		break;

	}
	return false;
}

bool	Goal_Think::Set_FriendMessageBias(const int message_type)
{
	switch (message_type)
	{
	case FriendMessageIcon::MsgType::_COME_ON:
		return false;
		break;

	case FriendMessageIcon::MsgType::_HELP:
		m_goal_Evalutor[static_cast<int>(goal_tag::medical_friend)]->Set_MessageBias();
		return true;
		break;

	default :
#if NPC_ASSART
		MyAssert(false,"Goal_Think::Set_FriendMessageBias‚É—\Šú‚µ‚È‚¢Message");
#endif
		break;
	}
	return false;
}
