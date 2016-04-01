
#ifndef __SCORE_CAL_MANAGER_H__
#define __SCORE_CAL_MANAGER_H__

#include "iextreme.h"
#include "CODBOForwardDecl.h"
#include "YenRender.h"
#include "GameSystem\GameBase.h"
#include <map>
#include <list>
#include "ScoreTag.h"
#include "ResultUI.h"
#include "StartCountDown.h"

struct AddScoreMsgData
{
	enum SCORE_TYPE
	{
		_ENEMY_KILL,
		_DEATH,
		_CHANGE_SHARE_OBJ,
	};
	SCORE_TYPE			type;
	LPPLAYERMATCHDATA	plData;
};

class ScoreCalcManager:public GameBaseEntity
{
private:
	typedef int PLAYER_NUM;

	struct PlayerScore
	{
		PLAYER_NUM player_num;
		int total_score;
		int kill_count;
		int death_count;
		int change_share_obj_count;
	};
	typedef std::map<PLAYER_NUM, PlayerScore> PlayerData;
public:
	struct TeamScore
	{
		TEAM_TYPE	team_type;
		PlayerData	player_data;
		YenRender*	pYenRender;
		RATIO		ShareRate;
		int			Score;
	};
	typedef std::map<TEAM_TYPE, TeamScore> TeamScoreData;
public:
	struct ScoreRate
	{
		float kill_rate;
		float death_rate;
		float share_rate;
		float share_change_rate;
	};
private:
	TeamScoreData			m_team_data;
	ScoreRate				m_ScoreRate;
	int						m_share_score_add_frame;
	int						m_share_score_add_count;
	LPIEX2DOBJ	const		m_pYen_render_tex;
	LPCOLASHAREGAUGE const	m_pColaShareGauge;
	ScoreTagManager			m_ScoreTagMgr;
	StartCountDown			m_StartCountDown;
	ResultUI				m_ResultUI;
	bool					m_isInitScoreTagMgr;
	bool					m_isCountStart;
	bool					m_GameEnd;
public:
	ScoreCalcManager();
	~ScoreCalcManager();
private:
	static bool ScoreRankPred(const TeamScore* s1, const TeamScore* s2);
	void AddShareScore();
	void GetTeamScore(TEAM_TYPE type, TeamScore** ppOut);
	void AddTeamScore(int Score, TeamScore& team);
public:
	void GameEnd();
	void StartCountStart();
	bool isFinishResultUI()const;

	void SetShareUpEffect(TEAM_TYPE type);
	bool GetTeamGaugePos(TEAM_TYPE type, Vector2& out);

	void GetNo1Team(TeamScoreData::const_iterator* pOutIt);
	void GetRanking(std::list<const TeamScore*>& out_ranking_list);

	void SetShareRatio(TEAM_TYPE type,RATIO val);
	void CreateTeamData(TEAM_TYPE type);
	void CreatePlayerData(TEAM_TYPE type, PLAYER_NUM num);

	void AddKillScore(TEAM_TYPE type, PLAYER_NUM num);
	void AddDeathScore(TEAM_TYPE type, PLAYER_NUM num);
	void AddChangeShareObjScore(TEAM_TYPE type, PLAYER_NUM num);

	void Update();
	void Render(TEAM_TYPE type);

	bool HandleMessage(LPGameMessage msg);
};

#endif