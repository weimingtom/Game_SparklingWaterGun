#ifndef __JOYPAD_H_
#define __JOYPAD_H_

#include "iextreme.h"
#include <list>

class JoyPad
{
private:
	struct GyroValue
	{
		int X;
		int Y;
	};
	typedef std::list<GyroValue> ValueList;
private:
	ValueList			m_List;
	int					m_Search_flame;
	int					m_Shake_value;
	int					m_Controller_num;
	bool				m_ShakeFlg;
public:
	JoyPad(
		UINT	search_flame,	//���݂̃t���[������U�������m���鎞��
		UINT	shake_value		//���̒l�ُ�̌X����U�������̂Ƃ���
		);
	~JoyPad();
	void operator =(const JoyPad& pd);
private:
	void DebugChangeValue();
	//�X���̒l���X�V
	void PushNowGyroValue();
public:
	//�X���̔�����X�V
	void Update();
public:
	bool ShakeJuge();				//	�U�������ǂ����𓾂�
public:
	void SetControllerNumber(int num);	//�R���g���[���ԍ���ݒ�
};



#endif