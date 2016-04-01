#include	"..\All.h"

Anime_ThrowGrenade::Anime_ThrowGrenade() :
AI_BehaviorBase(behavior_tag::grenede_throw, "ThrowGrenade")
{

}

Anime_ThrowGrenade::~Anime_ThrowGrenade()
{

}

void	Anime_ThrowGrenade::Enter(NPC_Brain* brain, Character* character)
{
	ChrStateMsg::SendChangeStateMsg(character, chr_move_state::throw_grenade);
}

goal_status	Anime_ThrowGrenade::Execute(NPC_Brain* brain, Character* character)
{
	return m_process_Function(brain, character);
}

void	Anime_ThrowGrenade::Exit(NPC_Brain* brain, Character* character)
{

}

bool	Anime_ThrowGrenade::HandleMessage(AI_MESSAGE* msg)
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
