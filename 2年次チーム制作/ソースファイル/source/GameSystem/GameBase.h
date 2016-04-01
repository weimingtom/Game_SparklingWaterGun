#ifndef __GAME_BASE_H__
#define __GAME_BASE_H__

#include <Windows.h>
#include "GameMessageType.h"
#include "GameIDDefine.h"



//���b�Z�[�W�N���X
class GameMessage
{
public:
	GameMessage();	//�f�t�H���g�̒l��ݒ肷��
	GameMessage(	//�����̒l���Z�b�g
		ENTITY_ID	from,
		ENTITY_ID	to,
		MSG_TYPE	type,
		void*		exinfo=nullptr);
public:
	ENTITY_ID	from;		//���̃��b�Z�[�W�𑗂����҂�ID
	ENTITY_ID	to;			//���̃��b�Z�[�W���󂯎��҂�ID
	MSG_TYPE	type;		//���̃��b�Z�[�W�̃^�C�v
	void*		exinfo;		//�ǉ����
};

typedef GameMessage *LPGameMessage;
typedef LPGameMessage LPGAMEMESSAGE;

//ID�x�[�X�N���X
class GameBaseEntity
{
private:
	const ENTITY_ID m_id;		//���̃G���e�B�e�B�̔ԍ�
public:
	GameBaseEntity(ENTITY_ID id);	//ID��ݒ�Ɠ�����ID�}�l�[�W���[�ɓo�^
	virtual~GameBaseEntity();		//ID�}�l�[�W�����玩�M���폜����
public:
	inline	ENTITY_ID GetId(){ return m_id; }	
	virtual	bool HandleMessage(LPGameMessage msg) = 0;	//���b�Z�[�W�󂯎��֐�
};

typedef GameBaseEntity *LPGameBaseEntity;


//���b�Z�[�W��M���s�G���[�}�N��
#define MSG_RECEIVE_ERROR(msg)\
{\
	char t[1000]; \
	sprintf_s<1000>(t,\
"���b�Z�[�W��M���s\n �֐���=%s \n type=%d \n from=%d \n to=%d \n exinfo=%d",\
	__FUNCTION__, (int)msg->type, (int)msg->from, (int)msg->to, (int)msg->exinfo\
);\
}

#endif