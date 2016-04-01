#include "All.h"


void CharacterMoveStateMessageFunc::SendViewMsg(
	LPCHARACTER p, CRVector3 view_vec)
{
	//指定したベクトルの方向に向くようにメッセージを作成して送る
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
	//指定したステートに変更するメッセージを送る(適切なステートでなかった場合無視される)
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
	//移動ベクトルを格納したメッセージを作成する

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
	//指定した場所に向かって視線をspeed分向ける
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
	//キャラクタをY軸にrotate_speed分だけ回転させる
	//if (ABS(rotate_speed < 0.001f))return;
	SendViewMsg(p, Vector3RotateAxis(CHARACTER_UPVEC, rotate_speed, Vector3Normalize(ChrFunc::GetFront(p))));
}


void CharacterMoveStateMessageFunc::SendRotateXViewMsg(
	LPCHARACTER p,
	RADIAN rotate_speed)
{
	//キャラクタをX軸にrotate_speed分だけ回転させる
	Vector3 axis;
	Vector3Cross(axis, CHARACTER_UPVEC, ChrFunc::GetFront(p));
	axis.Normalize();

	rotate_speed = -rotate_speed;

	Vector3 view_vec = Vector3RotateAxis(axis, rotate_speed, ChrFunc::GetFront(p));

	SendViewMsg(p, view_vec);
}


void CharacterMoveStateMessageFunc::SendShuffleMsg(LPCHARACTER p, bool shuffle)
{
	//振るメッセージを作成して送る
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
	//射撃メッセージを作成して送る
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