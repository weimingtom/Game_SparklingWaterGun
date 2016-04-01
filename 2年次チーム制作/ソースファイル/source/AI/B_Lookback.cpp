#include	"..\All.h"

Anime_LookBack::Anime_LookBack() :
AI_BehaviorBase(behavior_tag::look_back,"look_back"), m_update_Memory(false), m_wait(false)
{}

Anime_LookBack::~Anime_LookBack()
{
	
}

void	Anime_LookBack::Enter(NPC_Brain* brain, Character* character)
{
	m_update_Memory = false;
	m_wait = false;
	m_status = goal_status::active;
	m_timer = m_lookBack_Time;
}


goal_status	Anime_LookBack::Execute(NPC_Brain* brain, Character* character)
{
	m_timer--;
	if (m_timer < 0)
	{
		m_status = goal_status::failed;
		return m_status;
	}
	Vector3 my_Front = CharacterUtilityFunctions::GetFront(character);
	my_Front.Normalize();
	float cos = 1.0f -Vector3Dot(my_Front, m_look_Vec);
	
	cos = Min(.17f, cos);
	if (cos < 0.1f && !m_wait)
	{
		Accesor_Brain::Censor(brain)->m_sight->Look();
		m_wait = true;
	}
	//向く方向を決める
	Vector3 view_Pos = character->GetBodyCenter() + m_look_Vec * 3.0f;
	
	CharacterMoveStateMessageFunc::SendRotateViewPosMsg(character, view_Pos, 0.15f);

	if (m_wait && m_update_Memory)
	{
		m_status = goal_status::completed;
	}


	return m_status;
}

void	Anime_LookBack::Exit(NPC_Brain* brain, Character* character)
{
	m_status = goal_status::inactive;
}

bool	Anime_LookBack::HandleMessage(AI_MESSAGE* msg)
{
	switch (msg->type)
	{
	case ai_messagetype::set_LookVec : 
		m_look_Vec = *static_cast<Vector3*>(msg->extra_Info);
		if (m_look_Vec.Length() <= .0001f)
		{
			MyAssert(false, "LookVecがおかしい");
		}
	
		m_look_Vec.Normalize();
		break;

	case ai_messagetype::update_memory : 
		if (m_wait)
			m_update_Memory = true;
		break;

	default:
#if ASSERT
		MyAssert(false, "想定外のメッセージがAnime_LookBackにとんできました : msg : %s", message_char(msg->type));
#endif
		break;
	}
	return true;
}
