#include "All.h"

static void SendStateChangeMsg(LPCHARACTER p,CHARACTER_STATE_TYPE type)
{
	GameMessage msg;
	msg.to = p->GetId();
	msg.exinfo = &type;
	msg.type = msg_t::chr_table_state_change;
	p->HandleMessage(&msg);
}

//*****************************************************************
//	キャラクタのステートをひとまとめにした物(ステートテーブル)
//*****************************************************************

CharacterMoveStateTable::CharacterMoveStateTable(
	LPCHARACTER pOwner):
	m_now_state_type(chr_move_state::error_mode),
	m_next_state_type(chr_move_state::error_mode)
{
	using namespace chr_move_state;

	CharacterMoveStateBase* pState;

	for (int i = 0; i < _state_max; i++)	
	{

		//配列にステートを入れていく
		switch (i)
		{
		case usual_mode:	pState = new CharacterMoveStateUsualMode(pOwner); break;
		case jump_mode:		pState = new CharacterMoveStateJumpMode(pOwner); break;
		case dying_mode:	pState = new CharacterMoveStateDyingMode(pOwner); break;
		case die_mode:		pState = new CharacterMoveStateDieMode(pOwner); break;
		case near_atk_mode:	pState = new CharacterMoveStateNearAttackMode(pOwner); break;
		case throw_grenade:	pState = new CharacterMoveStateThrowGrenade(pOwner); break;
		case rolling:		pState = new CharacterMoveStateMoveRolling(pOwner); break;
		case vanish:		pState = new CharacterMoveStateVanish(pOwner); break;
		case parachute:		pState = new CharacterMoveStateParachuteMove(pOwner); break;
		case apeer_vending_machine:pState = new CharacterMoveStateResporn(pOwner); break;
		default:
			MyAssert(false, "キャラクタのステートテーブル構築に失敗しました");
			pState = nullptr;
			break;
		}
		m_pStateTable[i] = pState;
	}
}


CharacterMoveStateTable::~CharacterMoveStateTable()
{
	using namespace chr_move_state;
	//ステートをすべて消去
	for (int i = 0; i < _state_max; i++)
	{
		SAFE_DELETE(m_pStateTable[i]);
	}
}

void CharacterMoveStateTable::UpdateTransMatrix()
{
	if (m_now_state_type < 0)return;
	SAFE_EXE(m_pStateTable[m_now_state_type], UpdateTransMatrix);
}

void CharacterMoveStateTable::Render()
{
	if (m_now_state_type < 0)return;
	SAFE_EXE(m_pStateTable[m_now_state_type], Render);
}

void CharacterMoveStateTable::Execute()
{
	if (m_next_state_type != m_now_state_type)
	{
		if (m_now_state_type != chr_move_state::error_mode)
		{
			SAFE_EXE(m_pStateTable[m_now_state_type], Exit);	//現在のステートを終える
		}
		m_now_state_type = m_next_state_type;
		SAFE_EXE(m_pStateTable[m_now_state_type], Enter);
	}
	SAFE_EXE(m_pStateTable[m_now_state_type], Execute);
}

CHARACTER_STATE_TYPE CharacterMoveStateTable::GetState()const
{
	return m_next_state_type;
}

void CharacterMoveStateTable::SetState(CHARACTER_STATE_TYPE type)
//新しいステートを設定する
{
	m_next_state_type = type;
}


bool CharacterMoveStateTable::HandleMessage(
	LPGAMEMESSAGE msg)
{
	switch (msg->type)
	{
	case msg_t::chr_table_state_change:
		//メッセージタイプがステート変更だった場合変更する
		SetState(*(CHARACTER_STATE_TYPE*)(msg->exinfo));
		return true;
	case msg_t::chr_table_state_change_low_lvl:
		//ステート変更が可能だった場合変更する
		if (m_now_state_type != chr_move_state::error_mode&&
			m_pStateTable[m_now_state_type]->isCanChangeState(*(CHARACTER_STATE_TYPE*)(msg->exinfo)))
			SetState(*(CHARACTER_STATE_TYPE*)(msg->exinfo));
		return true;
	case  msg_t::to_chr_state:
		//メッセージタイプがステートに対してのものだった場合渡す
		return m_pStateTable[m_next_state_type]->HandleMessage((LPCHARACTERSTATEMESSAGE)msg->exinfo);
	default:
		//受信失敗
		MyAssert(false, "認識できない型のメッセージがCharacterMoveStateTable::HandleMessage関数に送られてきました type=%d", (int)msg->type);
		break;
	}
	return false;
}
//**********************************************************
//	ロックオンクラス
//**********************************************************
CharacterLockOn::CharacterLockOn(LPCHARACTER p) :
m_State(_USUAL),
m_Target(ERR_AIMTARGET_ID),
m_pOwner(p),
m_camera_pos(CameraFunction::_RIGHT)
{
}

void CharacterLockOn::LockOn(bool flg)
{
	LPCAMERA C = ChrFunc::GetCamera(m_pOwner);
	if (flg)
	{
		m_State = _ZOOM;
	}
	else
	{
		m_State = _USUAL;
	}

	if (flg)
	{
		Vector3 p;
		ChrFunc::GetUI(m_pOwner)->GetTargetPos(p);
		
		if (ChrFunc::GetUI(m_pOwner)->GetTargetID(m_Target))
		{
			ChrMove::AngleSet(m_pOwner, p - m_pOwner->GetBodyCenter());
		}

		C->SetNewState(CameraFunction::GetCharacterAimModeCamera(m_pOwner, C, m_camera_pos));
		m_Target = ERR_AIMTARGET_ID;
	}
	else{
		C->SetNewState(CameraFunction::GetCharacterUsualModeCamera(m_pOwner, C, m_camera_pos));
	}

	ChrFunc::GetUI(m_pOwner)->SetChangeTargetFlg(!flg);
}

void CharacterLockOn::Shuffle(bool flg)
{

	LPCAMERA C = ChrFunc::GetCamera(m_pOwner);

	if (flg&&m_State == _SHUFFLE)
		return;
	if (!flg&&m_State != _SHUFFLE)
		return;

	if (flg)
		m_State = _SHUFFLE;
	else
		m_State = _USUAL;

	if (flg)
	{
		C->SetNewState(CameraFunction::GetCharacterShuffleModeCamera(m_pOwner, C, m_camera_pos));
	}
	else{
		C->SetNewState(CameraFunction::GetCharacterUsualModeCamera(m_pOwner, C, m_camera_pos));
	}

}

void CharacterLockOn::ChangeRL()
{
	LPCAMERA C = ChrFunc::GetCamera(m_pOwner);

	if (m_camera_pos == CameraFunction::_RIGHT)
		m_camera_pos = CameraFunction::_LEFT;
	else
		m_camera_pos = CameraFunction::_RIGHT;

	LPCAMERASTATE(*CameraNewMoveFactorys[5])(LPCHARACTER, LPCAMERA, CameraFunction::RIGHTorLEFT);

	CameraNewMoveFactorys[_USUAL] = CameraFunction::GetCharacterUsualModeCamera;
	CameraNewMoveFactorys[_ZOOM] = CameraFunction::GetCharacterAimModeCamera;
	CameraNewMoveFactorys[_SHUFFLE] = CameraFunction::GetCharacterUsualModeCamera;

	C->SetNewState(CameraNewMoveFactorys[m_State](m_pOwner, C, m_camera_pos));
}

bool CharacterLockOn::isZoom()const
{
	return m_State==_ZOOM;
}

bool CharacterLockOn::isShuffle()const
{
	return m_State == _SHUFFLE;
}

AIMTARGET_ID CharacterLockOn::GetTarget()
{
	AIMTARGET_ID ret;

	if (ChrFunc::GetUI(m_pOwner)->GetTargetID(ret))
	{
		return ret;
	}
	return ERR_AIMTARGET_ID;
}

void CharacterLockOn::Update()
{	
	if (m_State != _ZOOM)return;

	AimTargetManager* mgr = (AimTargetManager*)IDMANAGER.GetPointer(game_id::id_aim_target_manager);
	AimTarget* target = nullptr;

	m_Target = GetTarget();

	if (m_Target != ERR_AIMTARGET_ID&&
		mgr->GetObjectPtr(&target, m_Target))
	{
		AimOwnerInfo info;
		AimTargetInfo Tinfo;

		info.camera_z = 0;
		info.circle_size = FLT_MAX;
		info.id = m_pOwner->GetId();
		info.pCamera = ChrFunc::GetCamera(m_pOwner);
		info.position = m_pOwner->GetBodyCenter();
		info.range = FLT_MAX;
		info.team_type = ControllObjFunc::GetMatchData(m_pOwner)->team_type;
		info.view_vec = ChrFunc::GetParam(m_pOwner).m_view_vec;

		if(target->isTarget(Tinfo, info))
			ChrMove::AngleControll(m_pOwner, Tinfo.cli_pos, 0.01f);
	}
}


//******************************************************
//	キャラクタのステート基底クラスのコンストラクタ
//******************************************************
CharacterMoveStateBase::CharacterMoveStateBase(LPCHARACTER pOwner):
m_pOwner(pOwner)
{
}

void CharacterMoveStateBase::UpdateTransMatrix()
{
	const float scale = 0.180f;
	Matrix temp;
	ChrFunc::GetTransMatrix(m_pOwner, temp, Vector3(scale, scale, scale));
	ChrFunc::GetModelPtr(m_pOwner)->SetMatrix(temp);
}

//******************************************************
//	キャラクタの自由移動クラス
//******************************************************

CharacterMoveStateUsualMode::CharacterMoveStateUsualMode(
	LPCHARACTER p):
	CharacterMoveStateBase(p),
	now_state(_USUAL)
{

}

void CharacterMoveStateUsualMode::Enter()
{

	ChrFunc::SetMotion(
		m_pOwner,
		chr_motion_parts::UPPER_BODY,
		chr_motion_type::AIM
		);

	ChrFunc::ResetMotion(
		m_pOwner,
		chr_motion_parts::LOWER_BODY
		);

	foot_sound_timer = 0;
	now_state = _USUAL;

	ChrMove::Shuffle(m_pOwner, false);
}

void CharacterMoveStateUsualMode::Execute()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);
	CRVector3 m = ChrFunc::GetParam(m_pOwner).m_move;

	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, CconstParam->move_max_xz_speed, CconstParam->move_down_speed, CconstParam->glavity);

	//ダメージチェック
	ChrMove::DamageStateUpdate(m_pOwner, ChrMove::UsualDamageCheck(m_pOwner));

	ChrFunc::GetModelPtr(m_pOwner)->SetBodyAngle(CharacterModelResouce::GetToTargetAngle(m_pOwner));
	ChrFunc::GetModelPtr(m_pOwner)->Animation(1);

	bool isWalk=ChrFunc::SetWalkingMotion(
		m_pOwner,
		Vector2(m.x, m.z),
		min(Vector3XZLength(m) / (CconstParam->move_max_xz_speed*0.2f), 1)
		);

	if (now_state == _USUAL)
	{
		ChrFunc::SetMotion(m_pOwner, chr_motion_parts::UPPER_BODY, chr_motion_type::AIM);
	}
	else if (now_state == _SHUFFLE)
	{
		ChrFunc::SetMotion(m_pOwner, chr_motion_parts::UPPER_BODY, chr_motion_type::SHUFFLE);
	}

	if (!m_pOwner->GetFrameInfo().isHitFloor)
	{
		ChrStateMsg::SendChangeStateMsg(m_pOwner, chr_move_state::jump_mode);
	}

	CHARACTER_BONE_TYPE touch_foot;

	if (++foot_sound_timer > 3 && isWalk && ChrFunc::PlayFootSound(m_pOwner, touch_foot))
	{
		//歩行時のエフェクト設定
		Vector3 pos;

		ChrFunc::GetBonePos(m_pOwner, touch_foot, pos);

		foot_sound_timer = 0;
		
		POLYGONEFFECT.SetEffect(
			pos,
			VECTOR3ZERO,
			VECTOR3ZERO,
			0.10f,
			EffectTex::_DUST,
			0xA0abaaa8,
			RS_COPY,
			18
			);
	}
}

void CharacterMoveStateUsualMode::Exit()
{
	ChrFunc::SetMotion(
		m_pOwner,
		chr_motion_parts::ALL_PARTS,
		chr_motion_type::AIM
		);

	ChrMove::Shuffle(m_pOwner, false);
}

bool CharacterMoveStateUsualMode::isCanChangeState(CHARACTER_STATE_TYPE change_state_type)
{
	switch (change_state_type)
	{
	case chr_move_state::error_mode:return false;
	case chr_move_state::jump_mode:return true;
	case chr_move_state::usual_mode:return true;
	case chr_move_state::near_atk_mode:return true;
	case chr_move_state::throw_grenade:return true;
	case chr_move_state::rolling:return true;

	case chr_move_state::parachute:return false;
		
	default:
		MyDebugString("認識できないステートタイプがCharacterMoveStateUsualMode::isCanChangeState関数に送られましたtype=%d ", (int)change_state_type);
		break;
	}
	return false;
}

bool CharacterMoveStateUsualMode::HandleMessage(
	LPCHARACTERSTATEMESSAGE msg)
{
	switch (msg->type)
	{
	case chr_state_msg_type::send_move_vector:		//移動ベクトルメッセージ
		ChrMove::AddMove(
			m_pOwner,
			msg->v1*(GetConstChrParam(m_pOwner)->move_acceleration + ChrFunc::GetParam(m_pOwner).UpValue.MoveSpeed));
		return true;
	case chr_state_msg_type::send_view_position:	//視線ベクトルメッセージ
		ChrMove::AngleSet(m_pOwner, msg->v1);
		return true;
	case chr_state_msg_type::send_wepon_fire_flg:
		if (ChrFunc::isShuffle(m_pOwner))
			return false;
		now_state = _USUAL;
		return ChrFunc::WeponFire(m_pOwner);
	case chr_state_msg_type::send_lock_on_flg:
		if (!ChrFunc::GetUI(m_pOwner)->isShuffle())
		{
			ChrMove::LockOn(m_pOwner, msg->v1.x > 0.1f);
			return true;
		}
	case chr_state_msg_type::send_shuffle_flg:
		if (msg->v1.x > 0.1f)
		{
			ChrFunc::SetPlusAlpha(m_pOwner, 1);
			now_state = _SHUFFLE;
		}
		else{
			now_state = _USUAL;
		}
		ChrMove::Shuffle(m_pOwner, msg->v1.x > 0.1f);
		return true;
	case chr_state_msg_type::send_lock_on_change_right_left:
		ChrMove::LockOnRLChange(m_pOwner);
		break;
	default:

		break;
	}

	
	//メッセージ受信失敗
	return false;
}




//******************************************************
//	キャラクタのジャンプモードクラス
//******************************************************
CharacterMoveStateJumpMode::CharacterMoveStateJumpMode(
	LPCHARACTER p) :
	CharacterMoveStateBase(p),
	m_glavity(0, GetConstChrParam(p)->glavity, 0),
	m_jump_power(GetConstChrParam(p)->jump_power),
	m_jump_frame((int)GetConstChrParam(p)->jump_frame),
	m_jump_end_frame((int)GetConstChrParam(p)->jump_end_frame),
	m_Sound_Timer(0)
{

}

//ジャンプステップ関数たち
void CharacterMoveStateJumpMode::JumpStartMode()
{
	//飛ぶ直前
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	//重力関係
	ChrMove::AddMove(m_pOwner, Vector3(0, CconstParam->glavity, 0));

	//床とのあたり判定チェック
	bool touch_floor = ChrMove::FloorCheck(m_pOwner);

	//ＸＺ軸の最大移動速度を設定
	ChrMove::SetMaxXZSpeed(m_pOwner, CconstParam->move_max_xz_speed);

	//位置を更新
	ChrMove::PositionUpdate(m_pOwner);

	//壁とのあたり判定チェック
	ChrMove::WallCheck(m_pOwner);

	//ＸＺ平面の最大加速度を現在の速度に設定(飛んでる最中にさらに加速するのはおかしいので)
	m_max_xz_speed =
		Vector3XZLength(ChrFunc::GetParam(m_pOwner).m_move) +
		Vector3XZLength(ChrFunc::GetParam(m_pOwner).m_parent_move);

	if (!touch_floor)
	{
		ChangeStep(MOVEDOWN);
		return;
	}

	if (++m_frame > m_jump_frame)
	{
		ChangeStep(MOVEUP);

		SOUNDPLAYER.PlaySE(
			sound_t::jump,
			SOUNDPLAYER.GetSoundVolume(ControllObjFunc::GetPos(m_pOwner), sound_t::jump)
			);

		//ジャンプ時の煙エフェクト
		for (int i = 0; i < 5; ++i)
		{
			POLYGONEFFECT.SetEffect(
				ControllObjFunc::GetPos(m_pOwner) + Vector3Rand()*0.1f,
				VECTOR3ZERO,
				VECTOR3ZERO,
				0.2f,
				EffectTex::_DUST,
				0xA0abaaa8,
				RS_ADD,
				18
				);
		}

		if (GetConstChrParam(m_pOwner)->jump_move_min_val > m_max_xz_speed)
			m_max_xz_speed = GetConstChrParam(m_pOwner)->jump_move_min_val;

		ChrMove::AddMove(m_pOwner, Vector3(0, GetConstChrParam(m_pOwner)->jump_power, 0));
	}
}

void CharacterMoveStateJumpMode::JumpMoveUpMode()
{
	m_frame++;
	//飛んでる最中
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	//重力関係
	ChrMove::AddMove(m_pOwner, Vector3(0, CconstParam->glavity, 0));


	//床とのあたり判定チェック
	bool touch_floor = ChrMove::FloorCheck(m_pOwner);

	//ＸＺ軸の最大移動速度を設定
	ChrMove::SetMaxXZSpeed(m_pOwner, m_max_xz_speed);


	//位置を更新
	ChrMove::PositionUpdate(m_pOwner);

	//壁とのあたり判定チェック
	ChrMove::WallCheck(m_pOwner);

	if (m_frame > 5 && touch_floor)
	{
		ChangeStep(FINISH);
		return;
	}

	//軌跡エフェクト
	if (m_frame < 10)
	{
		COLORf color(0xA0abaaa8);
		color.a = 1 - ((float)m_frame / 10.0f);
		color.ClampValue();

		POLYGONEFFECT.SetEffect(
			ControllObjFunc::GetPos(m_pOwner) + Vector3Rand()*0.1f,
			VECTOR3ZERO,
			VECTOR3ZERO,
			0.20f,
			EffectTex::_DUST,
			color.toDWORD(),
			RS_ADD,
			18
			);
	}

	if (ChrFunc::GetParam(m_pOwner).m_move.y<0)
	{
		ChangeStep(MOVEDOWN);
	}
}

void CharacterMoveStateJumpMode::JumpMoveDownMode()
{
	//飛んでる最中
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	//重力関係
	ChrMove::AddMove(m_pOwner, Vector3(0, CconstParam->glavity, 0));

	float pre_gl = ChrFunc::GetParam(m_pOwner).m_move.y;

	//床とのあたり判定チェック
	bool touch_floor = ChrMove::FloorCheck(m_pOwner);

	//ＸＺ軸の最大移動速度を設定
	ChrMove::SetMaxXZSpeed(m_pOwner, m_max_xz_speed);

	//位置を更新
	ChrMove::PositionUpdate(m_pOwner);

	//壁とのあたり判定チェック
	ChrMove::WallCheck(m_pOwner);


	if (ChrMove::FloorCheck(m_pOwner))
	{
		if (pre_gl < CconstParam->glavity * 10)
		{
			ChangeStep(FINISH);
		}
		else
			SendStateChangeMsg(m_pOwner, chr_move_state::usual_mode);
	}
}

void CharacterMoveStateJumpMode::JumpFinishMode()
{
	//着地した時
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, CconstParam->move_max_xz_speed, CconstParam->move_down_speed, CconstParam->glavity);

	if (++m_frame > m_jump_end_frame)
	{
		SendStateChangeMsg(m_pOwner, chr_move_state::usual_mode);
	}
}

void CharacterMoveStateJumpMode::ChangeStep(JUMPSTEP step)
//ステップ変更関数
{
	m_step = step;
	m_frame = 0;
	CHR_MOTION_TYPE motion = chr_motion_type::AIM;

	switch (step)
	{
	case START:
		motion = chr_motion_type::JUMP_START;
		break;
	case MOVEUP:
		motion = chr_motion_type::JUMP_START;
		break;
	case MOVEDOWN:
		motion = chr_motion_type::JUMP_MOVE_DOWN;
		break;
	case FINISH:
		motion = chr_motion_type::JUMP_END;
		SOUNDPLAYER.PlaySE(
			sound_t::jump,
			SOUNDPLAYER.GetSoundVolume(ControllObjFunc::GetPos(m_pOwner), sound_t::jump)
			);
		break;
	}
	ChrFunc::SetMotion(
		m_pOwner,
		chr_motion_parts::LOWER_BODY,
		motion
		);
}


void CharacterMoveStateJumpMode::Enter()
{
	ChangeStep(START);
	m_frame = 0;
	m_Sound_Timer = 9;
}

void CharacterMoveStateJumpMode::Execute()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	//ステップ関数の配列を作成
	void(CharacterMoveStateJumpMode::*jump_step_functions[])() =
	{
		&CharacterMoveStateJumpMode::JumpStartMode,	//ジャンプスタート
		&CharacterMoveStateJumpMode::JumpMoveUpMode,		//ジャンプ上昇中
		&CharacterMoveStateJumpMode::JumpMoveDownMode,	//ジャンプ下降中
		&CharacterMoveStateJumpMode::JumpFinishMode	//ジャンプ終了
	};

	//現在のstep番目を実行
	(this->*jump_step_functions[m_step])();

	//ダメージチェック
	ChrMove::DamageStateUpdate(m_pOwner, ChrMove::UsualDamageCheck(m_pOwner));

	ChrFunc::GetModelPtr(m_pOwner)->Animation(1);
}

void CharacterMoveStateJumpMode::Exit()
{

}

bool CharacterMoveStateJumpMode::isCanChangeState(CHARACTER_STATE_TYPE change_state_type)
{
	if (change_state_type == chr_move_state::parachute)return true;	//テスト
	return false;
}


bool CharacterMoveStateJumpMode::HandleMessage(
	LPCHARACTERSTATEMESSAGE msg)
{
	switch (msg->type)
	{
	case chr_state_msg_type::send_move_vector:
		if (m_step == MOVEDOWN || m_step == MOVEUP)
		{
			ChrMove::AddMove(m_pOwner, msg->v1*GetConstChrParam(m_pOwner)->jump_xz_acceleration);
		}
		return true;
	case chr_state_msg_type::send_view_position:
		ChrMove::AngleSet(m_pOwner, msg->v1);
		return true;
	case chr_state_msg_type::send_wepon_fire_flg:
		return ChrFunc::WeponFire(m_pOwner);
	case chr_state_msg_type::send_jump_state:
		ChangeStep((JUMPSTEP)((int)msg->v1.x));
		return true;
	case chr_state_msg_type::send_lock_on_flg:
		ChrMove::LockOn(m_pOwner, msg->v1.x > 0.1f);
		return true;

	case chr_state_msg_type::send_hover:
		if (m_step == MOVEDOWN || (m_step == MOVEUP&&m_frame > 20))
		{
			if (ChrMove::Hover(m_pOwner, 0.2f)&&++m_Sound_Timer > 10  )
			{
				m_Sound_Timer = 0;
				SOUNDPLAYER.PlaySE(sound_t::shot_weapon,
					SOUNDPLAYER.GetSoundVolume(m_pOwner->GetBodyCenter(), sound_t::shot_weapon)
					);
			}
		}
		return true;
	default:
		//	MyDebugString("メッセージ受信に失敗しています type=%d\n", msg->type);
		break;
	}



	return false;
}










//******************************************************
//	キャラクタの瀕死状態の移動クラス
//******************************************************

CharacterMoveStateDyingMode::CharacterMoveStateDyingMode(
	LPCHARACTER p) :
	CharacterMoveStateBase(p),
	step(&CharacterMoveStateDyingMode::step_begin),
	timer(0)
{

}

void CharacterMoveStateDyingMode::Enter()
{
	step = &CharacterMoveStateDyingMode::step_begin;
	timer = 0;
//	ChrFunc::GetModelPtr(m_pOwner)->SetMotion(chr_motion_parts::ALL_PARTS, chr_motion_type::DYING);
}

void CharacterMoveStateDyingMode::step_begin()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, CconstParam->dying_move_xz_max_speed, CconstParam->move_down_speed, CconstParam->glavity);
	ChrMove::UsualDamageCheck(m_pOwner);

	if (ChrFunc::isDead(m_pOwner))
	{
		//死んでいる場合の処理
		SendStateChangeMsg(m_pOwner, chr_move_state::die_mode);
		return;
	}

	//次のステップに移行
	if (timer > static_cast<int>(CconstParam->dying_begin_frame))
	{
		ChangeStep(&CharacterMoveStateDyingMode::step_move);
	}

	ChrFunc::GetModelPtr(m_pOwner)->Animation( 1);
}

void CharacterMoveStateDyingMode::step_move()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, CconstParam->dying_move_xz_max_speed, CconstParam->move_down_speed, CconstParam->glavity);
	
	//ダメージチェック
	ChrMove::UsualDamageCheck(m_pOwner);

	//毎フレーム体力を減少させる
	ChrFunc::SetDamageValue(m_pOwner, CconstParam->dying_life_damage_speed);

	if (ChrFunc::isDead(m_pOwner))
	{
		//死んでいる場合の処理
		SendStateChangeMsg(m_pOwner, chr_move_state::die_mode);
		return;
	}

	if (!ChrFunc::isDying(m_pOwner))
	{
		//瀕死でない場合(回復した場合)の処理
		SendStateChangeMsg(m_pOwner, chr_move_state::usual_mode);
		return;
	}
	ChrFunc::GetModelPtr(m_pOwner)->Animation(1);
}

void CharacterMoveStateDyingMode::step_end()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, CconstParam->dying_move_xz_max_speed, CconstParam->move_down_speed, CconstParam->glavity);
	ChrMove::UsualDamageCheck(m_pOwner);

	if (ChrFunc::isDead(m_pOwner))
	{
		//死んでいる場合の処理
		SendStateChangeMsg(m_pOwner, chr_move_state::die_mode);
		return;
	}

	if (ChrFunc::isDying(m_pOwner))
	{
		//瀕死の場合の処理
		ChangeStep(&CharacterMoveStateDyingMode::step_begin);
		return;
	}
	ChrFunc::GetModelPtr(m_pOwner)->Animation( 1);
}

void CharacterMoveStateDyingMode::ChangeStep(DyingModeStep next)
{
	this->step = next;
	timer = 0;
}

void CharacterMoveStateDyingMode::Execute()
{
	(this->*step)();
	if(timer<INT_MAX)timer++;
}

void CharacterMoveStateDyingMode::Exit()
{

}

bool CharacterMoveStateDyingMode::isCanChangeState(CHARACTER_STATE_TYPE change_state_type)
{
	return false;	//後に変わることができるステートが追加されるかもしれない？

	switch (change_state_type)
	{
	case chr_move_state::error_mode:return false;
	default:
		return false;
		break;
	}
	return false;
}

bool CharacterMoveStateDyingMode::HandleMessage(
	LPCHARACTERSTATEMESSAGE msg)
{
	if (step != &CharacterMoveStateDyingMode::step_move)return false;

	switch (msg->type)
	{
	case chr_state_msg_type::send_move_vector:		//移動ベクトルメッセージ
		ChrMove::AddMove(m_pOwner, msg->v1*GetConstChrParam(m_pOwner)->move_acceleration);
		return true;
	case chr_state_msg_type::send_view_position:	//視線ベクトルメッセージ
		ChrMove::AngleSet(m_pOwner, msg->v1);
		return true;
	default:
	//	MyDebugString("メッセージ受信に失敗しています type=%d\n", msg->type);
		break;
	}


	//メッセージ受信失敗
	return false;
}


//******************************************************
//	キャラクタの死亡状態のクラス
//******************************************************

CharacterMoveStateDieMode::CharacterMoveStateDieMode(
	LPCHARACTER p) :
	CharacterMoveStateBase(p),
	m_timer(0)
{

}

void CharacterMoveStateDieMode::SetDestroyEffect(
	CRVector3 pos,
	ENTITY_ID render_entity)
{
	GameMessage msg;
	TextureRect texrc;

	texrc.left = 0;
	texrc.top = 0;
	texrc.width = 2048;
	texrc.height = 2048;

	ShareIcon* icon = new ShareIcon(
		GAMEMATCHINFO.GetTextureMgr().GetTexturePtr(TextureManager::KILL_ENEMY),
		texrc,
		render_entity,
		pos,
		Vector3(0, 0.01f, 0),
		VECTOR3ZERO,
		Vector2(150, 70)*2.f,
		60,
		true
		);

	msg.to = game_id::id_screen_icon_manager;
	msg.from = game_id::id_error;
	msg.type = msg_t::send_screen_icon_msg;
	msg.exinfo = icon;

	IDMANAGER.Dispatch(&msg);

	ControllObjFunc::SetBlur(m_pOwner, 180, 1, Vector2(0.5f, 0.5f));
}

void CharacterMoveStateDieMode::AddScore()
{
	const ENTITY_ID killmePlayer =ChrFunc::GetParam(m_pOwner).m_last_damage_parent;
	LPCONTROLLOBJCT obj;

	if (IS_CONTROLLOBJ_ID(killmePlayer)&&
		(obj = (LPCONTROLLOBJCT)IDMANAGER.GetPointer(killmePlayer)))
	{
		ControllObjFunc::AddKillScore(obj);
	}

	ControllObjFunc::AddDeathScore(m_pOwner);
}

void CharacterMoveStateDieMode::Enter()
{
	m_timer = 0;
	ChrFunc::SetMotion(
		m_pOwner,
		chr_motion_parts::LOWER_BODY,
		chr_motion_type::DIE
		);
	ChrMove::SetExplositon(ControllObjFunc::GetPos(m_pOwner) + Vector3(0, 0.4f, 0), 1, m_pOwner);

	//ChrFunc::GetModelPtr(m_pOwner)->SetMotion(chr_motion_parts::ALL_PARTS, chr_motion_type::DYING);
}

void CharacterMoveStateDieMode::Execute()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);
	
	if (m_timer++ == static_cast<int>(CconstParam->revive_time))
	{
		AddScore();
		SetDestroyEffect(m_pOwner->GetBodyCenter() + Vector3(0, 0.5f, 0), ChrFunc::GetParam(m_pOwner).m_last_damage_parent);
		ControllObjFunc::SetBlur(
			ChrFunc::RespornCharacter(m_pOwner),
			120,
			10,
			Vector2(0.5f, 0.5f));
		return;
	}
	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, CconstParam->dying_move_xz_max_speed, CconstParam->move_down_speed, CconstParam->glavity);
	ChrFunc::GetModelPtr(m_pOwner)->Animation(1);
}

void CharacterMoveStateDieMode::Exit()
{

}

bool CharacterMoveStateDieMode::isCanChangeState(CHARACTER_STATE_TYPE change_state_type)
{
	return false;
}

bool CharacterMoveStateDieMode::HandleMessage(
	LPCHARACTERSTATEMESSAGE msg)
{
	//メッセージ受信失敗
	return false;
}


//******************************************************
//	キャラクタの近接攻撃のクラス
//******************************************************

CharacterMoveStateNearAttackMode::CharacterMoveStateNearAttackMode(
	LPCHARACTER p) :
	CharacterMoveStateBase(p),
	m_timer(0),
	m_Target(game_id::id_error),
	m_cola_atk((DamageObjectManager*)IDMANAGER.GetPointer(game_id::id_damage_manager)),
	m_kick_atk((DamageObjectManager*)IDMANAGER.GetPointer(game_id::id_damage_manager))
{
	m_kick_atk.param.parent=m_cola_atk.param.parent = m_pOwner->GetId();

	m_cola_atk.SetSize(0.8f);
	m_kick_atk.SetSize(0.8f);

	m_cola_atk.param.type = m_kick_atk.param.type = (damage_type_bits::enemy_damage | damage_type_bits::except_myself | damage_type_bits::team_cure_damage);
	m_cola_atk.param.val = m_kick_atk.param.val = 70;
}

CharacterMoveStateNearAttackMode::~CharacterMoveStateNearAttackMode()
{
	
}

void CharacterMoveStateNearAttackMode::FollowTarget(LPCHARACTER p)
{
	Vector3 move_pos = ControllObjFunc::GetPos(p);
	const Vector3& pos = ControllObjFunc::GetPos(m_pOwner);

	if (ABS(move_pos.y - pos.y) > 1)return;

	ChrMove::AngleControll(m_pOwner, p->GetBodyCenter(), 0.2f);
}

void CharacterMoveStateNearAttackMode::FrontMove(LPCHARACTER p)
{
	Vector3 v = ChrFunc::GetFront(p);
	v.y = 0;
	ChrMove::AddMove(m_pOwner, Vector3Normalize(v)*0.2f);
	ChrMove::SetMaxXZSpeed(m_pOwner, 1.00f);
}

LPCharacter CharacterMoveStateNearAttackMode::GetTarget()
{
	class funcobj
	{
		CRVector3 pos, view;	//viewは正規化しておくこと
		Vector3 rpos, rvec, rout;
		float	rdist;
		float	len, nearl;
		Vector3 tmp;
		LPCONTROLLOBJCT target,me;
	public:
		funcobj(LPCONTROLLOBJCT me, CRVector3 pos, CRVector3 view, float length) :pos(pos), view(view), nearl(length), target(nullptr), me(me){}
		void operator()(LPCONTROLLOBJCT you)
		{
			if (me == you || typeid(*you) != typeid(Character) || ChrFunc::isDead((LPCHARACTER)you))return;

			CRVector3 p = ControllObjFunc::GetPos(you);
			VECTOR3POINTTOPOINT(len, p, pos, tmp);
			if (len > nearl)return;
			VECTOR3NORMALIZE(tmp);
			if (acosf(VECTOR3DOT(tmp, view)) > PI_d4)return;
			rpos = pos;
			rvec=tmp;
			rdist = len;

			if (MAP_MANAGER.RayPick(&rout, &rpos, &rvec, &rdist))return;
			len = nearl;
			target = you;
		}
		bool GetTarget(LPCONTROLLOBJCT* ppOut)
		{
			*ppOut = target;
			return (target!=nullptr);
		}
	};
	LPCONTROLLOBJCT ret;
	Vector3 pos, view;

	pos = m_pOwner->GetBodyCenter();
	view = ChrFunc::GetFront(m_pOwner);
	view.Normalize();

	funcobj f(m_pOwner,pos, view, 5);
	CONTROLLOBJMGR.For_each(f);

	if (f.GetTarget(&ret))
		return (LPCharacter)ret;

	return nullptr;
}

void CharacterMoveStateNearAttackMode::Enter()
{
	m_timer = 0;

	LPCHARACTER pTarget=GetTarget();
	if (pTarget)m_Target = pTarget->GetId();
	
	m_step = ATK1;

	if (pTarget&&ChrFunc::isDying(pTarget))
		m_step = ATK2;

	m_cola_atk.param.hit_count = 0;

	ChrFunc::SetMotion(m_pOwner, chr_motion_parts::ALL_PARTS, chr_motion_type::NATK_COLA);
}

CharacterMoveStateNearAttackMode::ATK_STEP CharacterMoveStateNearAttackMode::ColaAttack()
{
	const int dFrame = (int)GetConstChrParam(m_pOwner)->near_atk_cola_damage_frame;
	LPCHARACTER pTarget = (LPCHARACTER)IDMANAGER.GetPointer(m_Target);
	++m_timer;

	if (m_timer == dFrame)
	{
		m_cola_atk.param.pos = m_pOwner->GetBodyCenter() + Vector3Normalize(ChrFunc::GetFront(m_pOwner))*0.50f;
		m_cola_atk.Apper();
		m_cola_atk.SetPos(m_cola_atk.param.pos);
	}
	if (m_timer == dFrame + 1)
	{
		m_cola_atk.Exit();
	}

	if (m_timer<dFrame)
	{
		FrontMove(m_pOwner);
		if (pTarget)
			FollowTarget(pTarget);
	}

	if (m_timer > dFrame + 2)
	{
		m_timer = 0;
		if (m_cola_atk.param.hit_count)
			return ATK2;
		else 
			return FINISH;
	}
	return ATK1;
}

CharacterMoveStateNearAttackMode::ATK_STEP CharacterMoveStateNearAttackMode::KickAttack()
{
	return FINISH;
}


CharacterMoveStateNearAttackMode::ATK_STEP CharacterMoveStateNearAttackMode::FinishMode()
{

	SendStateChangeMsg(m_pOwner, chr_move_state::usual_mode);

	return FINISH;
}


void CharacterMoveStateNearAttackMode::Execute()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	ATK_STEP(CharacterMoveStateNearAttackMode::*step_func[])() =
	{
		&CharacterMoveStateNearAttackMode::ColaAttack,
		&CharacterMoveStateNearAttackMode::KickAttack,
		&CharacterMoveStateNearAttackMode::FinishMode
	};

	m_step = (this->*step_func[m_step])();

	ChrMove::UsualDamageCheck(m_pOwner);

	//ダメージチェック
	ChrMove::DamageStateUpdate(m_pOwner, ChrMove::UsualDamageCheck(m_pOwner));


	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, CconstParam->dying_move_xz_max_speed, CconstParam->move_down_speed, CconstParam->glavity);
	ChrFunc::GetModelPtr(m_pOwner)->Animation(1);
}

void CharacterMoveStateNearAttackMode::Exit()
{
	m_cola_atk.Exit();
	m_kick_atk.Exit();
}

bool CharacterMoveStateNearAttackMode::isCanChangeState(CHARACTER_STATE_TYPE change_state_type)
{
	return false;
}

bool CharacterMoveStateNearAttackMode::HandleMessage(
	LPCHARACTERSTATEMESSAGE msg)
{
	//メッセージ受信失敗
	return false;
}


//******************************************************
//	キャラクタのグレネード投擲クラス
//******************************************************
CharacterMoveStateThrowGrenade::CharacterMoveStateThrowGrenade(
	LPCHARACTER p) :
	CharacterMoveStateBase(p)
{
}

CharacterMoveStateThrowGrenade::~CharacterMoveStateThrowGrenade()
{

}

void CharacterMoveStateThrowGrenade::SetGaugeMoveTarget()
{
	Vector3 target;
	Vector3 hand_pos; 
	Vector2 size(0.05f, 0.05f);
	const RECTI& rc = GAMEMATCHINFO.GetScreenSize();
	LPCAMERA pCamera = ControllObjFunc::GetCamera(m_pOwner);

	ChrFunc::GetBonePos(m_pOwner, chr_bone_type::RIGHT_ARM_WEAPON, hand_pos);

	pCamera->ClientToScreen(target, hand_pos);

	target.x = (target.x + 1)*0.5f*(float)rc.width;
	target.y = (-target.y + 1)*0.5f*(float)rc.height;

	size *= (float)rc.width;

	if (target.z > 0)
		ChrFunc::SetGaugeMove(m_pOwner, Vector2(target.x, target.y), size);

}

void CharacterMoveStateThrowGrenade::ThrowGrenade()
{
	GameMessage msg;
	DamageParam param;

	Vector3 move, target;
	Vector3 pos = m_pOwner->GetBodyCenter() + Vector3Normalize(ChrFunc::GetParam(m_pOwner).m_view_vec)*0.2f;

	const float glavity = -0.005f;

	param.color = CODMgr::GetTeamColaWaterColor(ControllObjFunc::GetTeamType(m_pOwner));
	param.hit_count = 0;
	param.parent = m_pOwner->GetId();
	param.pos = Vector3(0, 5, 0);
	param.type = damage_type_bits::team_cure_damage | damage_type_bits::except_myself | damage_type_bits::enemy_damage;
	param.val = 1;
	param.vec = Vector3(0, 0, 0);

	ChrFunc::GetUI(m_pOwner)->GetTargetPos(target);

	Grenade::GetMoveValue(move, pos, target, glavity);

	msg.exinfo = new Grenade(
		GAMEMATCHINFO.GetMeshMgr().GetMeshPtr((MeshManager::MESH_ID)GameMatchInfomation::_COLA_GUN),
		pos,
		move,
		glavity,
		ChrFunc::GetChargePower(m_pOwner),
		0.18f,
		param,
		ControllObjFunc::GetTeamType(m_pOwner)
		);

	msg.from = m_pOwner->GetId();
	msg.to = game_id::id_bullet_manager;
	msg.type = msg_t::add_bullet;

	IDMANAGER.Dispatch(&msg);
}

void CharacterMoveStateThrowGrenade::Enter()
{
	m_timer = 0;
	ChrFunc::SetMotion(m_pOwner, chr_motion_parts::UPPER_BODY, chr_motion_type::THROW);
}




void CharacterMoveStateThrowGrenade::Execute()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);
	CRVector3 m = ChrFunc::GetParam(m_pOwner).m_move;

	m_timer++;

	if (m_timer < 20)
	{
		SetGaugeMoveTarget();
	}

	if (m_timer == 20)
	{
		ThrowGrenade();
		ChrFunc::ResetGauge(m_pOwner);
	}

	if (m_timer >= 70)
	{
		SendStateChangeMsg(m_pOwner, chr_move_state::usual_mode);
	}

	ChrFunc::GetModelPtr(m_pOwner)->SetBodyAngle(CharacterModelResouce::GetToTargetAngle(m_pOwner));

	ChrFunc::SetWalkingMotion(
		m_pOwner,
		Vector2(m.x, m.z),
		min(Vector3XZLength(m) / (CconstParam->move_max_xz_speed*0.2f), 1)
		);

	ChrMove::UsualDamageCheck(m_pOwner);

	//ダメージチェック
	ChrMove::DamageStateUpdate(m_pOwner, ChrMove::UsualDamageCheck(m_pOwner));


	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, CconstParam->dying_move_xz_max_speed, CconstParam->move_down_speed, CconstParam->glavity);
	ChrFunc::GetModelPtr(m_pOwner)->Animation(1);
}

void CharacterMoveStateThrowGrenade::Exit()
{

}

bool CharacterMoveStateThrowGrenade::isCanChangeState(CHARACTER_STATE_TYPE change_state_type)
{
	return false;
}

bool CharacterMoveStateThrowGrenade::HandleMessage(
	LPCHARACTERSTATEMESSAGE msg)
{
	switch (msg->type)
	{
	case chr_state_msg_type::send_move_vector:		//移動ベクトルメッセージ
		ChrMove::AddMove(m_pOwner, msg->v1*GetConstChrParam(m_pOwner)->move_acceleration);
		return true;
	case chr_state_msg_type::send_view_position:	//視線ベクトルメッセージ
		ChrMove::AngleSet(m_pOwner, msg->v1);
		return true;
	case chr_state_msg_type::send_lock_on_flg:
		ChrMove::LockOn(m_pOwner, msg->v1.x > 0.1f);
		return true;

	default:break;

	}


	//メッセージ受信失敗
	return false;
}




//******************************************************
//	キャラクタのゴロゴロクラス
//******************************************************

CharacterMoveStateMoveRolling::CharacterMoveStateMoveRolling(
	LPCHARACTER p) :
	CharacterMoveStateBase(p),
	m_DamageObj((DamageObjectManager*)IDMANAGER.GetPointer(game_id::id_damage_manager)),
	m_isTouchFloor(false)
{
	m_DamageObj.Exit();
	m_RollAngle = 0;
}

CharacterMoveStateMoveRolling::~CharacterMoveStateMoveRolling()
{

}


void CharacterMoveStateMoveRolling::Enter()
{	
	ChangeState(_PreRoll);
	m_RollAngle = 0;
	m_DamageObj.Exit();
}

void CharacterMoveStateMoveRolling::UpdateTransMatrix()
{
	Matrix R,T;
	CharacterMoveStateBase::UpdateTransMatrix();

	if (m_State != _Rolling)return;

	Vector3 bone;
	ChrFunc::GetBoneLocalPos(m_pOwner, chr_bone_type::UPPER_BODY, bone);

	D3DXMatrixTranslation(&T, 0, -bone.y, 0);
	D3DXMatrixRotationX(&R, m_RollAngle);

	R = T*R;

	D3DXMatrixTranslation(&T, 0, bone.y, 0);

	R *= T;

	ChrFunc::GetModelPtr(m_pOwner)->SetMatrix(
		R*ChrFunc::GetModelPtr(m_pOwner)->GetMatrix()
		);

}

void CharacterMoveStateMoveRolling::ChangeState(STATE_TYPE		NextState)
{
	switch (NextState)
	{
	case _PreRoll:
		ChrFunc::ResetMotion(
			m_pOwner,
			chr_motion_parts::ALL_PARTS
			);
		ChrFunc::SetMotion(m_pOwner, chr_motion_parts::ALL_PARTS, chr_motion_type::ROLL_START);
		ChrFunc::SetMotionSpeed(m_pOwner, chr_motion_parts::ALL_PARTS, 2);
		ChrMove::AddMove(m_pOwner, Vector3(0, GetConstChrParam(m_pOwner)->jump_power, 0));
		m_Timer = 0;
		break;
	case _Rolling:
		ChrFunc::SetMotion(m_pOwner, chr_motion_parts::ALL_PARTS, chr_motion_type::ROLLING);
		m_DamageObj.Apper();
		m_Timer = 0;
		break;
	case _UsualFinish:
		m_DamageObj.Exit();
		ChrFunc::SetMotion(m_pOwner, chr_motion_parts::ALL_PARTS, chr_motion_type::ROLLUSUALEND);
		ChrFunc::SetMotionSpeed(m_pOwner, chr_motion_parts::ALL_PARTS, 2);
		m_Timer = 0;
		break;
	case _ImpactFinish:
		m_DamageObj.Exit();
		ChrFunc::SetMotion(m_pOwner, chr_motion_parts::ALL_PARTS, chr_motion_type::ROLLIMPACTEND);
		m_Timer = 0;
		break;
	}
	m_State = NextState;
}

void CharacterMoveStateMoveRolling::AngleControll(CRVector3 v)
{
	const CHRPARAM& param = ChrFunc::GetParam(m_pOwner);
	Vector3 m = param.m_move;
	Vector3 xzv(0, 0, 0);
	const float xz_l = Vector3XZLength(m);
	
	ChrMove::AngleControllAxisY(
		m_pOwner,
		ControllObjFunc::GetPos(m_pOwner) + v,
		0.025f
		);

	xzv.x = param.m_view_vec.x;
	xzv.z = param.m_view_vec.z;
	
	xzv.Normalize();
	xzv *= xz_l;

	xzv.x -= m.x;
	xzv.z -= m.z;

	ChrMove::AddMove(m_pOwner, xzv);
}

void CharacterMoveStateMoveRolling::SetDamageParams()
{
	using namespace damage_type_bits;

	DamageObject& d = m_DamageObj;

	d.SetSize(0.5f);
	d.param.hit_count = 0;
	d.param.color = CODMgr::GetTeamColor(ControllObjFunc::GetTeamType(m_pOwner));
	d.param.parent = m_pOwner->GetId();
	d.param.type = blow_off_damage | enemy_damage | except_myself;
	d.param.val = GetConstChrParam(m_pOwner)->rollingAtkValue;
}

void CharacterMoveStateMoveRolling::StartEffect()
{
	if (m_isSuccess)
	{
		ControllObjFunc::SetBlur(m_pOwner, 60, 0.5f, Vector2(0.5f, 0.5f));

		const Vector3 glavity(0, -0.003f, 0);
		Vector3 vec = Vector3Normalize(-m_MoveVec);
		Vector3 move, pos = m_pOwner->GetBodyCenter();

		for (int i = 0; i < 8; i++)
		{
			move = vec;

			move.x += fRAND(-0.1f, 0.1f);
			move.z += fRAND(-0.1f, 0.1f);
			move.y += fRAND(-0.05f, 0.2f);
			move *= fRAND(0.1f, 0.3f);

			POLYGONEFFECT.SetEffect(
				pos,
				move,
				glavity,
				fRAND(0.005f, 0.01f),
				EffectTex::_SOUR,
				m_DamageObj.param.color,
				RS_COPY,
				80 + rand() % 60
				);
		}

		vec *= 0.02f;

		for (int i = 0; i < 4; ++i)
		{
			move = pos;

			move.x += fRAND(-0.25f, 0.25f);
			move.y += fRAND(-0.25f, 0.25f);
			move.z += fRAND(-0.25f, 0.25f);

			POLYGONEFFECT.SetEffect(
				move,
				vec,
				VECTOR3ZERO,
				1.5f,
				EffectTex::_SMOKE,
				0xFFFFFFFF,
				RS_ADD
				);
		}
	}
	else{

		POLYGONEFFECT.SetEffect(
			m_pOwner->GetBodyCenter(),
			VECTOR3ZERO,
			VECTOR3ZERO,
			0.6f,
			EffectTex::_SMOKE,
			0xFFFFFFFF,
			RS_ADD
			);
	}
}

void CharacterMoveStateMoveRolling::UpdateDamageClass()
{
	m_DamageObj.param.pos = m_pOwner->GetBodyCenter();
	m_DamageObj.SetPos(m_pOwner->GetBodyCenter());
	m_DamageObj.param.vec = ChrFunc::GetParam(m_pOwner).m_move;
}

void CharacterMoveStateMoveRolling::StepPreRoll()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	if (m_Timer > 45)
	{
		ChangeState(_Rolling);

		//
		m_MoveVec = ChrFunc::GetParam(m_pOwner).m_view_vec;
		m_MoveVec.y = 0;
		m_MoveVec.Normalize();

		m_isSuccess = ChrFunc::GetChargePower(m_pOwner) > CconstParam->rolliingNeedCharge;
		m_Speed = (m_isSuccess) ? (1.0f) : (0.2f);
		m_Pre_pos = ControllObjFunc::GetPos(m_pOwner);

		SetDamageParams();
		UpdateDamageClass();
		m_DamageObj.Apper();

		StartEffect();

		ChrMove::AddMove(
			m_pOwner,
			m_MoveVec*(CconstParam->rolliingMovePower*m_Speed));

		if (m_isSuccess)
		{
			SOUNDPLAYER.PlaySE(sound_t::rolling_start,
				SOUNDPLAYER.GetSoundVolume(m_pOwner->GetBodyCenter(), sound_t::rolling_start)
				);
		}

		return;
	}

	//ダメージチェック
	ChrMove::DamageStateUpdate(m_pOwner, ChrMove::UsualDamageCheck(m_pOwner));


	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, CconstParam->dying_move_xz_max_speed, CconstParam->move_down_speed, CconstParam->glavity * 2);

}

void CharacterMoveStateMoveRolling::StepRolling()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);
	const float subValue = -0.02f;

	if (m_isSuccess)
	{
		ControllObjFunc::SetBlur(m_pOwner, 20, 0.2f, Vector2(0.5f, 0.5f));
		m_RollAngle += 0.85f;
	}
	else
	{
		m_RollAngle += 0.15f;
	}

	if (m_Timer<static_cast<int>(CconstParam->rolliingNeedCharge / (-subValue)))
	{
		ChrFunc::GetUI(m_pOwner)->AddColaGauge(subValue);
	}

	m_Pre_pos = ControllObjFunc::GetPos(m_pOwner);

	//ダメージチェック
	ChrMove::DamageStateUpdate(m_pOwner, ChrMove::UsualDamageCheck(m_pOwner));

	//移動更新
	m_isTouchFloor = ChrMove::UpdateAllMoveFunc(m_pOwner, 100, 0.001f, CconstParam->glavity*1.0f);

	//あたり判定位置の更新
	UpdateDamageClass();


	if ((m_Timer > 120 && m_isTouchFloor) ||
		(m_isSuccess == false && m_Timer >= 20 && m_isTouchFloor))
	{
		ChangeState(_UsualFinish);
	}
	else if (
		m_isSuccess&&
		m_Timer > 5 &&
		Vector3Length(m_Pre_pos - ControllObjFunc::GetPos(m_pOwner)) < ChrFunc::GetParam(m_pOwner).m_move.Length()*0.6f
		)
	{
		Vector3 v = ChrFunc::GetParam(m_pOwner).m_view_vec;
		v.Normalize();
		v.y = 0;
		v *= -1.0f;
		v.y = CconstParam->jump_power*0.5f;
		ChrMove::AddMove(m_pOwner, v);
		ChangeState(_ImpactFinish);
	}
}

void CharacterMoveStateMoveRolling::StepUsualFinish()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	if (m_Timer > 43)
	{
		SendStateChangeMsg(m_pOwner, chr_move_state::usual_mode);
	}

	//ダメージチェック
	ChrMove::DamageStateUpdate(m_pOwner, ChrMove::UsualDamageCheck(m_pOwner));


	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, 1000, CconstParam->move_down_speed, CconstParam->glavity);

}

void CharacterMoveStateMoveRolling::StepImpactFinish()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	if (m_Timer > 45)
	{
		SendStateChangeMsg(m_pOwner, chr_move_state::usual_mode);
	}

	//ダメージチェック
	ChrMove::DamageStateUpdate(m_pOwner, ChrMove::UsualDamageCheck(m_pOwner));


	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, CconstParam->dying_move_xz_max_speed, CconstParam->move_down_speed, CconstParam->glavity);

}

void CharacterMoveStateMoveRolling::Execute()
{
	void(CharacterMoveStateMoveRolling::*functions[])()=
	{
		&CharacterMoveStateMoveRolling::StepPreRoll,
		&CharacterMoveStateMoveRolling::StepRolling,
		&CharacterMoveStateMoveRolling::StepUsualFinish,
		&CharacterMoveStateMoveRolling::StepImpactFinish,
	};

	ChrFunc::GetModelPtr(m_pOwner)->SetBodyAngle(VECTOR3ZERO);

	(this->*functions[m_State])();

	if (m_Timer < INT_MAX)m_Timer++;
	else m_Timer = 0;

	ChrFunc::GetModelPtr(m_pOwner)->Animation(1);
}

void CharacterMoveStateMoveRolling::Exit()
{
	ChrFunc::SetMotion(m_pOwner, chr_motion_parts::LOWER_BODY, chr_motion_type::STAND);
	m_DamageObj.Exit();
	
//	ChrMove::LockOn(m_pOwner, false);
}

bool CharacterMoveStateMoveRolling::isCanChangeState(CHARACTER_STATE_TYPE change_state_type)
{
	if (change_state_type == chr_move_state::jump_mode&&
		m_isTouchFloor&&
		m_State == _Rolling)
	{
		ChrMove::AddMove(m_pOwner, Vector3(0, GetConstChrParam(m_pOwner)->jump_power*0.65f, 0));
	}
	return false;
}

bool CharacterMoveStateMoveRolling::HandleMessage(
	LPCHARACTERSTATEMESSAGE msg)
{
	switch (msg->type)
	{
	case chr_state_msg_type::send_move_vector:		//移動ベクトルメッセージ
		if (m_State == _Rolling)
		{
			AngleControll(msg->v1);
		}
		break;
	default:
		break;
	}


	//メッセージ受信失敗
	return false;
}



//******************************************************
//	キャラクタのふきとび状態のステート
//******************************************************

CharacterMoveStateVanish::CharacterMoveStateVanish(LPCHARACTER p):
CharacterMoveStateBase(p),
m_vec(1,1,0),
m_nowStep(),
m_Timer(0)
{

}

CharacterMoveStateVanish::~CharacterMoveStateVanish()
{

}

void CharacterMoveStateVanish::Bound(CRVector3 normal)
{
	ChrMove::Bound(m_pOwner, normal, 0.6f);
}

void CharacterMoveStateVanish::StepFunc_Vanish()
{
	//飛んでる最中
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);
	Vector3 Normal;

	//重力関係
	ChrMove::AddMove(m_pOwner, Vector3(0, CconstParam->glavity, 0));

	float pre_gl = ChrFunc::GetParam(m_pOwner).m_move.y;

	//床とのあたり判定チェック
	bool touch_floor = ChrMove::FloorCheck(m_pOwner);

	//位置を更新
	ChrMove::PositionUpdate(m_pOwner);

	//壁とのあたり判定チェック
	bool wall = ChrMove::WallCheckGetNormal(m_pOwner, Normal);

	//if (もし地面についていたら)
	if (ChrMove::FloorCheck(m_pOwner))
	{
		if (ChrFunc::isDead(m_pOwner))
		{
			SendStateChangeMsg(m_pOwner, chr_move_state::die_mode);
		}
		else{
			m_nowStep = &CharacterMoveStateVanish::StepFunc_VanishFinish;
		}
	}
	else if (wall)
	{
		if (ChrFunc::isDying(m_pOwner))
		{
			SendStateChangeMsg(m_pOwner, chr_move_state::die_mode);
		}
		else{
			Bound(Normal);
		}
	}
}

void CharacterMoveStateVanish::StepFunc_VanishFinish()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	//ダメージチェック
	ChrMove::DamageStateUpdate(m_pOwner, ChrMove::UsualDamageCheck(m_pOwner));

	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, 100, CconstParam->move_down_speed, CconstParam->glavity);

	if (++m_Timer > 60)
	{
		SendStateChangeMsg(m_pOwner, chr_move_state::usual_mode);
	}
}

void CharacterMoveStateVanish::Enter()
{
	ChrFunc::SetMotion(
		m_pOwner,
		chr_motion_parts::ALL_PARTS,
		chr_motion_type::VANISH
		);

	m_Timer = 0;
	m_nowStep = &CharacterMoveStateVanish::StepFunc_Vanish;

	ChrMove::AddMove(m_pOwner, m_vec);
	ChrMove::AngleSet(m_pOwner, Vector3Normalize(-m_vec));

	LPCAMERA cam = ControllObjFunc::GetCamera(m_pOwner);

	cam->SetNewState(
		new UsualPlayerMoveCamera(
		cam,
		m_pOwner,
		0.1f,
		0.1f,
		Vector3(0.32f, 1.2f, -3.3f),
		Vector3(0.32f, 1.2f, 0))
		);
}

void CharacterMoveStateVanish::Execute()
{
	(this->*m_nowStep)();

	ChrFunc::GetModelPtr(m_pOwner)->Animation(1);
}

void CharacterMoveStateVanish::Exit()
{
	ChrMove::LockOn(m_pOwner,false);
}

bool CharacterMoveStateVanish::isCanChangeState(
	CHARACTER_STATE_TYPE change_state_type)
{
	return false;
}

bool CharacterMoveStateVanish::HandleMessage(
	LPCHARACTERSTATEMESSAGE msg)
{
	if (msg->type != chr_state_msg_type::send_vanish_vec)
		return false;

	m_vec = msg->v1;

	return true;
}


//******************************************************
//	キャラクタの自由移動クラス
//******************************************************

CharacterMoveStateParachuteMove::CharacterMoveStateParachuteMove(
	LPCHARACTER p) :
	CharacterMoveStateBase(p),
	m_MaxLeaveFrame(60),
	m_LeaveFrame(0)
{

}

void CharacterMoveStateParachuteMove::SetPlayerPos()
{
	ChrFunc::GetBonePos(m_pOwner, chr_bone_type::HEAD_PULLTAB, m_ParachutePos);
}

void CharacterMoveStateParachuteMove::Enter()
{

	ChrFunc::SetMotion(
		m_pOwner,
		chr_motion_parts::LOWER_BODY,
		chr_motion_type::STAND
		);

	ChrFunc::SetMotion(
		m_pOwner,
		chr_motion_parts::UPPER_BODY,
		chr_motion_type::AIM
		);

	ChrMove::Shuffle(m_pOwner, false);
	m_LeaveFrame = 0;
}

void CharacterMoveStateParachuteMove::Execute()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);
	CRVector3 m = ChrFunc::GetParam(m_pOwner).m_move;


	//解除タイマー
	if (m_LeaveFrame > 0)
	{
		m_LeaveFrame++;
		m_ParachutePos += m_ParachuteMove;
		m_ParachuteMove.y += 0.001f;
	}
	else{
		//移動量の更新
		m_ParachuteMove = m_ParachutePos;
		SetPlayerPos();

		m_ParachuteMove -= m_ParachutePos;
		m_ParachuteMove = -m_ParachuteMove;
	}

	if (m_LeaveFrame > m_MaxLeaveFrame)
	{
		SendStateChangeMsg(m_pOwner, chr_move_state::usual_mode);
	}
	

	//移動更新
	ChrMove::UpdateParachuteMoveFunc(m_pOwner, CconstParam->move_max_xz_speed*0.25f, 0.01f);

	//ダメージチェック
	ChrMove::DamageStateUpdate(m_pOwner, ChrMove::UsualDamageCheck(m_pOwner));

	ChrFunc::GetModelPtr(m_pOwner)->SetBodyAngle(CharacterModelResouce::GetToTargetAngle(m_pOwner));
	ChrFunc::GetModelPtr(m_pOwner)->Animation(1);

	if (m_LeaveFrame == 0 && m_pOwner->GetFrameInfo().isHitFloor)
	{
		m_LeaveFrame = 1;
	}
}

void CharacterMoveStateParachuteMove::Exit()
{
	ChrMove::Shuffle(m_pOwner, false);
}

void CharacterMoveStateParachuteMove::Render()
{
	LPIEXMESH pMesh = GAMEMATCHINFO.GetMeshMgr().GetMeshPtr((MeshManager::MESH_ID)GameMatchInfomation::_PARACHUTE);
	const float scale = 0.09f;

	D3DXMatrixScaling(&pMesh->TransMatrix, scale, scale, scale);
	
	pMesh->TransMatrix._41 = m_ParachutePos.x;
	pMesh->TransMatrix._42 = m_ParachutePos.y;
	pMesh->TransMatrix._43 = m_ParachutePos.z;

	pMesh->Render(RS_COPY, 0.5f);// 1 - ((float)m_LeaveFrame / (float)m_MaxLeaveFrame));
}

bool CharacterMoveStateParachuteMove::isCanChangeState(CHARACTER_STATE_TYPE change_state_type)
{
	return false;
}

bool CharacterMoveStateParachuteMove::HandleMessage(
	LPCHARACTERSTATEMESSAGE msg)
{
	switch (msg->type)
	{
	case chr_state_msg_type::send_move_vector:		//移動ベクトルメッセージ
		ChrMove::AddMove(
			m_pOwner,
			msg->v1*(GetConstChrParam(m_pOwner)->move_acceleration + ChrFunc::GetParam(m_pOwner).UpValue.MoveSpeed));
		return true;
	case chr_state_msg_type::send_view_position:	//視線ベクトルメッセージ
		ChrMove::AngleSet(m_pOwner, msg->v1);
		return true;
	case chr_state_msg_type::send_wepon_fire_flg:
		if (ChrFunc::isShuffle(m_pOwner))
			return false;
		return ChrFunc::WeponFire(m_pOwner);
	case chr_state_msg_type::send_lock_on_flg:
		if (!ChrFunc::GetUI(m_pOwner)->isShuffle())
		{
			ChrMove::LockOn(m_pOwner, msg->v1.x > 0.1f);
			return true;
		}
	case chr_state_msg_type::send_shuffle_flg:
		ChrMove::Shuffle(m_pOwner, msg->v1.x > 0.1f);
		return true;
	case chr_state_msg_type::send_lock_on_change_right_left:
		ChrMove::LockOnRLChange(m_pOwner);
		break;
	default:

		break;
	}


	//メッセージ受信失敗
	return false;
}


//******************************************************
//	キャラクタのリスポーン時のクラス
//******************************************************


CharacterMoveStateResporn::CharacterMoveStateResporn(LPCHARACTER p):
CharacterMoveStateBase(p),
m_isTouchFloor(false)
{
	m_RollAngle = 0;
}


void CharacterMoveStateResporn::Enter()
{
	ChangeState(_PreRoll);
	m_RollAngle = 0;
}

void CharacterMoveStateResporn::UpdateTransMatrix()
{
	Matrix R, T;
	CharacterMoveStateBase::UpdateTransMatrix();

	if (m_State != _Rolling)return;

	Vector3 bone;
	ChrFunc::GetBoneLocalPos(m_pOwner, chr_bone_type::UPPER_BODY, bone);

	D3DXMatrixTranslation(&T, 0, -bone.y, 0);
	D3DXMatrixRotationX(&R, m_RollAngle);

	R = T*R;

	D3DXMatrixTranslation(&T, 0, bone.y, 0);

	R *= T;

	ChrFunc::GetModelPtr(m_pOwner)->SetMatrix(
		R*ChrFunc::GetModelPtr(m_pOwner)->GetMatrix()
		);

}


void CharacterMoveStateResporn::ChangeState(STATE_TYPE		NextState)
{
	switch (NextState)
	{
	case _PreRoll:
		ChrFunc::ResetMotion(
			m_pOwner,
			chr_motion_parts::ALL_PARTS
			);
		ChrFunc::SetMotion(m_pOwner, chr_motion_parts::ALL_PARTS, chr_motion_type::ROLL_START);
		ChrFunc::SetMotionSpeed(m_pOwner, chr_motion_parts::ALL_PARTS, 2);
		m_Timer = 0;
		break;
	case _Rolling:
		ChrFunc::SetMotion(m_pOwner, chr_motion_parts::ALL_PARTS, chr_motion_type::ROLLING);
		m_Timer = 0;
		break;
	case _UsualFinish:
		ChrFunc::SetMotion(m_pOwner, chr_motion_parts::ALL_PARTS, chr_motion_type::ROLLUSUALEND);
		ChrFunc::SetMotionSpeed(m_pOwner, chr_motion_parts::ALL_PARTS, 2);
		m_Timer = 0;
		break;

	}
	m_State = NextState;
}

void CharacterMoveStateResporn::StartEffect()
{
	ControllObjFunc::SetBlur(m_pOwner, 60, 0.5f, Vector2(0.5f, 0.5f));

	const Vector3 glavity(0, -0.003f, 0);
	Vector3 vec = Vector3Normalize(-m_MoveVec);
	Vector3 move, pos = m_pOwner->GetBodyCenter();

	vec *= 0.02f;

	for (int i = 0; i < 4; ++i)
	{
		move = pos;

		move.x += fRAND(-0.25f, 0.25f);
		move.y += fRAND(-0.25f, 0.25f);
		move.z += fRAND(-0.25f, 0.25f);

		POLYGONEFFECT.SetEffect(
			move,
			vec,
			VECTOR3ZERO,
			1.5f,
			EffectTex::_SMOKE,
			0xFFFFFFFF,
			RS_ADD
			);
	}

	SOUNDPLAYER.PlaySE(
		sound_t::apper_vending,
		SOUNDPLAYER.GetSoundVolume(pos, sound_t::apper_vending)
		);
}

void CharacterMoveStateResporn::StepPreRoll()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	ChangeState(_Rolling);

	m_MoveVec = ChrFunc::GetParam(m_pOwner).m_view_vec;
	m_MoveVec.y = 0;
	m_MoveVec.Normalize();


	m_Speed = 0.5f;
	m_Pre_pos = ControllObjFunc::GetPos(m_pOwner);


	StartEffect();

	ChrMove::AddMove(
		m_pOwner,
		m_MoveVec*(CconstParam->rolliingMovePower*m_Speed) +
		Vector3(0, CconstParam->jump_power, 0)
		);

}


void CharacterMoveStateResporn::StepRolling()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);
	
	m_RollAngle += 0.55f;

	m_Pre_pos = ControllObjFunc::GetPos(m_pOwner);

	//移動更新
	m_isTouchFloor = ChrMove::UpdateAllMoveFunc(m_pOwner, 100, 0.001f, CconstParam->glavity*1.0f);

	if (m_Timer == 40)
	{
		LPCAMERA cam = ControllObjFunc::GetCamera(m_pOwner);

		cam->SetNewState(
			new UsualPlayerMoveCamera(
			cam,
			m_pOwner,
			0.01f,
			0.025f,
			Vector3(0.32f, 1.2f, -3.3f),
			Vector3(0.32f, 1.2f, 0))
			);
	}

	if (m_Timer > 40 && m_isTouchFloor)
	{
		ChangeState(_UsualFinish);
	}
}

void CharacterMoveStateResporn::StepUsualFinish()
{
	LPCHARACTERCONSTPARAMETERS CconstParam = GetConstChrParam(m_pOwner);

	if (m_Timer > 43)
	{
		SendStateChangeMsg(m_pOwner, chr_move_state::usual_mode);
	}

	//ダメージチェック
	ChrMove::DamageStateUpdate(m_pOwner, ChrMove::UsualDamageCheck(m_pOwner));


	//移動更新
	ChrMove::UpdateAllMoveFunc(m_pOwner, 1000, CconstParam->move_down_speed, CconstParam->glavity);

}


void CharacterMoveStateResporn::Execute()
{
	void(CharacterMoveStateResporn::*functions[])() =
	{
		&CharacterMoveStateResporn::StepPreRoll,
		&CharacterMoveStateResporn::StepRolling,
		&CharacterMoveStateResporn::StepUsualFinish,
	};

	ChrFunc::GetModelPtr(m_pOwner)->SetBodyAngle(VECTOR3ZERO);

	(this->*functions[m_State])();

	if (m_Timer < INT_MAX)m_Timer++;
	else m_Timer = 0;

	ChrFunc::GetModelPtr(m_pOwner)->Animation(1);
}

void CharacterMoveStateResporn::Exit()
{
	ChrMove::LockOn(m_pOwner, false);
	ChrFunc::SetMotion(m_pOwner, chr_motion_parts::LOWER_BODY, chr_motion_type::STAND);
}


void CharacterMoveStateResporn::Render()
{

}

bool CharacterMoveStateResporn::isCanChangeState(CHARACTER_STATE_TYPE change_state_type)
{

	return false;
}

bool CharacterMoveStateResporn::HandleMessage(LPCHARACTERSTATEMESSAGE msg)
{

	return false;
}