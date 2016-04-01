#include	"..\All.h"

#pragma region Stand
Anime_ShotWeapon_Stand::Anime_ShotWeapon_Stand(NPC_Aim* use_aimObject) :
AI_BehaviorBase(behavior_tag::shot_weapon_stand, "ShotWeapon_Stand")
, m_aim(use_aimObject)
{}

Anime_ShotWeapon_Stand::~Anime_ShotWeapon_Stand()
{
	SAFE_DELETE(m_aim);
}

void	Anime_ShotWeapon_Stand::Enter(NPC_Brain* brain, Character* character)
{
	m_aim->Activate();
	m_holder_Id = character->GetId();
	m_target_Id = Accesor_Brain::Current_Target(brain);
	m_status = goal_status::active;
}

//void	Anime_ShotWeapon_Stand::Aim(NPC_Brain* brain, Character* character)
//{
//	//必要な情報としてターゲットの速度、座標を取得
//	Vector3 target_Pos, target_Velocity;	
//	if (IS_CONTROLLOBJ_ID(m_target_Id))
//	{
//		float reliability;
//		Character_Record* target = brain->Get_CharacterMemory(m_target_Id,&reliability);
//		target_Pos = target->m_sensed_Pos;
//		target_Velocity = target->m_sensed_Velocity;
//	}
//	else 
//	{
//		float reliability;
//		MapObject_Record* target = brain->Get_MapObjectMemory(m_target_Id, &reliability);
//		target_Pos = target->m_sensed_Pos;
//		target_Velocity = target->m_sensed_Velocity;
//	}
//	
//	//ターゲットの進行方向に少しだけ補正をかけた座標をターゲットとする
//	target_Pos.y += .3f;
//	target_Pos += target_Velocity * 0.5f;
//	CharacterMoveStateMessageFunc::SendRotateViewPosMsg(character, target_Pos, 0.12f);
//
//}

goal_status	Anime_ShotWeapon_Stand::Execute(NPC_Brain* brain, Character* character)
{
	m_aim->Update(brain, character);
	CharacterMoveStateMessageFunc::SendWeponFireMsg(character);
	m_status = m_process_Function(brain, character);
	return m_status;
}

void	Anime_ShotWeapon_Stand::Exit(NPC_Brain* brain, Character* character)
{
	m_status = goal_status::inactive;
}

bool	Anime_ShotWeapon_Stand::HandleMessage(AI_MESSAGE* msg)
{
	switch (msg->type)
	{
	case ai_messagetype::update_memory:
		break;
	default : 
#if ASSERT
		MyAssert(false, "想定外のメッセージがAnime_ShotWeaponにとんできました : msg : %s", message_char(msg->type));
#endif
		break;
	}
	return false;
}

#pragma endregion

#pragma region Circle
Anime_ShotWeapon_Circle::Anime_ShotWeapon_Circle(NPC_Aim* use_aimObject) :
AI_BehaviorBase(behavior_tag::shot_weapon_circle, "ShotWeapon_Circle"), m_before_Pos(Vector3(0,0,0)),
m_current_Time(0),m_aim(use_aimObject)
{
}

Anime_ShotWeapon_Circle::~Anime_ShotWeapon_Circle()
{
	SAFE_DELETE(m_aim);
}

void	Anime_ShotWeapon_Circle::Enter(NPC_Brain* brain, Character* character)
{
	m_aim->Activate();
	m_holder_Id = character->GetId();
	m_target_Id = Accesor_Brain::Current_Target(brain);
	//Circleの中心ノードを設定する
	Culculate_WayPointInformation(ControllObjFunc::GetPos(character));
	m_status = goal_status::active;
	m_current_Time = m_timer;
	m_before_Pos = Vector3(0, 0, 0);
}

void	Anime_ShotWeapon_Circle::Culculate_WayPointInformation(const Vector3& pos)
{
	Field_Node* current_Node = navigation_system->Find_Node_Pos(pos);
	m_current_Centor_Pos = current_Node->pos;
	m_current_CentorNodeInformation = navigation_system->Get_NodeInformation(current_Node->index);
	m_current_MoveCircleSize = m_current_CentorNodeInformation->m_even_with_Circle.size();
	m_current_MoveTargetIndex = 0;
}

void	Anime_ShotWeapon_Circle::Shuffle_Point()
{
	int next = m_current_MoveTargetIndex;
	while (next == m_current_MoveTargetIndex)
		next = Rand::Equal_DistanceNum(0, m_current_MoveCircleSize - 1);
	m_current_MoveTargetIndex = next;
}

void	Anime_ShotWeapon_Circle::Move(NPC_Brain* brain, Character* character)
{
	if (m_current_MoveCircleSize <= 1)
		return;
	Vector3 current_Pos = ControllObjFunc::GetPos(character);
	Vector3 target_Pos = m_current_CentorNodeInformation->m_even_with_Circle[m_current_MoveTargetIndex]->pos;
	Vector3 vec = target_Pos - current_Pos;

	float dist_sq = vec.Length();
	if (dist_sq < 1.0f)
	{
		Shuffle_Point();
	}
	float move_Dist = (current_Pos - m_before_Pos).LengthSq();
	if (move_Dist <= .0f)
	{
		m_current_Time--;
		if (m_current_Time <= 0)
			Shuffle_Point();
	}
	else
		m_current_Time = m_timer;
	
	vec.Normalize();
	vec *= .35f;
	CharacterMoveStateMessageFunc::SendMoveVecMsg(character, vec);
	
	m_before_Pos = current_Pos;
}

//void	Anime_ShotWeapon_Circle::Aim(NPC_Brain* brain, Character* character)
//{
//	//必要な情報としてターゲットの速度、座標を取得
//	Vector3 target_Pos, target_Velocity;
//	if (IS_CONTROLLOBJ_ID(m_target_Id))
//	{
//		float reliability;
//		Character_Record* target = brain->Get_CharacterMemory(m_target_Id, &reliability);
//		target_Pos = target->m_sensed_Pos;
//		target_Velocity = target->m_sensed_Velocity;
//	}
//	else
//	{
//		
//	}
//
//	//ターゲットの進行方向に少しだけ補正をかけた座標をターゲットとする
//	target_Pos.y += .3f;
//	target_Pos += target_Velocity * 0.5f;
//	CharacterMoveStateMessageFunc::SendRotateViewPosMsg(character, target_Pos, 0.12f);
//}

goal_status	Anime_ShotWeapon_Circle::Execute(NPC_Brain* brain, Character* character)
{
	Move(brain, character);
	m_aim->Update(brain, character);
	CharacterMoveStateMessageFunc::SendWeponFireMsg(character);
	m_status= m_process_Function(brain, character);
	return m_status;
}

void	Anime_ShotWeapon_Circle::Exit(NPC_Brain* brain, Character* character)
{
	m_status = goal_status::inactive;
}

bool	Anime_ShotWeapon_Circle::HandleMessage(AI_MESSAGE* msg)
{
	switch (msg->type)
	{
	case ai_messagetype::update_memory:
		break;
	default:
#if ASSERT
		MyAssert(false, "想定外のメッセージがAnime_ShotWeaponにとんできました : msg : %s", message_char(msg->type));
#endif
		break;
	}
	return false;
}

#pragma endregion
