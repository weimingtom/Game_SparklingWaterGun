#ifndef __CHARACTER_MOVE_H__
#define __CHARACTER_MOVE_H__

#include "Character.h"
#include "CODBOForwardDecl.h"
#include "FriendMessage.h"

//*****************************************************
//	�L�����N�^�̓����Ɋւ���֐����܂Ƃ߂��N���X
//*****************************************************

class	CharacterMoveFunctions;
typedef CharacterMoveFunctions	ChrMove;
typedef DWORD DAMAGEFLAGS;

class CharacterMoveFunctions : private Character
{
private:
	CharacterMoveFunctions();
	~CharacterMoveFunctions();
private:
	static bool					UpCheck(LPCHARACTER C);
public:
	//���t���[�����i�X�V����֐��̂܂Ƃ�
	static bool					UpdateAllMoveFunc(LPCHARACTER C, float max_speed_val, RATE down_speed_val, float glavity);	//�߂�l�͒n�ʂɂ��Ă��邩
	static void					UpdateAllDamageFunc(LPCHARACTER C);
	static void					UpdateAimTargetInfo(LPCHARACTER C, LPTEAMVIEWDATA viewData);
	static bool					UpdateParachuteMoveFunc(LPCHARACTER C, float max_speed_val, float down_speed);
public:
	//�ړ��֌W
	static void					AddMove(LPCHARACTER C, CRVector3 move);		//�w�肵�������Ɉړ��ʂ����Z����
	static void					PositionUpdate(LPCHARACTER C);				//���W�Ɉړ��ʂ����Z����
	static void					DownSpeed(LPCHARACTER C, RATE val);			//�ړ��ʂ�val�̊�����������������
	static void					SetMaxXZSpeed(LPCHARACTER C, float max_val);//XZ���ʂ̍ő�ړ��ʂ�max_val�Ɋۂ߂�
	static bool					FloorCheck(LPCHARACTER C);					//���Ƃ̂����蔻��`�F�b�N
	static bool					CeilingCheck(LPCHARACTER C);
	static bool					WallCheck(LPCHARACTER C);					//�ǂƂ̂����蔻��`�F�b�N
	static bool					WallCheckGetNormal(LPCHARACTER C, Vector3& out_normal);//�ǂƂ̂����蔻��`�F�b�N(���������ǂ̖@��������)
	static void					Bound(LPCHARACTER C,CRVector3 normal, RATIO r);
	static bool					Hover(LPCHARACTER C, RATIO power);
public:
	//�p�x�Ƃ������֌W
	static void					AngleSet(LPCHARACTER C, CRVector3 viewvec);//�w�肵�������Ɍ�������(��u��)
	static void					AngleControll(LPCHARACTER C, CRVector3 view_pos, RADIAN speed);//�w�肵���ꏊ�Ɍ�������speed�̑��x�Ō���
	static void					AngleControllAxisY(LPCHARACTER C, CRVector3 view_pos, RADIAN speed);//�w�肵���ꏊ�Ɍ�������speed�̑��x�Ō���
	static void					CheckMaxViewUpAndDown(LPCHARACTER C, Vector3& v);	//�L�����N�^���㉺�Ɍ��������Ă����ꍇ�ɕ␳����
public:
	//�����蔻��֌W
	static void					Vanish(LPCHARACTER C, CRVector3 move);
	static DAMAGEFLAGS			UsualDamageCheck(LPCHARACTER C);
	static void					SetDamageEffect(LPCHARACTER C, DAMAGEFLAGS flgs, const DamageParam& dParam);
public:
	//��ԑJ�ڊ֌W
	static CHARACTER_STATE_TYPE	DamageStateUpdate(LPCHARACTER C, DAMAGEFLAGS flgs);
	static void					Shuffle(LPCHARACTER C,bool flg);			//�V���J�V���J���邩�ǂ���
	static void					LockOn(LPCHARACTER C, bool flg);			//���b�N�I�����邩�ǂ���
	static void					LockOnRLChange(LPCHARACTER C);			//���b�N�I�����̃J�������E�؂�ւ�
public:
	static void					AddShuffleGauge(LPCHARACTER C,RATIO power);
public:
	static void					SetExplositonBlur(CRVector3 pos, RATIO power);
	static void					SetExplositon(CRVector3 pos, RATIO power, LPCHARACTER C);
	static void					SetExplositon(CRVector3 pos, RATIO power, const DamageParam& param, float damage_size);
public:
	//�A�C�R��
	static void					SetFriendMessegeIcon(LPCHARACTER C, FriendMessageIcon::MsgType type);
};

#endif
