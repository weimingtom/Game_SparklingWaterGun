#ifndef __CHARACTER_BRAIN_H__
#define __CHARACTER_BRAIN_H__

#include "Character.h"
#include "GameSystem\GameController.h"

//**************************************************************
//		キャラクタの脳クラスベース
//**************************************************************
class CharacterBrainBase
{
private:
public:
	virtual ~CharacterBrainBase(){}
	virtual void Update(LPCHARACTER p) = 0;				//毎フレームの更新
	virtual bool HandleMessage(LPGAMEMESSAGE msg) = 0;	//メッセージ
};

/*
このクラスを継承してプレイヤー操作クラスとCOM操作クラスがつくられる
*/


#endif // __CHARACTER_BRAIN_H__
