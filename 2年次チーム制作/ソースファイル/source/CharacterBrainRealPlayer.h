#ifndef __CHARACTER_BRAIN_REAL_PLAYER_H__
#define __CHARACTER_BRAIN_REAL_PLAYER_H__

#include "CharacterBrain.h"

//**************************************************************
//		�v���C���̑��삷��ꍇ�̃L�����N�^�̔]�N���X
//**************************************************************
class CharacterBrainRealPlayer :public CharacterBrainBase
{
	ControllerButtonSet m_MyControllerButtonSet;	//�{�^���z�u���
public:
	CharacterBrainRealPlayer(LPCONTROLLERBUTTONSET pMyControllerButtonSet);	
	~CharacterBrainRealPlayer();
private:
	void UpdateKeyMsgIcon(LPCHARACTER p);
public:
	void Update(LPCHARACTER p);	//�L�����N�^���X�V
	bool HandleMessage(LPGAMEMESSAGE msg);	//���b�Z�[�W
};

#endif