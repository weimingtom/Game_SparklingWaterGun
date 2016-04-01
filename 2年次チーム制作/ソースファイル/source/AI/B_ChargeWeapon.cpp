#include	"..\All.h"

Anime_ChargeWeapon::Anime_ChargeWeapon() :
AI_BehaviorBase(behavior_tag::charge_weapon,"ChargeWeapon") , m_need_Charge(0)
{

}

Anime_ChargeWeapon::~Anime_ChargeWeapon()
{

}

void	Anime_ChargeWeapon::Enter(NPC_Brain* brain, Character* character)
{
	m_need_Charge = Accesor_Brain::Symbol(brain).m_weapon_Charge;
	m_status = goal_status::active;
}

goal_status	Anime_ChargeWeapon::Execute(NPC_Brain* brain, Character* character)
{
	CharacterMoveStateMessageFunc::SendShuffleMsg(character, true);
	int current_Charge = (int)((float)ChrFunc::GetUI(character)->GetColaGaugePower() * 100.0f);
	if (m_need_Charge <= current_Charge)
	{
		CharacterMoveStateMessageFunc::SendShuffleMsg(character, false);
		m_status = goal_status::completed;
	}
	return m_status;
}

void	Anime_ChargeWeapon::Exit(NPC_Brain* brain, Character* character)
{
	m_need_Charge = -1;
	CharacterMoveStateMessageFunc::SendShuffleMsg(character, false);
	m_status = goal_status::inactive;
}

bool	Anime_ChargeWeapon::HandleMessage(AI_MESSAGE* msg)
{
	switch (msg->type)
	{
	case ai_messagetype::update_memory:
		break;

	default :
	MyAssert(false, "想定外のメッセージがAnime_ChargeWeaponにとんできました : msg : %s",
		message_char(msg->type));
	}
	
	return false;
}
