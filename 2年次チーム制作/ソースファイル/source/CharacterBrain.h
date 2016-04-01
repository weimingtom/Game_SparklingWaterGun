#ifndef __CHARACTER_BRAIN_H__
#define __CHARACTER_BRAIN_H__

#include "Character.h"
#include "GameSystem\GameController.h"

//**************************************************************
//		�L�����N�^�̔]�N���X�x�[�X
//**************************************************************
class CharacterBrainBase
{
private:
public:
	virtual ~CharacterBrainBase(){}
	virtual void Update(LPCHARACTER p) = 0;				//���t���[���̍X�V
	virtual bool HandleMessage(LPGAMEMESSAGE msg) = 0;	//���b�Z�[�W
};

/*
���̃N���X���p�����ăv���C���[����N���X��COM����N���X��������
*/


#endif // __CHARACTER_BRAIN_H__
