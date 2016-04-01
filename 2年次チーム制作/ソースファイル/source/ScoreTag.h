#ifndef __SCORE_TAG_H__
#define __SCORE_TAG_H__

#include "iextreme.h"
#include "YenRender.h"
#include <list>
#include "CODBOForwardDecl.h"

class ScoreCalcManager;


class ScoreTag
{
public:
	enum STATE
	{
		_NOTMOVE,
		_UPMOVE,
		_DOWNMOVE,
	};
	enum RANK
	{
		_ERR_VALUE
	};
	struct RenderData
	{
		YenRender*	pYenRender;
		LPIEX2DOBJ	pTagTexture;
		LPIEX2DOBJ	pLogoTexture;
		COLOR		color;
	};
	struct
	{
		Vector2		pos;
		Vector2		RotPos;
		float		first_target_len;
	}
	m_MoveTarget;
	Vector2			m_Pos;
	RADIAN			m_Count;
	STATE			m_State;
	YenRender*		m_pYenRender;
	const RATIO		m_MoveSpeed;
	int				m_Rank;
	const TEAM_TYPE	m_team;
public:
	ScoreTag(TEAM_TYPE		team);
	~ScoreTag();
public:
	void SetPos(CRVector2 pos);
	void SetRank(int rank);
	STATE GetState()const;
	void SetMove(STATE nextState, CRVector2 target);
	void Update();
	void Render(RenderData& rp);
};



class ScoreTagManager
{
private:
	typedef std::list<ScoreTag> ScoreTagList;
	enum STATE
	{
		_FREE,
		_MOVE,
	};
private:
	ScoreTagList				m_ScoreTagList;
	STATE						m_State;
	int							m_Count;
public:
	ScoreTagManager();
	~ScoreTagManager();
private:
	Vector2 GetRankPos(int rank);
	void InitRankPos(ScoreTagList& out);
	void GetScoreTagList(ScoreTagList& outList, ScoreCalcManager* pScoreMgr);
	void UpdateRanking(ScoreCalcManager* pScoreMgr);
	void MoveRankingPos();
public:
	void CreateTeamData(ScoreCalcManager* pScoreMgr);
	void Update(ScoreCalcManager* pScoreMgr);
	void Render();

};
#endif