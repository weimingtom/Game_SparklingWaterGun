#include "All.h"


void CharacterMoveStateMessageFunc::SendViewMsg(
	LPCHARACTER p, CRVector3 view_vec)
{
	//�w�肵���x�N�g���̕����Ɍ����悤�Ƀ��b�Z�[�W���쐬���đ���
	CharacterStateMessage state_msg;
	GameMessage game_msg;

	state_msg.type = chr_state_msg_type::send_view_position;
	state_msg.v1 = view_vec;

	game_msg.to = p->GetId();
	game_msg.from = game_id::id_error;
	game_msg.type = msg_t::to_chr_state;
	game_msg.exinfo = &state_msg;

	p->HandleMessage(&game_msg);
}

void CharacterMoveStateMessageFunc::SendChangeStateMsg(LPCHARACTER p, CHARACTER_STATE_TYPE set_state_type)
{
	//�w�肵���X�e�[�g�ɕύX���郁�b�Z�[�W�𑗂�(�K�؂ȃX�e�[�g�łȂ������ꍇ���������)
	GameMessage game_msg;

	game_msg.to = p->GetId();
	game_msg.from = game_id::id_error;
	game_msg.type = msg_t::chr_table_state_change_low_lvl;
	game_msg.exinfo = &set_state_type;

	p->HandleMessage(&game_msg);
}
void CharacterMoveStateMessageFunc::SendMoveVecMsg(
	LPCHARACTER p,
	CRVector3 move_vec)	
{
	//�ړ��x�N�g�����i�[�������b�Z�[�W���쐬����

	CharacterStateMessage state_msg;
	GameMessage game_msg;

	state_msg.type = chr_state_msg_type::send_move_vector;
	state_msg.v1 = move_vec;

	game_msg.to = p->GetId();
	game_msg.from = game_id::id_error;
	game_msg.type = msg_t::to_chr_state;
	game_msg.exinfo = &state_msg;

	p->HandleMessage(&game_msg);
}

void CharacterMoveStateMessageFunc::SendRotateViewPosMsg(
	LPCHARACTER p,
	CRVector3 view_pos,
	RADIAN rotate_speed)
{
	//�w�肵���ꏊ�Ɍ������Ď�����speed��������
	Vector3 v1 = ChrFunc::GetFront(p), v2 = view_pos - p->GetBodyCenter(), axis;

	float angle = Vector3Radian(v1, v2);
	angle = min(angle, rotate_speed);

	Vector3Cross(axis, v1, v2);
	axis.Normalize();

	SendViewMsg(p, Vector3RotateAxis(axis, angle, Vector3Normalize(v1)));

//	SendViewMsg(p, view_pos - p->GetBodyCenter());
}

void CharacterMoveStateMessageFunc::SendRotateYViewMsg(
	LPCHARACTER p,
	RADIAN rotate_speed)
{
	//�L�����N�^��Y����rotate_speed��������]������
	//if (ABS(rotate_speed < 0.001f))return;
	SendViewMsg(p, Vector3RotateAxis(CHARACTER_UPVEC, rotate_speed, Vector3Normalize(ChrFunc::GetFront(p))));
}


void CharacterMoveStateMessageFunc::SendRotateXViewMsg(
	LPCHARACTER p,
	RADIAN rotate_speed)
{
	//�L�����N�^��X����rotate_speed��������]������
	Vector3 axis;
	Vector3Cross(axis, CHARACTER_UPVEC, ChrFunc::GetFront(p));
	axis.Normalize();

	rotate_speed = -rotate_speed;

	Vector3 view_vec = Vector3RotateAxis(axis, rotate_speed, ChrFunc::GetFront(p));

	SendViewMsg(p, view_vec);
}


void CharacterMoveStateMessageFunc::SendShuffleMsg(LPCHARACTER p, bool shuffle)
{
	//�U�郁�b�Z�[�W���쐬���đ���
	CharacterStateMessage state_msg;
	GameMessage game_msg;

	state_msg.type = chr_state_msg_type::send_shuffle_flg;
	state_msg.v1.x = shuffle ? 1.f : 0.f;

	game_msg.to = p->GetId();
	game_msg.from = game_id::id_error;
	game_msg.type = msg_t::to_chr_state;
	game_msg.exinfo = &state_msg;

	p->HandleMessage(&game_msg);
}

void CharacterMoveStateMessageFunc::SendWeponFireMsg(LPCHARACTER p)
{
	//�ˌ����b�Z�[�W���쐬���đ���
	CharacterStateMessage state_msg;
	GameMessage game_msg;

	state_msg.type = chr_state_msg_type::send_wepon_fire_flg;

	game_msg.to = p->GetId();
	game_msg.from = game_id::id_error;
	game_msg.type = msg_t::to_chr_state;
	game_msg.exinfo = &state_msg;

	p->HandleMessage(&game_msg);
}

void CharacterMoveStateMessageFunc::SendLockOnMsg(LPCHARACTER p, bool lockOn)
{
	CharacterStateMessage state_msg;
	GameMessage game_msg;

	state_msg.type = chr_state_msg_type::send_lock_on_flg;
	state_msg.v1.x = lockOn ? 1.f : 0.f;

	game_msg.to = p->GetId();
	game_msg.from = game_id::id_error;
	game_msg.type = msg_t::to_chr_state;
	game_msg.exinfo = &state_msg;

	p->HandleMessage(&game_msg);
}

void CharacterMoveStateMessageFunc::SendChangeCameraPosMsg(LPCHARACTER p)
{
	CharacterStateMessage state_msg;
	GameMessage game_msg;

	state_msg.type = chr_state_msg_type::send_lock_on_change_right_left;

	game_msg.to = p->GetId();
	game_msg.from = game_id::id_error;
	game_msg.type = msg_t::to_chr_state;
	game_msg.exinfo = &state_msg;

	p->HandleMessage(&game_msg);
}

void CharacterMoveStateMessageFunc::SendJumpHoverMsg(LPCHARACTER p)
{
	CharacterStateMessage state_msg;
	GameMessage game_msg;

	state_msg.type = chr_state_msg_type::send_hover;

	game_msg.to = p->GetId();
	game_msg.from = game_id::id_error;
	game_msg.type = msg_t::to_chr_state;
	game_msg.exinfo = &state_msg;

	p->HandleMessage(&game_msg);
}

void CharacterMoveStateMessageFunc::SendFriendMsgIcon(LPCHARACTER p, FriendMessageIcon::MsgType type)
{
	GameMessage game_msg;

	game_msg.to = p->GetId();
	game_msg.from = game_id::id_error;
	game_msg.type = msg_t::chr_friend_msg_icon;
	game_msg.exinfo = &type;

	p->HandleMessage(&game_msg);
}