#ifndef __CHARACTER_MESH_MANAGER_H__
#define __CHARACTER_MESH_MANAGER_H__

#include "iextreme.h"
#include "GameSystem\GameBase.h"
#include "Character.h"
#include "CODBO.h"

//******************************************************
//		�L�����N�^�̃��f���Ǘ��w�b�_�[
//******************************************************

//************************************
//���[�V�������w�肷�镔�ʂ̗�
//************************************
namespace chr_motion_parts
{
	enum _ENUM_CHARACTER_MOTION_PARTS
	{
		ALL_PARTS,	//�S����
		LOWER_BODY,	//�㔼�g
		UPPER_BODY,	//�����g

		_PARTS_MAX,
	};
}

typedef chr_motion_parts::_ENUM_CHARACTER_MOTION_PARTS	CHR_MOTION_PARTS;	//�ł���΂�������ry

//************************************
//���[�V�������^�C�v�̗�
//************************************
namespace chr_motion_type
{
	enum _ENUM_CHARACTER_MOTION_TYPE
	{
		STAND = 1,			//�_����
		AIM = 28, //�\�����[�V����

		MOVE_FRONT = 3,			//�O�����ړ�
		MOVE_RIGHT = 9,			//�E�����ړ�
		MOVE_LEFT = 7,			//�������ړ�
		MOVE_BACK = 5,			//�������ړ�

		DIE = 22,			//���S���
		DYING,				//�m�����
		DYING_MOVE_FRONT,	//�m����Ԃ̑O�ړ�
		DYING_MOVE_RIGHT,	//�m����Ԃ̉E�ړ�
		DYING_MOVE_BACK,	//�m����Ԃ̌��ړ�
		DYING_MOVE_LEFT,	//�m����Ԃ̍��ړ�

		THROW = 19,				//�O���l�[�h������
		DAMAGE_BODY,		//�ڂ��ڂ����

		JUMP_START = 11,			//�W�����v�X�^�[�g
		JUMP_MOVE_DOWN = 12,			//�W�����v�X�^�[�g
		JUMP_END = 13,			//���n

		ROLL_START = 34,
		ROLLING = 35,
		ROLLIMPACTEND = 43,
		ROLLUSUALEND = 36,

		VANISH = 38,
		VANISH_FINISH = 39, 

		NATK_COLA = 19,			//�R�[���ŉ���
		NATK_KICK = 20,			//�R��

		SHUFFLE = 31,
		HOVER=44,
		HOVER_END = 45,

		APEER_VENDING_MACHINE=41,	//���̋@����łĂ���
	};
}

typedef chr_motion_type::_ENUM_CHARACTER_MOTION_TYPE CHR_MOTION_TYPE;	//�ł���΂�������ry

namespace chr_bone_type
{
	enum _ENUM_CHARACTER_BONE_TYPE
	{
		HEAD_PULLTAB = 13,
		UPPER_BODY = 11,
		LOWER_BODY = 0,

		EYE_RIGHT = 4,
		EYE_LEFT = 4,

		RIGHT_ARM_1 = 9,
		RIGHT_ARM_2 = 10,
		RIGHT_ARM_3 = 12,
		RIGHT_ARM_WEAPON = 26,

		LEFT_ARM_1 = 14,
		LEFT_ARM_2 = 15,
		LEFT_ARM_3 = 17,
		LEFT_ARM_WEAPON = 31,

		RIGHT_LEG_1 = 19,
		RIGHT_LEG_2 = 20,
		RIGHT_LEG_3 = 3,

		LEFT_LEG_1 = 24,
		LEFT_LEG_2 = 25,
		LEFT_LEG_3 = 8,

		BODY_DAMAGE_1 = 2,
		BODY_DAMAGE_2 = 2,
		BODY_DAMAGE_3 = 2,
		BODY_DAMAGE_4 = 2, 

	};
}

typedef chr_bone_type::_ENUM_CHARACTER_BONE_TYPE CHARACTER_BONE_TYPE;

//************************************
//�L�����N�^�̃��f���x�[�X�N���X
//************************************
class CharacterModelBase
{
public:
	virtual ~CharacterModelBase(){}
public:
	//���f����`��
	virtual void Render(LPCHARACTER C) = 0;
	virtual void Render(LPCHARACTER C, char* shader_name) = 0;

	virtual const Matrix& GetMatrix()const = 0;
	virtual void SetMatrix(CRMatrix mat) = 0;

	virtual void AnimationParts(
		CHR_MOTION_PARTS	parts,	//�A�j���[�V�������镔��
		float				speed,	//�o�߃X�s�[�h
		int					line_no	//���[�V�������C��
		) = 0;

	virtual void Animation(float	speed) = 0;//�o�߃X�s�[�h
	virtual void UsualAnimation() = 0;

	virtual void SetBodyAngle(CRVector3 v) = 0;

	virtual void SetMotion(
		CHR_MOTION_TYPE		motion,
		CHR_MOTION_PARTS	parts,
		int					line_no,
		RATIO				blend,	//���C���P��Blend�䗦
		bool				update
		) = 0;

	virtual void SetSpeed(
		CHR_MOTION_PARTS	parts,
		int					line_no,
		RATIO				speed
		) = 0;

	virtual void SetWeight(
		CHR_MOTION_PARTS	parts,
		int					line_no,
		RATIO				w
		) = 0;

	virtual void GetWeight(
		CHR_MOTION_PARTS	parts,
		RATIO*	pOutArray
		) = 0;

	virtual const Matrix& GetBoneMatrix(CHARACTER_BONE_TYPE type) = 0;

	virtual void SetMotionFrame(
		CHR_MOTION_PARTS	parts,
		int					line_no,
		float				frame) = 0;

	virtual float GetFrame(
		CHR_MOTION_PARTS	parts,
		int					line_no
		) = 0;

	virtual float GetMotionFrame(
		CHR_MOTION_PARTS	parts,
		int					line_no
		) = 0;
};

typedef CharacterModelBase* LPCHARACTERMODELBASE;

#endif