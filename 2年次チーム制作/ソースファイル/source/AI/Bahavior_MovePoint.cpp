#include	"..\All.h"

Move_Point::Move_Point() :
AI_BehaviorBase(behavior_tag::move_point, "Move_Point"),
m_goal_Pos(Vector3(.0f, .0f, .0f)), m_jump(false), m_move_ErrorTimer(move_error_time),
m_before_Pos(Vector3(.0f, .0f, .0f))
{

}

Move_Point::~Move_Point()
{

}

void	Move_Point::Enter(NPC_Brain* brain, Character* character)
{
	m_Active = false;
	m_status = goal_status::active;
	m_jump = false;
	m_move_ErrorTimer = move_error_time;
}

void	Move_Point::Move(NPC_Brain* brain, Character* character, const Vector3& cur_Pos)
{
	Vector3 vec = m_goal_Pos - cur_Pos;
	//この辺はテスト
	vec.Normalize();
	vec *= .5f;
	CharacterMoveStateMessageFunc::SendMoveVecMsg(character, vec);
	CharacterMoveStateMessageFunc::SendRotateViewPosMsg(character, m_goal_Pos, 0.1f);

}

void	Move_Point::Jump(NPC_Brain* brain, Character* character, const Vector3& cur_Pos)
{
	Vector3 view_Target = m_goal_Pos;
	view_Target.y += 1.0f;
	Vector3 vec = m_goal_Pos - cur_Pos;
	vec.Normalize();
	vec *= .5f;
	CHARACTER_STATE_TYPE current_State = ChrFunc::GetStateType(character);
	if (current_State != CHARACTER_STATE_TYPE::jump_mode)
	{
		m_jump = false;
	}
	CharacterMoveStateMessageFunc::SendMoveVecMsg(character, vec);
	CharacterMoveStateMessageFunc::SendRotateViewPosMsg(character, view_Target, 0.1f);
}

bool	Move_Point::Check_Difference_In_Level(Character* character, const Vector3& cur_Pos)
{
	if (m_jump)
		return false;
	bool flg = character->GetFrameInfo().isHitWall;
	CHARACTER_STATE_TYPE type = ChrFunc::GetStateType(character);
	if (flg && type != CHARACTER_STATE_TYPE::jump_mode)
	{
		CharacterMoveStateMessageFunc::SendChangeStateMsg(character, CHARACTER_STATE_TYPE::jump_mode);
		m_jump = true;
	}
	return m_jump;
}

bool	Move_Point::MoveCheck( const Vector3& cur_Pos)
{
	Vector3 vec = cur_Pos - m_before_Pos;
	float length_sq = vec.LengthSq();
	if (length_sq <= 0.0000001f)
	{
		m_move_ErrorTimer--;
		if (m_move_ErrorTimer <= 0)
		{
			m_move_ErrorTimer = move_error_time;
			return false;
		}
	}
	else
	{
		m_move_ErrorTimer = move_error_time;
	}
	return true;
}

bool	Move_Point::GoalCheck(const Vector3& cur_Pos)
{
	Vector3 work = m_goal_Pos - cur_Pos;
	work.y = .0;
	float dist_sq = work.LengthSq();
	if (dist_sq < 0.5f)
	{
		return true;
	}
	return false;
}

goal_status	Move_Point::Execute(NPC_Brain* brain, Character* character)
{
	//ゴールがセットされてなかったらreturn 
	if (!m_Active)
		return goal_status::active;
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	//ジャンプ判定
	Check_Difference_In_Level(character, my_Pos);
	if (!MoveCheck(my_Pos))
	{
		CharacterMoveStateMessageFunc::SendChangeStateMsg(character, CHARACTER_STATE_TYPE::jump_mode);
		m_jump = true;
	}

	if (m_jump)
		Jump(brain,character, my_Pos);
	else
		Move(brain, character, my_Pos);

	if (GoalCheck(my_Pos))
	{
		m_status = goal_status::completed;
		return m_status;
	}
	

	m_status = m_process_Function(brain, character);
	//一フレーム前の座標を保存
	m_before_Pos = my_Pos;
	return m_status;
}

void	Move_Point::Exit(NPC_Brain* brain, Character* character)
{
	m_Active = false;
	m_status = goal_status::inactive;
}

bool	Move_Point::HandleMessage(AI_MESSAGE* msg)
{
	switch (msg->type)
	{
	case ai_messagetype::set_currentGoalPos : 
		m_goal_Pos = *static_cast<Vector3*>(msg->extra_Info);
		m_Active = true;
		return true;
		break;

	case ai_messagetype::update_memory:
		break;
	default :
		MyAssert(false, "想定外のメッセージがMove_Pointにとんできました : msg : %s", message_char(msg->type));
		break;
	}

	return false;
}
