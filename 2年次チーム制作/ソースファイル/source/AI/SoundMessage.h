#ifndef __SOUND_MESSAGE_H__
#define __SOUND_MESSAGE_H__

#include "../EnumCP11.h"
//***************************************************************
//	�������b�Z�[�W
//***************************************************************

_ENUM_BEGIN(SOUND_TYPE, sound_type)
	error = -1,
	foot,	//��������
	gun,	//�e��

	num_envsound,
	_ENUM_END(SOUND_TYPE, sound_type)

//���̂����ꏊ���ڂ�
struct Sound_Msg
{
	
	SOUND_TYPE			type;
	Vector3						source_Pos;	//���̈ʒu
	float							power;					//���̑傫��
	int								end_Frame;	//���̏����t���[���iend_Frame - current_Frame�j
	Sound_Msg(){}
	Sound_Msg(const SOUND_TYPE& t, const Vector3& pos, const int& end_F, const float& p)
	{
		type = t;		source_Pos = pos;		end_Frame = end_F;	power = p;
	}
};

#endif
