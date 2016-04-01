#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "iextreme.h"
#include "../JoyPad.h"
//***********************************************
//	�R���g���[���[�Ɋւ���w�b�_
//***********************************************

//�O���錾����
struct ControllerButtonSet;

namespace controller
{
	typedef int CONTROLLER_NUM;	//�R���g���[���ԍ�

	namespace button
	{
		enum button_type
		{
			//�{�^���^�C�v
			maru,
			batu,
			shikaku,
			sankaku,

			//RL�{�^��
			_R1,
			_R2,
			_R3,
			_L1,
			_L2,
			_L3,

			//�����L�[�^�C�v
			up,
			down,
			right,
			left,

			max,//�{�^���^�C�v�ő吔
		};
	}
	namespace stick{
		enum stick_type
		{
			//�X�e�B�b�N�^�C�v
			left,
			right,

			max//�X�e�B�b�N�^�C�v�ő吔
		};
	}
	//�������u�Ԃ��ǂ���
	bool	GetTRG(button::button_type x, CONTROLLER_NUM num);
	//�������u�Ԃ��ǂ���
	bool	GetLeave(button::button_type x, CONTROLLER_NUM num);
	//�����Ă��邩�ǂ���
	bool	GetPush(button::button_type x, CONTROLLER_NUM num);
	//�R���g���[���[��U���Ă��邩�ǂ���
	bool	GetVibration(ControllerButtonSet& cnt);
	//�X�e�B�b�N�̌X���̒l�𓾂�
	Vector2 GetStickValue(stick::stick_type x, CONTROLLER_NUM num);
	
	//�w�肵���R���g���[��(�I�v�V�����Őݒ肵�����]�Ƃ��X�e�B�b�N�̊��x��K�p��������)�̃X�e�B�b�N�̌X���̒l�𓾂�
	Vector2 GetMyControllerSettingStickValue(
		stick::stick_type				stick_t,
		const ControllerButtonSet&		info
		);

	//�w�肵���R���g���[���[��U��������
	void SetVibration(RATE power, int frame, CONTROLLER_NUM num);
}

//�f�t�H���g��button�Z�b�g��ݒ肷��
void SetUsualButtonSet(ControllerButtonSet& out, controller::CONTROLLER_NUM number);

//�R���g���[��Button��stick�z�u�\����
struct ControllerButtonSet
{
	controller::CONTROLLER_NUM		controller_num;
	controller::button::button_type ButtonSet[controller::button::max];	//Button�z�u
	controller::stick::stick_type	StickSet[controller::stick::max];	//Stick�z�u
	bool	stick_reverse		[controller::stick::max][2];			//stick���]�t���O(0��x�A�P��y�̃t���O�������Ă���)
	float	stick_sensitivity	[controller::stick::max];				//stick���xLR
	JoyPad	ps3_parameter;												//�W���C���R���g���[���p�p�����^
public:
	ControllerButtonSet();
};


#endif