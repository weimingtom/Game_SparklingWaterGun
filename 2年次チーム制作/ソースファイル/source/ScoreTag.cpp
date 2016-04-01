#include "ScoreTag.h"
#include "GameMatchInfomation.h"
#include "ScoreCalcManager.h"
#include "DebugParticle.h"

ScoreTag::ScoreTag(TEAM_TYPE		team) :
m_Pos(0, 0),
m_Count(0),
m_State(_NOTMOVE),
m_MoveSpeed(0.2f),
m_team(team)
{

}

ScoreTag::~ScoreTag()
{

}

ScoreTag::STATE ScoreTag::GetState()const
{
	return m_State;
}

void ScoreTag::SetMove(STATE nextState, CRVector2 target)
{
	m_State = nextState;

	m_MoveTarget.pos = target;
	m_MoveTarget.RotPos = m_Pos + (target - m_Pos) / 2;
	m_MoveTarget.first_target_len = Vector2Length(target - m_Pos);

	m_Count = PI;

}

void ScoreTag::Update()
{
	switch (m_State)
	{
	case _NOTMOVE:

		m_Pos = m_MoveTarget.pos;
		break;
	case _UPMOVE:

		if (m_Count -= m_MoveSpeed < 0)
		{
			m_Count = 0;
			m_State = _NOTMOVE;
		}
		m_Pos.x = m_MoveTarget.RotPos.x + sinf(m_Count)*m_MoveTarget.first_target_len;
		m_Pos.y = m_MoveTarget.RotPos.y + cosf(m_Count)*m_MoveTarget.first_target_len;
		break;
	case _DOWNMOVE:

		m_Pos += (m_MoveTarget.pos - m_Pos)*m_MoveSpeed;
		if (Vector2Length(m_MoveTarget.pos - m_Pos) < 2)
		{
			m_State = _NOTMOVE;
		}
		break;
	default: 
		assert(0 && "ScoreTag::Update default ‚±‚±‚É‚Í‚±‚È‚¢‚Í‚¸‚È‚ñ‚¾‚¯‚Çc");
	}
}

void ScoreTag::Render(RenderData& rp)
{
	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	const RECTI tagRc = { (int)m_Pos.x / w, (int)m_Pos.y / h, 300 / w, 100 / h };
	const RECTI logoRc = { tagRc.x + 100 / w, tagRc.y + 50 / h, 200 / w, 100 / h };
	const RECTI yenRc = { tagRc.x + 90 / w, tagRc.y + 50 / h, 50 / w, 100 / h };

	rp.pTagTexture->Render(
		tagRc.x,
		tagRc.y,
		tagRc.width,
		tagRc.height,
		0,
		0,
		512,
		512,
		RS_COPY,
		rp.color
		);

	rp.pLogoTexture->Render(
		logoRc.x,
		logoRc.y,
		logoRc.width,
		logoRc.height,
		0,
		0,
		512,
		512,
		RS_COPY
		);

	rp.pYenRender->Render(
		yenRc.x,
		yenRc.y,
		yenRc.width,
		yenRc.height,
		yenRc.width + 4 / w
		);
}

void ScoreTag::SetPos(CRVector2 pos)
{
	m_Pos = pos;
}

void ScoreTag::SetRank(int rank)
{
	m_Rank = rank;
}


//************************************************************************
//
//************************************************************************

ScoreTagManager::ScoreTagManager()
{
	m_State = _FREE;
	m_Count = 0;
}

ScoreTagManager::~ScoreTagManager()
{

}

Vector2 ScoreTagManager::GetRankPos(int rank)
{
	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	Vector2 pos(300 / (float)w, 400 / (float)h);
	const float spaceY = 100 / (float)h;

	pos.y += spaceY*rank;
	return pos;
}

void ScoreTagManager::InitRankPos(ScoreTagList& out)
{
	int rank_count = 0;

	for (ScoreTagList::iterator it = out.begin();
		it != out.end();
		++it)
	{
		it->SetPos(GetRankPos(rank_count));
		++rank_count;
	}
}

void ScoreTagManager::GetScoreTagList(
	ScoreTagList&		outList,
	ScoreCalcManager*	pScoreMgr)
{
	typedef std::list<const ScoreCalcManager::TeamScore*> TeamDataList;
	TeamDataList tList;
	const ScoreCalcManager::TeamScore* pIt;
	int rank_count = 0;

	pScoreMgr->GetRanking(tList);
	outList.clear();

	for (TeamDataList::iterator it = tList.begin();
		it != tList.end();
		++it)
	{
		pIt = *it;
		ScoreTag tag(pIt->team_type);
		tag.SetRank(rank_count);
		tag.m_pYenRender = pIt->pYenRender;
		outList.push_back(tag);
		++rank_count;
	}
}

void ScoreTagManager::CreateTeamData(ScoreCalcManager* pScoreMgr)
{
	GetScoreTagList(m_ScoreTagList, pScoreMgr);
	InitRankPos(m_ScoreTagList);
}

void ScoreTagManager::Update(ScoreCalcManager* pScoreMgr)
{

	if (m_State == _FREE)
	{
		UpdateRanking(pScoreMgr);
	}
	if (m_State == _MOVE)
	{
		m_Count++;
		if (m_Count == 20)
		{
			MoveRankingPos();
		}
		if (m_Count >= 60)
		{
			m_State = _FREE;
		}
	}

	for (ScoreTagList::iterator it = m_ScoreTagList.begin();
		it != m_ScoreTagList.end();
		++it)
	{
		it->Update();
	}
}

void ScoreTagManager::UpdateRanking(ScoreCalcManager* pScoreMgr)
{

	ScoreTagList			scList;
	GetScoreTagList(scList, pScoreMgr);

	ScoreTagList::iterator it = m_ScoreTagList.begin();
	ScoreTagList::iterator it2 = scList.begin();
	ScoreTagList::iterator searchIt;

	ScoreTag* pIt, *pIt2;

	while (it != m_ScoreTagList.end())
	{
		pIt = &*it;
		pIt2 = &*it2;

		if (pIt->m_team != pIt2->m_team)
		{
			while (it->m_team != pIt2->m_team)
			{
				++it;
			}
			it->SetMove(ScoreTag::_UPMOVE, GetRankPos(pIt2->m_Rank));
			return;
		}
		++it;
		++it2;
	}
}

void ScoreTagManager::MoveRankingPos()
{
	int rank_count = 0;

	for (ScoreTagList::iterator it = m_ScoreTagList.begin();
		it != m_ScoreTagList.end();
		++it)
	{
		if (it->m_State == ScoreTag::_NOTMOVE)
		{
			it->SetMove(ScoreTag::_DOWNMOVE, GetRankPos(rank_count));
		}
		++rank_count;
	}
}

void ScoreTagManager::Render()
{
	return;

	ScoreTag::RenderData rd;

	rd.color = 0xFFFFFFFF;
	rd.pLogoTexture = GAMEMATCHINFO.GetTextureMgr().GetADTexturePtr(0);
	rd.pTagTexture = GAMEMATCHINFO.GetTextureMgr().GetADTexturePtr(0);

	for (ScoreTagList::iterator it = m_ScoreTagList.begin();
		it != m_ScoreTagList.end();
		++it)
	{
		rd.pYenRender = it->m_pYenRender;
		it->Render(rd);
	}
}