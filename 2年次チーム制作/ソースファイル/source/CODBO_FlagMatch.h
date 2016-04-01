#ifndef __CODBO_FLAG_MATCH_H__
#define __CODBO_FLAG_MATCH_H__

#include"CODBO.h"
#include "CODBOForwardDecl.h"

//********************************************************
//	フラッグゲーム用マッチステート
//********************************************************
class FlagMatchState :public MatchTypeStateBase
{
private:
	enum MATCHSTATE
	{

	};
private:
	const CODMatchData					m_MatchData;		//プレイヤー
	AimTargetManager*					m_pAimTargetManager;//エイム目標マネージャ
	ScreenSeparation*					m_pScreenSeparation;//画面分割管理クラス
	std::list<RECTI>					m_ScreenPosList;	//分割した画面の配置場所
	Camera*								m_pCamera;			//とりあえずカメラ
	ControllerManager*					m_pControllerMgr;	//コントローラマネージャー
	BulletManager*						m_pBulletManager;	//弾マネージャ
	DamageObjectManager*				m_pDamageObjectManager;	//ダメージマネージャ
public:
	FlagMatchState(const CODMatchData& match_data);
	~FlagMatchState();
private:
	bool CreateCharacters();
public:
	void GameRender(LPIEXVIEW pView, ControllObject*pOwner);
public:
	void GameStart();
	bool isGameFinish();
public:
	void Initialize();
	void Update();
	void Render();
};
#endif