#ifndef __CHARACTER_MOVE_STATE_MESSAGE_FUNC_H__
#define __CHARACTER_MOVE_STATE_MESSAGE_FUNC_H__

#include "Character.h"
#include "FriendMessage.h"

//*************************************************************
//	�L�����N�^�ւ̈ړ����߃��b�Z�[�W�𑗂�N���X
//*************************************************************

class CharacterMoveStateMessageFunc;
typedef CharacterMoveStateMessageFunc ChrStateMsg;


class CharacterMoveStateMessageFunc
{
private:
	CharacterMoveStateMessageFunc();
	~CharacterMoveStateMessageFunc();
private:
	static void SendViewMsg(LPCHARACTER p, CRVector3 view_vec);
public:
	static void SendChangeStateMsg(LPCHARACTER p, CHARACTER_STATE_TYPE set_state_type);		//�w�肵���X�e�[�g�ɕύX���郁�b�Z�[�W�𑗂�(�K�؂ȃX�e�[�g�o�Ȃ������ꍇ���������)
	static void SendMoveVecMsg(LPCHARACTER p, CRVector3 move_vec);							//�ړ��x�N�g�����i�[�������b�Z�[�W���쐬���A���M����
	static void SendRotateViewPosMsg(LPCHARACTER p,CRVector3 view_pos,RADIAN rotate_speed);	//�w�肵���ꏊ�Ɍ������Ď�����speed��������
	static void SendRotateYViewMsg(LPCHARACTER p, RADIAN rotate_speed);						//�L�����N�^��Y����rotate_speed��������]������
	static void SendRotateXViewMsg(LPCHARACTER p, RADIAN rotate_speed);						//�L�����N�^��X����rotate_speed��������]������
public:
	static void SendShuffleMsg(LPCHARACTER p, bool shuffle);	//shuffle���邩�ǂ����̃��b�Z�[���𑗂�
	static void SendWeponFireMsg(LPCHARACTER p);											//���݂̕���𔭎˂��郁�b�Z�[�W�𑗂�
	static void SendLockOnMsg(LPCHARACTER p, bool lockOn);									//���b�N�I�����邩�ǂ����̃��b�Z�[�W�𑗂�
	static void SendChangeCameraPosMsg(LPCHARACTER p);									//���b�N�I����Ԃł̍��E�؂�ւ����b�Z�[�W�𑗂�
	static void SendJumpHoverMsg(LPCHARACTER p);
public:
	static void SendFriendMsgIcon(LPCHARACTER p, FriendMessageIcon::MsgType type);
};

#endif