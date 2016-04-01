#ifndef __COLA_BULLET_H__
#define __COLA_BULLET_H__

#include "iextreme.h"
#include "GameSystem\GameBase.h"

//**************************************************************
//	ゲーム上に存在する弾のベースクラス
//**************************************************************
class BulletBase
{
public:
	virtual ~BulletBase(){}
	virtual bool isDelete() = 0;	//消去するかどうか
	virtual void Update() = 0;		//更新
	virtual void Render() = 0;		//描画
};

typedef BulletBase* LPBULLETBASE;

//**************************************************************
//	弾マネージャクラス
//**************************************************************

//弾管理最大数
static const int BULLETMAX = 600;


class BulletManager:public GameBaseEntity
{
private:
	int				m_BulletSetCount;			//弾をセットするカウント
	LPBULLETBASE	m_pBulletArray[BULLETMAX];	//弾のポインタデータ
public:
	BulletManager();
	~BulletManager();
public:
	bool HandleMessage(LPGameMessage msg);		//メッセージ受信
	bool AddBullet(LPBULLETBASE bullet);		//弾をマネージャにセット(newしたポインタを渡すこと)
	void Update();	
	void Render();
};

#endif