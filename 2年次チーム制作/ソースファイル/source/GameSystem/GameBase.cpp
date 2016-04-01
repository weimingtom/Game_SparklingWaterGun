#include "../All.h"

//********************************
//���b�Z�[�W�N���X
//********************************

//�f�t�H���g�̒l��ݒ肷��
GameMessage::GameMessage()
{
	from =	game_id::id_error;
	to =	game_id::id_error;
	type =	msg_t::error_msg;
	exinfo = nullptr;
}

GameMessage::GameMessage(
	ENTITY_ID	from,
	ENTITY_ID	to,
	MSG_TYPE	type,
	void*		exinfo):
	from(from),
	to(to),
	type(type),
	exinfo(exinfo)
{

}


//ID�x�[�X�N���X


//ID��ݒ�Ɠ�����ID�}�l�[�W���[�ɓo�^
GameBaseEntity::GameBaseEntity(ENTITY_ID id):
m_id(id)
{
	MyAssert(IDMANAGER.Register(this), "ID�o�^�Ɏ��s���܂��� \n ID�ԍ�= %d ", m_id);
}

//ID�}�l�[�W�����玩�M���폜����
GameBaseEntity::~GameBaseEntity()
{
	MyAssert(IDMANAGER.Erace(m_id), "ID�폜�Ɏ��s���܂��� \n ID�ԍ�= %d ", m_id);
}