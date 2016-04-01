#include "All.h"

CharacterBrainRealPlayer::CharacterBrainRealPlayer(
	LPCONTROLLERBUTTONSET pMyControllerButtonSet):
	m_MyControllerButtonSet(*pMyControllerButtonSet)
{

}

CharacterBrainRealPlayer::~CharacterBrainRealPlayer()
{

}

void CharacterBrainRealPlayer::UpdateKeyMsgIcon(LPCHARACTER p)
{
	bool isSet = false;
	FriendMessageIcon::MsgType type;

	if (controller::GetTRG(controller::button::up, m_MyControllerButtonSet.controller_num))
	{
		isSet = true;
		type = FriendMessageIcon::_GO;
	}
	if (controller::GetTRG(controller::button::right, m_MyControllerButtonSet.controller_num))
	{
		isSet = true;
		type = FriendMessageIcon::_OK;
	}
	if (controller::GetTRG(controller::button::down, m_MyControllerButtonSet.controller_num))
	{
		isSet = true;
		type = FriendMessageIcon::_COME_ON;
	}
	if (controller::GetTRG(controller::button::left, m_MyControllerButtonSet.controller_num))
	{
		isSet = true;
		type = FriendMessageIcon::_HELP;
	}

	if (isSet)
	{
		ChrStateMsg::SendFriendMsgIcon(p, type);
	}
}

void CharacterBrainRealPlayer::Update(LPCHARACTER p)
{
	//キー値を取得
	Vector2 stickL = controller::GetMyControllerSettingStickValue(controller::stick::left, m_MyControllerButtonSet);
	Vector2 stickR = controller::GetMyControllerSettingStickValue(controller::stick::right, m_MyControllerButtonSet);

	LPCCAMERA cam = ControllObjFunc::GetCamera(p);

	Vector3 move =
		Vector3Normalize(CameraFunction::GetFront(cam))*stickL.y +
		Vector3Normalize(CameraFunction::GetRight(cam))*stickL.x;

	move.y = 0;

	//キャラクタのステートに移動量メッセージを送る
	ChrStateMsg::SendMoveVecMsg(p, move);

	//向いている方向を変えるメッセージを送る
	const float rotate_speed = GetConstChrParam(p)->rotate_speed;

	if (ChrFunc::isZoom(p))
	{
		stickR *= 0.5f;
	}

	if (ABS(stickR.y) > 0.1f)
	{
		ChrStateMsg::SendRotateXViewMsg(p, stickR.y*rotate_speed);
	}

	if (ABS(stickR.x) > 0.1f)
	{
		ChrStateMsg::SendRotateYViewMsg(p, stickR.x*rotate_speed);
	}

	//ジャンプ
	if (controller::GetTRG(controller::button::batu, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendChangeStateMsg(p, chr_move_state::jump_mode);
	}

	//ホバー
	if (controller::GetPush(controller::button::batu, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendJumpHoverMsg(p);
	}

	//近距離攻撃
	//if (controller::GetTRG(controller::button::maru, m_MyControllerButtonSet.controller_num))
	//{
	//	ChrStateMsg::SendChangeStateMsg(p, chr_move_state::near_atk_mode);
	//}

	//武器
	if (controller::GetPush(controller::button::_R1, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendWeponFireMsg(p);
	}

	//ロックオン
	if (controller::GetTRG(controller::button::_L1, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendLockOnMsg(p,true);
	}
	if (controller::GetLeave(controller::button::_L1, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendLockOnMsg(p, false);
	}

	//カメラ左右切り替え
	if (controller::GetTRG(controller::button::_L3, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendChangeCameraPosMsg(p);
	}

	//グレネード投擲
	if (controller::GetTRG(controller::button::_L2, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendChangeStateMsg(p, chr_move_state::throw_grenade);
	}

	//転がる
	if (controller::GetTRG(controller::button::maru, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendChangeStateMsg(p, chr_move_state::rolling);
	}

#ifdef _DEBUG
	//パラシュート(デバッグ用)
	if (controller::GetTRG(controller::button::shikaku, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendChangeStateMsg(p, chr_move_state::parachute);
	}
#endif

	//シャッフル(コントローラによって切り替え)
#if USE_CONTROLLER_TYPE == PS3_CONTRLLER
	ChrStateMsg::SendShuffleMsg(p, controller::GetVibration(m_MyControllerButtonSet));
	m_MyControllerButtonSet.ps3_parameter.Update();
#else
	ChrStateMsg::SendShuffleMsg(p, controller::GetPush(controller::button::_R2, m_MyControllerButtonSet.controller_num));
#endif

	//味方へのメッセージアイコン
	UpdateKeyMsgIcon(p);
	
}

bool CharacterBrainRealPlayer::HandleMessage(LPGAMEMESSAGE msg)
{
	return false;
}