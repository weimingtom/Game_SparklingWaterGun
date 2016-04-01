#include	"..\All.h"
#include "..\FriendMessage.h"
#include	"NPC_FriendMessage.h"
#include	"..\CharacterMoveStateMessageFunc.h"

NPCFriendMessage::NPCFriendMessage(NPC_Brain* holder) : m_holder(holder), m_param()
{
	m_param.active = false;
	m_param.message_DelayFrame = 0;
	m_param.message_Type = -1;
	m_param.wait_Frame = 0;
}

NPCFriendMessage::~NPCFriendMessage()
{

}

void	NPCFriendMessage::Avtive(const int message_type)
{
	if (m_param.active)
		return;
	m_param.wait_Frame = m_message_interval;
	m_param.message_DelayFrame = m_responce_Time;
	m_param.message_Type = message_type;
	m_param.active = true;
}


void NPCFriendMessage::Update(Character*character)
{
	if (!m_param.active)
		return;
	if (m_param.message_DelayFrame > 0)
	{
		if (--m_param.message_DelayFrame <= 0)
			ChrStateMsg::SendFriendMsgIcon(character, (FriendMessageIcon::MsgType)m_param.message_Type);
		return;
	}
	if (m_param.wait_Frame >= 0)
	{
		if (--m_param.wait_Frame <= 0)
		{
			m_param.active = false;
			m_param.message_DelayFrame = 0;
			m_param.wait_Frame = 0;
		}
	}
}

