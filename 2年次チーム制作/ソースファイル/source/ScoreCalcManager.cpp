#include "ScoreCalcManager.h"
#include "LuaLoadParam.h"
#include "GameMatchInfomation.h"
#include "ColaGameGauge.h"
#include "map\MapObject.h"

ScoreCalcManager::ScoreCalcManager():
GameBaseEntity(game_id::id_score_calc_manager),
m_pYen_render_tex(new iex2DObj("DATA\\TEXTURE\\Number.png")),
m_pColaShareGauge(new ColaShareGauge()),
m_StartCountDown(new iex2DObj("DATA\\TEXTURE\\Start_End.png")),
m_isInitScoreTagMgr(false),
m_GameEnd(false),
m_isCountStart(false)
{
	LPGAMESYSTEMCONSTPARAMETERS gp = GetConstGameParam();

	m_ScoreRate.death_rate = gp->ScoreRates.death_score_rate;
	m_ScoreRate.kill_rate = gp->ScoreRates.kill_score_rate;
	m_ScoreRate.share_change_rate = gp->ScoreRates.share_change_score;
	m_ScoreRate.share_rate = gp->ScoreRates.share_score;

	m_share_score_add_frame = (int)gp->ScoreRates.share_score_add_frame;
	m_share_score_add_count = 0;
}

ScoreCalcManager::~ScoreCalcManager()
{
	for (TeamScoreData::iterator it = m_team_data.begin();
		it != m_team_data.end();
		++it)
	{
		delete it->second.pYenRender;
	}
	delete m_pYen_render_tex;
	delete m_pColaShareGauge;
}

bool ScoreCalcManager::ScoreRankPred(const TeamScore* s1, const TeamScore* s2)
{
	return s1->Score > s2->Score;
}

void ScoreCalcManager::AddShareScore()
{
	for (TeamScoreData::iterator it = m_team_data.begin();
		it != m_team_data.end();
		++it)
	{
		TeamScoreData::value_type& data = *it;
		AddTeamScore(static_cast<int>(m_ScoreRate.share_rate*data.second.ShareRate), data.second);
	}
}

void ScoreCalcManager::AddTeamScore(int Score, TeamScore& team)
{
	team.Score += Score;
	if (team.Score < 0)team.Score = 0;
	team.pYenRender->SetValue(team.Score);
}

void ScoreCalcManager::GetTeamScore(TEAM_TYPE type, TeamScore** ppOut)
{
	TeamScoreData::iterator it = m_team_data.find(type);
	MyAssert(it != m_team_data.end(), "作成されていないチームデータの探索が行われました　チーム番号 %d", (int)type);
	*ppOut = &it->second;
}

void ScoreCalcManager::GameEnd()
{
	m_GameEnd = true;
	m_ResultUI.Start();
}

void ScoreCalcManager::StartCountStart()
{
	m_isCountStart = true;
}

bool ScoreCalcManager::isFinishResultUI()const
{
	return m_ResultUI.isEnd();
}

void ScoreCalcManager::SetShareUpEffect(TEAM_TYPE type)
{
	m_pColaShareGauge->SetShareUpEffect(type);
}

bool ScoreCalcManager::GetTeamGaugePos(TEAM_TYPE type, Vector2& out)
{
	return m_pColaShareGauge->GetTeamGaugePos(type, out);
}

void ScoreCalcManager::GetNo1Team(TeamScoreData::const_iterator* pOutIt)
{
	MyAssert(!m_team_data.empty(), "チームスコア作成がされていない状態で１位のチームが要求されました");

	TeamScoreData::iterator it = m_team_data.begin();

	*pOutIt = it;
	++it;

	while (it != m_team_data.end())
	{
		if ((*pOutIt)->second.ShareRate < it->second.ShareRate)
		{
			(*pOutIt) = it;
		}
		++it;
	}
}

void ScoreCalcManager::GetRanking(
	std::list<const TeamScore*>& out_ranking_list)
{
	for (TeamScoreData::iterator it = m_team_data.begin();
		it != m_team_data.end();
		++it)
	{
		out_ranking_list.push_back(&it->second);
	}

	out_ranking_list.sort(ScoreRankPred);
}

void ScoreCalcManager::SetShareRatio(TEAM_TYPE type, RATIO val)
{
	TeamScore* ts;
	GetTeamScore(type, &ts);

	ts->ShareRate = val;
}

void ScoreCalcManager::CreateTeamData(TEAM_TYPE type)
{
	if (m_team_data.count(type))return;

	TeamScore ts;
	ColaShareGauge::TeamUIParam gauge;

	ts.pYenRender = new YenRender(m_pYen_render_tex, false);
	ts.pYenRender->SetChangeSpeed(5);
	ts.Score = 0;
	ts.ShareRate = 0;
	ts.team_type = type;

	m_team_data.insert(TeamScoreData::value_type(type, ts));

	gauge.m_Color = CODMgr::GetTeamColor(type);
	gauge.m_NowRenderRatio = 0;
	gauge.m_NowShareRatio = 0;
	gauge.m_pLogoTex = GAMEMATCHINFO.GetTextureMgr().GetLogoTexturePtr(type);
	gauge.m_TargetShareRatio = 0;
	gauge.m_Type = type;
	gauge.m_ShareUpAlpha = 0;

	m_pColaShareGauge->CreateTeamData(gauge);
}

void ScoreCalcManager::CreatePlayerData(TEAM_TYPE type, PLAYER_NUM num)
{
	TeamScore& ts = m_team_data.at(type);

	if (ts.player_data.count(num))return;

	ts.player_data.insert(
		PlayerData::value_type(num, { num, 0, 0, 0, 0 })
		);
}

void ScoreCalcManager::AddKillScore(TEAM_TYPE type, PLAYER_NUM num)
{
	TeamScore* ts;
	GetTeamScore(type, &ts);
	AddTeamScore(static_cast<int>(m_ScoreRate.kill_rate), *ts);
	ts->player_data.at(num).kill_count += 1;
}

void ScoreCalcManager::AddDeathScore(TEAM_TYPE type, PLAYER_NUM num)
{
	TeamScore* ts;
	GetTeamScore(type, &ts);
	AddTeamScore(static_cast<int>(m_ScoreRate.death_rate), *ts);
	ts->player_data.at(num).death_count += 1;
}

void ScoreCalcManager::AddChangeShareObjScore(TEAM_TYPE type, PLAYER_NUM num)
{
	TeamScore* ts;
	GetTeamScore(type, &ts);
	AddTeamScore(static_cast<int>(m_ScoreRate.share_change_rate), *ts);
	ts->player_data.at(num).change_share_obj_count += 1;
}


void ScoreCalcManager::Update()
{
	if (m_StartCountDown.isStart())
	{
		GAMEMATCHINFO.GameStart();
	}

	if (m_isInitScoreTagMgr == false)
	{
		m_ScoreTagMgr.CreateTeamData(this);
	}

	for (TeamScoreData::iterator it = m_team_data.begin();
		it != m_team_data.end();
		++it)
	{
		it->second.pYenRender->Update();
	}

	if (++m_share_score_add_count > m_share_score_add_frame)
	{
		m_share_score_add_count = 0;
		AddShareScore();
	}
	MapObjectManager::SHAREPARAM_ARRAY share_array;
	MAP_MANAGER.Get_ShareValue(share_array);

	m_pColaShareGauge->UpdateAllTeamData(share_array);
	m_pColaShareGauge->Update();
	
	m_ScoreTagMgr.Update(this);

	if (m_isCountStart)
	{
		m_StartCountDown.Update();
	}

	if (m_GameEnd)
	{
		m_ResultUI.Update(m_pColaShareGauge);
	}
}

void ScoreCalcManager::Render(TEAM_TYPE type)
{
	TeamScore* ts;
	GetTeamScore(type, &ts);

	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	const int 
		x = 1260/w,
		y = 10/h,
		sizeX = 20 / w,
		sizeY = 40 / h,
		space = (sizeX + 10) / w;

	TeamScoreData::const_iterator	No1Team;

	GetNo1Team(&No1Team);

	if (m_GameEnd)
	{
		m_ResultUI.Render(
			m_pColaShareGauge,
			type,
			No1Team->first
			);
		return;
	}

	//ts->pYenRender->Render(x, y, sizeX, sizeY, space);

	m_pColaShareGauge->Render(type, No1Team->first);

	m_ScoreTagMgr.Render();
	m_StartCountDown.Render();
}

bool ScoreCalcManager::HandleMessage(LPGameMessage msg)
{
	if (msg->type == msg_t::add_score_msg)
	{
		AddScoreMsgData* s = (AddScoreMsgData*)msg->exinfo;
		switch (s->type)
		{
		case AddScoreMsgData::_ENEMY_KILL:
			AddKillScore(s->plData->team_type, s->plData->player_num);
			return true;

		case AddScoreMsgData::_DEATH:
			AddDeathScore(s->plData->team_type, s->plData->player_num);
			return true;

		case AddScoreMsgData::_CHANGE_SHARE_OBJ:
			AddChangeShareObjScore(s->plData->team_type, s->plData->player_num);
			return true;

		default:break;
		}
	}
	else if (msg->type == msg_t::render_score_msg)
	{
		Render(*(TEAM_TYPE*)msg->exinfo);
	}

	return false;
}