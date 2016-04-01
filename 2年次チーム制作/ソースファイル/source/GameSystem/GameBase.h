#ifndef __GAME_BASE_H__
#define __GAME_BASE_H__

#include <Windows.h>
#include "GameMessageType.h"
#include "GameIDDefine.h"



//メッセージクラス
class GameMessage
{
public:
	GameMessage();	//デフォルトの値を設定する
	GameMessage(	//引数の値をセット
		ENTITY_ID	from,
		ENTITY_ID	to,
		MSG_TYPE	type,
		void*		exinfo=nullptr);
public:
	ENTITY_ID	from;		//このメッセージを送った者のID
	ENTITY_ID	to;			//このメッセージを受け取る者のID
	MSG_TYPE	type;		//このメッセージのタイプ
	void*		exinfo;		//追加情報
};

typedef GameMessage *LPGameMessage;
typedef LPGameMessage LPGAMEMESSAGE;

//IDベースクラス
class GameBaseEntity
{
private:
	const ENTITY_ID m_id;		//このエンティティの番号
public:
	GameBaseEntity(ENTITY_ID id);	//IDを設定と同時にIDマネージャーに登録
	virtual~GameBaseEntity();		//IDマネージャから自信を削除する
public:
	inline	ENTITY_ID GetId(){ return m_id; }	
	virtual	bool HandleMessage(LPGameMessage msg) = 0;	//メッセージ受け取り関数
};

typedef GameBaseEntity *LPGameBaseEntity;


//メッセージ受信失敗エラーマクロ
#define MSG_RECEIVE_ERROR(msg)\
{\
	char t[1000]; \
	sprintf_s<1000>(t,\
"メッセージ受信失敗\n 関数名=%s \n type=%d \n from=%d \n to=%d \n exinfo=%d",\
	__FUNCTION__, (int)msg->type, (int)msg->from, (int)msg->to, (int)msg->exinfo\
);\
}

#endif