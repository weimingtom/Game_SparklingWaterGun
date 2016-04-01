#ifndef __COD_BO_SHARE_MATCH_H__
#define __COD_BO_SHARE_MATCH_H__

#include "CODBO.h"
#include "CODBOForwardDecl.h"

//********************************************************
//	デバッグ(開発)用マッチステート
//********************************************************
class ShareMatchState :public MatchTypeStateBase
{
private:
	enum SHARE_MATCH_STATE
	{
		_PRE_PLAY,
		_PLAY,
		_FINISH
	};
private:
	AimTargetManager*					m_pAimTargetManager;//エイム目標マネージャ
	ScreenSeparation*					m_pScreenSeparation;//画面分割管理クラス
	std::list<RECTI>					m_ScreenPosList;	//分割した画面の配置場所
	ControllerManager*					m_pControllerMgr;	//コントローラマネージャー
	BulletManager*						m_pBulletManager;	//弾マネージャ
	DamageObjectManager*				m_pDamageObjectManager;	//ダメージマネージャ
	LPWATERPOOLMANAGER					m_pColaWaterMgr;	//水たまりマネージャ
	LPTEAMDATAMGR						m_pTeamDataMgr;		//チームごとのデータ
	LPSCORECALCMGR						m_pScoreCalcMgr;	//スコアカウントクラス
	LPSCRICONMGR						m_pScrIconMgr;		//スクリーン上のアイコンマネージャ
	LPNEWSTELOPMGR						m_pNewsTelopMgr;	//ニューステロップマネージャ
	NewsTelopUpdaterShareMatch*			m_pNewsUpdater;		//ニュース更新クラス
	TimeRender*							m_pTimeRender;		//時間カウントクラス
	SHARE_MATCH_STATE					m_State;			//マッチ状態
	int									m_TimeCount;		//時間カウント
	bool								m_isGameFinish;		//ゲーム終了フラグ
public:
	ShareMatchState();
	~ShareMatchState();
private:
	void SetSoundCameraMatrix();
	void SetNextState(SHARE_MATCH_STATE	next);
	void MatchState_PrePlay();
	void MatchState_Play();
	void MatchState_Finish();
	void SetTeamShareRatio();
	bool CreateCharacter(ENTITY_ID id, PLAYER_TYPE type, int player_num, TEAM_TYPE team);
	bool CreateMovieModeObj(ENTITY_ID id, PLAYER_TYPE type, int player_num, TEAM_TYPE team);
public:
	void GameRender(LPIEXVIEW pView, ControllObject*pOwner);
public:
	void GameStart();
	bool isGameFinish();
	void GetResultScene(Scene**ppOut);
public:
	void Initialize();
	void Update();
	void Render();

	//test
	void	Water_Render(LPIEXVIEW pView, ControllObject	*pOwner);
};


#endif