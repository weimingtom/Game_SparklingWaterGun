#ifndef __COD_BO_DEBUG_MATCH_STATE__
#define __COD_BO_DEBUG_MATCH_STATE__

#include "CODBO.h"
#include "CODBOForwardDecl.h"

//********************************************************
//	デバッグ(開発)用マッチステート
//********************************************************
class DebugMatchState:public MatchTypeStateBase
{
private:
	AimTargetManager*					m_pAimTargetManager;//エイム目標マネージャ
	ScreenSeparation*					m_pScreenSeparation;//画面分割管理クラス
	std::list<RECTI>					m_ScreenPosList;	//分割した画面の配置場所
	Camera*								m_pCamera;			//とりあえずカメラ
	ControllerManager*					m_pControllerMgr;	//コントローラマネージャー
	BulletManager*						m_pBulletManager;	//弾マネージャ
	DamageObjectManager*				m_pDamageObjectManager;	//ダメージマネージャ
	LPWATERPOOLMANAGER					m_pColaWaterMgr;	//水たまりマネージャ

public:
	DebugMatchState();
	~DebugMatchState();
public:
	void GameRender(LPIEXVIEW pView, ControllObject*pOwner);
public:
	void GameStart();
	bool isGameFinish();
public:
	void Initialize();
	void Update();
	void Render();

	//test
	void	Water_Render(LPIEXVIEW pView, ControllObject	*pOwner);
};


#endif