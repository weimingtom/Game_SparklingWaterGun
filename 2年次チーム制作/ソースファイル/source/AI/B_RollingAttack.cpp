#include	"..\All.h"

Anime_RollingAttack::Anime_RollingAttack() :
AI_BehaviorBase(behavior_tag::rolling_attack, "RollingAttack")
{

}

Anime_RollingAttack::~Anime_RollingAttack()
{
	
}

void	Anime_RollingAttack::Enter(NPC_Brain* brain, Character* character)
{
	ChrStateMsg::SendChangeStateMsg(character, chr_move_state::rolling);
}

goal_status	Anime_RollingAttack::Execute(NPC_Brain* brain, Character* character)
{
	return m_process_Function(brain, character);
}

void	Anime_RollingAttack::Exit(NPC_Brain* brain, Character* character)
{
	
}

bool	Anime_RollingAttack::HandleMessage(AI_MESSAGE* msg)
{
	switch (msg->type)
	{
	case ai_messagetype::update_memory:
		break;

	default:
		MyAssert(false, "想定外のメッセージがAnime_RollingAttackにとんできました : msg : %s",
			message_char(msg->type));
	}

	return false;
}


