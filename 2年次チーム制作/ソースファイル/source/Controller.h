#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "ControllObject.h"
#include "ControllObjectFunction.h"
#include "GameSystem\GameController.h"


//*****************************************************************//
//	�Q�[�����v���C�����(�v���C���ƃR���s���[�^����)�̃N���X	
//	���삷��ControllObject�ւ̃|�C���^������
//*****************************************************************//

class Controller
{
protected:
	PLAYERMATCHDATA				m_chr_match_data;		//���̃L�����N�^�̃}�b�`�f�[�^
	LPCONTROLLOBJCT				m_pControllObj;			//���������삵�Ă���I�u�W�F�N�g�ւ̃|�C���^
public:
	Controller(const PLAYERMATCHDATA& chr_match_data) :
		m_pControllObj(nullptr),
		m_chr_match_data(chr_match_data)
	{
	}
	~Controller(){}
public:
	inline ControllObject* GetControllObjectPtr()
	{
		return m_pControllObj;
	}
	inline const PLAYERMATCHDATA& GetMatchData()const
	{
		return m_chr_match_data;
	}
	void SetControllObj(LPCONTROLLOBJCT p)
	{
		m_pControllObj = p;
		if (p)ControllObjFunc::SetMatchDataPtr(p, &m_chr_match_data);
	}
	void Controll()
	{
		//���t���[���̃L�����N�^�X�V
		SAFE_EXE(m_pControllObj, Controll);
	}
};


typedef Controller*	 LPCONTROLLER;

#endif