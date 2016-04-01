#include "../All.h"

//********************************
//メッセージクラス
//********************************

//デフォルトの値を設定する
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


//IDベースクラス


//IDを設定と同時にIDマネージャーに登録
GameBaseEntity::GameBaseEntity(ENTITY_ID id):
m_id(id)
{
	MyAssert(IDMANAGER.Register(this), "ID登録に失敗しました \n ID番号= %d ", m_id);
}

//IDマネージャから自信を削除する
GameBaseEntity::~GameBaseEntity()
{
	MyAssert(IDMANAGER.Erace(m_id), "ID削除に失敗しました \n ID番号= %d ", m_id);
}