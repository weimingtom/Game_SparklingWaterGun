#ifndef __CHARACTER_BRAIN_REAL_PLAYER_H__
#define __CHARACTER_BRAIN_REAL_PLAYER_H__

#include "CharacterBrain.h"

//**************************************************************
//		プレイヤの操作する場合のキャラクタの脳クラス
//**************************************************************
class CharacterBrainRealPlayer :public CharacterBrainBase
{
	ControllerButtonSet m_MyControllerButtonSet;	//ボタン配置情報
public:
	CharacterBrainRealPlayer(LPCONTROLLERBUTTONSET pMyControllerButtonSet);	
	~CharacterBrainRealPlayer();
private:
	void UpdateKeyMsgIcon(LPCHARACTER p);
public:
	void Update(LPCHARACTER p);	//キャラクタを更新
	bool HandleMessage(LPGAMEMESSAGE msg);	//メッセージ
};

#endif