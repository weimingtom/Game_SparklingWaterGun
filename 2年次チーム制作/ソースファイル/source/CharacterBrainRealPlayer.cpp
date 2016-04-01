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
	//�L�[�l���擾
	Vector2 stickL = controller::GetMyControllerSettingStickValue(controller::stick::left, m_MyControllerButtonSet);
	Vector2 stickR = controller::GetMyControllerSettingStickValue(controller::stick::right, m_MyControllerButtonSet);

	LPCCAMERA cam = ControllObjFunc::GetCamera(p);

	Vector3 move =
		Vector3Normalize(CameraFunction::GetFront(cam))*stickL.y +
		Vector3Normalize(CameraFunction::GetRight(cam))*stickL.x;

	move.y = 0;

	//�L�����N�^�̃X�e�[�g�Ɉړ��ʃ��b�Z�[�W�𑗂�
	ChrStateMsg::SendMoveVecMsg(p, move);

	//�����Ă��������ς��郁�b�Z�[�W�𑗂�
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

	//�W�����v
	if (controller::GetTRG(controller::button::batu, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendChangeStateMsg(p, chr_move_state::jump_mode);
	}

	//�z�o�[
	if (controller::GetPush(controller::button::batu, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendJumpHoverMsg(p);
	}

	//�ߋ����U��
	//if (controller::GetTRG(controller::button::maru, m_MyControllerButtonSet.controller_num))
	//{
	//	ChrStateMsg::SendChangeStateMsg(p, chr_move_state::near_atk_mode);
	//}

	//����
	if (controller::GetPush(controller::button::_R1, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendWeponFireMsg(p);
	}

	//���b�N�I��
	if (controller::GetTRG(controller::button::_L1, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendLockOnMsg(p,true);
	}
	if (controller::GetLeave(controller::button::_L1, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendLockOnMsg(p, false);
	}

	//�J�������E�؂�ւ�
	if (controller::GetTRG(controller::button::_L3, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendChangeCameraPosMsg(p);
	}

	//�O���l�[�h����
	if (controller::GetTRG(controller::button::_L2, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendChangeStateMsg(p, chr_move_state::throw_grenade);
	}

	//�]����
	if (controller::GetTRG(controller::button::maru, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendChangeStateMsg(p, chr_move_state::rolling);
	}

#ifdef _DEBUG
	//�p���V���[�g(�f�o�b�O�p)
	if (controller::GetTRG(controller::button::shikaku, m_MyControllerButtonSet.controller_num))
	{
		ChrStateMsg::SendChangeStateMsg(p, chr_move_state::parachute);
	}
#endif

	//�V���b�t��(�R���g���[���ɂ���Đ؂�ւ�)
#if USE_CONTROLLER_TYPE == PS3_CONTRLLER
	ChrStateMsg::SendShuffleMsg(p, controller::GetVibration(m_MyControllerButtonSet));
	m_MyControllerButtonSet.ps3_parameter.Update();
#else
	ChrStateMsg::SendShuffleMsg(p, controller::GetPush(controller::button::_R2, m_MyControllerButtonSet.controller_num));
#endif

	//�����ւ̃��b�Z�[�W�A�C�R��
	UpdateKeyMsgIcon(p);
	
}

bool CharacterBrainRealPlayer::HandleMessage(LPGAMEMESSAGE msg)
{
	return false;
}