#include	"..\All.h"

Move_Edge::Move_Edge() :
AI_BehaviorBase(behavior_tag::move_edge, "MoveEdge"), m_move_ErrorTimer(move_error_time),
m_jump(false), m_wait(false), m_before_Pos(Vector3(0, 0, 0)), m_wait_ErrorTimer(wait_error_time)
{
	m_wait = true;
}

Move_Edge::~Move_Edge()
{
	route_List.clear();
}

void	Move_Edge::Enter(NPC_Brain* brain, Character* character)
{
	m_wait = true;
	m_status = goal_status::active;
	m_max_rotate_Speed = Accesor_Brain::Param(brain).m_max_rotateAngle_Speed;
	m_move_ErrorTimer = move_error_time;
	m_wait_ErrorTimer = wait_error_time;
}

bool	Move_Edge::Check_Erase_Edge(Character* character, const Vector3& current_Pos,
	const Vector3& tonode_Pos, const float& dist_sq)
{
	//�����Ƌ����Ŕ�����s��
	float height = fabs(current_Pos.y - tonode_Pos.y);
	if (dist_sq < 0.25f && height < 1.5f && !m_jump)
	{
		route_List.pop_front();
		//������������s��
		Smoothing_Route(character);
		if (route_List.empty())
			return true;
	}
	return false;
}

float body_centor_adjust = 0.5f;
goal_status	Move_Edge::Move(Character* character, const Vector3& current_Pos, const Vector3& tonode_Pos)
{
	Vector3 vec = tonode_Pos - current_Pos;
	vec.y = .0f;
	float distsq = vec.LengthSq();
	vec.Normalize();
	vec *= 0.5f;
	Vector3 view_Target = tonode_Pos ;

	Vector3 temp = character->GetBodyCenter();
	view_Target.y += body_centor_adjust;

	//�i�s�����̏����������
	view_Target += vec *3.0f;

	CharacterMoveStateMessageFunc::SendMoveVecMsg(character, vec);
	CharacterMoveStateMessageFunc::SendRotateViewPosMsg(character, view_Target, m_max_rotate_Speed);

	debug_particle::point(view_Target, Vector3(0, 0, 0), 0.1f, dword_color::red);

	//�G�b�W�̍폜����
	bool ret = Check_Erase_Edge(character, current_Pos, tonode_Pos, distsq);

	if (ret)
		return goal_status::completed;
	return goal_status::active;
}

bool	Move_Edge::Check_Difference_In_Level(Character* character, const Vector3& current_Pos, const Vector3& to_NodePos)
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

goal_status	Move_Edge::Jump(Character* character, const Vector3& current_Pos, const Vector3& tonode_Pos,
	Field_Edge* current_Edge)
{
	Vector3 direc = tonode_Pos - dynamic_cast<Field_Node*>(current_Edge->from)->pos;
	direc.y = .0f;
	direc.Normalize();
	direc *= 0.5f;
	Vector3 view_Target = tonode_Pos;
	Vector3 temp = character->GetBodyCenter();
	view_Target.y = temp.y - 0.2f;
	CHARACTER_STATE_TYPE current_State = ChrFunc::GetStateType(character);
	if (current_State != CHARACTER_STATE_TYPE::jump_mode)
	{
		m_jump = false;
		return goal_status::active;
	}
	CharacterMoveStateMessageFunc::SendMoveVecMsg(character, direc);
	CharacterMoveStateMessageFunc::SendRotateViewPosMsg(character, view_Target, m_max_rotate_Speed);
	return goal_status::active;
}

bool	Move_Edge::WaitErrorCheck()
{
	m_wait_ErrorTimer--;
	if (m_wait_ErrorTimer <= 0)
		return false;
	return true;
}

goal_status	Move_Edge::Execute(NPC_Brain* brain, Character* character)
{
	//�ҋ@���̏ꍇ
	if (m_wait)
	{
		if (!WaitErrorCheck())
			return goal_status::failed;
		return goal_status::active;
	}
	
	//�o�H���Ȃ��ꍇ
	if (route_List.empty())
	{
		m_status = goal_status::failed;
		return m_status;
	}
	Field_Edge* current_Edge = route_List.front();

	Field_Node* to_Node = dynamic_cast<Field_Node*>(current_Edge->to);
	Vector3 target = to_Node->pos;
	Vector3 cur_Pos = ControllObjFunc::GetPos(character);
	//�W�����v����
	Check_Difference_In_Level(character, cur_Pos, target);
	//��Q���Ɉ����������Ă��Ȃ���
	if (!MoveCheck(cur_Pos))
	{
		CharacterMoveStateMessageFunc::SendChangeStateMsg(character, CHARACTER_STATE_TYPE::jump_mode);
		m_jump = true;
	}

	if (m_jump)
	{
		m_status = Jump(character, cur_Pos, target, current_Edge);
	}
	else
		m_status = Move(character, cur_Pos, target);
	if (m_status == goal_status::completed)
	{
		return m_status;
	}

	//processFunction�ɂ�锻��
	m_status = m_process_Function(brain, character);
	if (m_status != goal_status::active)
	{
		Accesor_Brain::PathPlanner(brain)->Exit();
		return m_status;
	}
	if (!MoveErrorCheck(character, cur_Pos, target))
		m_status = goal_status::failed;

	//�O��t���[���̍��W�Ƃ��ĕۑ�
	m_before_Pos = cur_Pos;

	return m_status;
}

void	Move_Edge::Exit(NPC_Brain* brain, Character* character)
{
	m_status = goal_status::inactive;
}

void	Move_Edge::SetRoute(PathSearch_Base<Field_Node, Field_Edge>* alg)
{
	route_List.clear();
	navigation_system->Set_Route(alg->Get_GoalList()->front(), alg->start_Node, &route_List);
	m_wait = false;
	m_wait_ErrorTimer = wait_error_time;
}

bool	Move_Edge::HandleMessage(AI_MESSAGE* msg)
{
	switch (msg->type)
	{
	case ai_messagetype::search_path_ok:
		break;
	case ai_messagetype::set_currentGoalPos:
		break;
	case ai_messagetype::set_route:
		SetRoute(static_cast<PathSearch_Base<Field_Node, Field_Edge>*>(msg->extra_Info));
		break;

	case ai_messagetype::wait : 
		m_wait = true;
		break;

	case ai_messagetype::update_memory:
		break;
	default:
#if NPC_ASSART
		MyAssert(false, "�z��O�̃��b�Z�[�W��MoveEdge�ɂƂ�ł��܂��� : msg : %s", message_char(msg->type));
#endif
		break;
	}

	return true;
}

void	Move_Edge::Set_DebugEdge()
{

}

bool	Move_Edge::Smoothing_Route(Character* character)
{
	//RouteList�̃T�C�Y��1�ȉ��̏ꍇ����������o�H�͂Ȃ�
	if (route_List.size() <= 1)
		return false;
	//��̌o�H���擾
	auto temp = route_List.begin();
	auto next_Edge = ++temp;
	auto current_Edge = route_List.begin();

	//��������
	//current
	Field_Node* current_From = dynamic_cast<Field_Node*>((*current_Edge)->from);
	Field_Node* current_to = dynamic_cast<Field_Node*>((*current_Edge)->to);

	float height = fabs(current_From->pos.y - current_to->pos.y);
	if (height > 0.6f)
		return false;

	//next
	Field_Node* next_From = current_to;
	Field_Node* next_to = dynamic_cast<Field_Node*>((*next_Edge)->to);
	
	height = fabs(next_From->pos.y - next_to->pos.y);
	if (height > 0.6f)
		return false;
	 
	//curret_from��next_to�ŏ�Q���v�Z
	Vector3 to_next_Pos = next_to->pos;
	to_next_Pos.y += .6f;
	//�����I�ɃG�b�W��ExtraInfo���l���ɓ��ꂽ������������s���i�����������Ȃ畽�����Ȃǁj
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	my_Pos.y += 0.6f;
	Vector3 vec = to_next_Pos - my_Pos;
	float dist = vec.Length();
	vec.Normalize();
	Vector3 out;
	MapObjectInterface * object = MAP_MANAGER.RayPick(&out, &my_Pos, &vec, &dist);
	//��Q�����Ȃ��ꍇ�͕���������
	if (object == nullptr)
	{
		field_Debug->Erase_Debug_Edge(character->GetId(), route_List.front());
		route_List.pop_front();
		return true;
	}

	return false;
}

bool	Move_Edge::MoveCheck(const Vector3& current_Pos)
{
	Vector3 vec = current_Pos - m_before_Pos;
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

bool	Move_Edge::MoveErrorCheck(Character* character,
	const Vector3& current_Pos,const Vector3& current_TargetPos)
{
	//�R�̍����i�Ƃ肠�����Œ�l�j
	static const float precipice = 2.6f;
	
	if (fabs(current_Pos.y - current_TargetPos.y) >= precipice)
		return false;

	return true;
}
