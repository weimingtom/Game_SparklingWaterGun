#include	"..\All.h"

Anime_AirBorne::Anime_AirBorne() :
AI_BehaviorBase(behavior_tag::air_borne, "air_borne"), m_radius(400.0f)
{}

Anime_AirBorne::~Anime_AirBorne()
{

}

void	Anime_AirBorne::Enter(NPC_Brain* brain, Character* character)
{
	m_status = goal_status::active;
	Direction_MoveVec();
}

void	Anime_AirBorne::Direction_MoveVec()
{
	float rand_x = ((float)Rand::Equal_DistanceNum(0, 100) / 100) * 2.0f -1.0f;
	float rand_z = ((float)Rand::Equal_DistanceNum(0, 100) / 100) * 2.0f - 1.0f;
	Vector3 vec(rand_x,.0f,rand_z);
	vec.Normalize();
	m_move_vec = vec * 0.5f;
	m_timer = m_changevec_intarval;
}

void	Anime_AirBorne::Radius_Check(const Vector3& player_pos)
{
	Vector3 centor(.0f, player_pos.y, .0f);
	Vector3 vec = centor - player_pos;
	float dist = vec.LengthSq();
	if (dist >= m_radius)
	{
		vec.Normalize();
		m_move_vec = vec* .5f;
		m_timer = m_changevec_intarval;
	}
}

goal_status	Anime_AirBorne::Execute(NPC_Brain* brain, Character* character)
{
	Vector3 pos = ControllObjFunc::GetPos(character);
	Radius_Check(pos);
	m_timer--;
	if (m_timer <= 0)
		Direction_MoveVec();
	CharacterMoveStateMessageFunc::SendMoveVecMsg(character, m_move_vec);
	if (character->GetFrameInfo().isHitFloor)
		m_status = goal_status::completed;

	return m_status;
}

void	Anime_AirBorne::Exit(NPC_Brain* brain, Character* character)
{
	m_status = goal_status::inactive;
}

bool	Anime_AirBorne::HandleMessage(AI_MESSAGE* msg)
{
	/*switch (msg->type)
	{
	default:
#if ASSERT
		MyAssert(false, "想定外のメッセージがAnime_LookBackにとんできました : msg : %s", message_char(msg->type));
#endif
		break;
	}*/
	 return false;
}
