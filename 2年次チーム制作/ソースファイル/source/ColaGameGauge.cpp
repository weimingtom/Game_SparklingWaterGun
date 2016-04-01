#include "ColaGameGauge.h"
#include "GameMatchInfomation.h"

ColaShareGauge::ColaShareGauge() :
m_ChangeSpeed(0.05f),
m_AnimationCount(0),
m_NowState(USUAL),
m_UsualGaugeSize(880,45),
m_UsualGaugePos(640,70)
{
	char *file_names[_TEXMAX] =
	{
		"DATA\\TEXTURE\\UI\\UI_Gauge.png",
		"DATA\\TEXTURE\\Animation\\explode.png",
		"DATA\\TEXTURE\\UI\\pet.png",
		"DATA\\TEXTURE\\Number.png",
		"DATA\\TEXTURE\\UI\\your_team.png"
	};
	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	for (int i = 0; i < (int)_TEXMAX; ++i)
	{
		m_pTextures[i] = new iex2DObj(file_names[i]);
	}

	for (int i = 0; i < m_TeamMax; ++i)
	{
		m_GaugeCenters[i].x = 0;
		m_GaugeCenters[i].y = 60;
	}

	m_GameFirst = true;

	m_GaugeSize.x = (float)((int)m_UsualGaugeSize.x / w);
	m_GaugeSize.y = (float)((int)m_UsualGaugeSize.y / h);
	m_GaugePos.x = (float)((int)m_UsualGaugePos.x / w);
	m_GaugePos.y = (float)((int)m_UsualGaugePos.y / h);

}


ColaShareGauge::~ColaShareGauge()
{
	for (int i = 0; i < (int)_TEXMAX; ++i)
	{
		delete m_pTextures[i];
	}
}

void ColaShareGauge::GaugeMove()
{
	Vector2 Pos, Size;
	int w, h;
	const float speed = 2.5f;

	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	if (m_NowState == USUAL)
	{
		Pos = m_UsualGaugePos;
		Size = m_UsualGaugeSize;
	}
	else if (m_NowState == RESULT)
	{
		Pos.x = 640;
		Pos.y = 250;
		Size.x = m_UsualGaugeSize.x*1.2f;
		Size.y = m_UsualGaugeSize.y*2.0f;
	}

	Pos.x /= (float)w;
	Pos.y /= (float)h;

	Size.x /= (float)w;
	Size.y /= (float)h;

	Vector2 v;

	v = Pos - m_GaugePos;
	if (Vector2Length(v) > speed)
	{
		v = Vector2Normalize(v);
		v *= speed;
	}

	m_GaugePos += v;

	v = Size - m_GaugeSize;
	if (Vector2Length(v) > speed)
	{
		v = Vector2Normalize(v);
		v *= speed;
	}

	m_GaugeSize += v;

}

void ColaShareGauge::ParcentRender(
	RATIO value,
	int x,
	int y,
	int sizeX,
	int sizeY)
{
	const int NumberTexSize = 64;
	int NumValue = static_cast<int>(((value + 0.005f) * 100));
	int number;

	do
	{
		number = NumValue % 10;
		
		m_pTextures[PARCENT_NUMBER]->Render(
			x,
			y,
			sizeX,
			sizeY,

			NumberTexSize*number, 
			0,

			NumberTexSize,
			NumberTexSize
			);

		NumValue /= 10;
		x -= sizeX;

	} while (NumValue > 0);

}

void ColaShareGauge::SetRenderRatio()
{
	const RATIO min_value = 0.12f;
	TeamUIParamPtrMap NotMinTeam;
	TeamUIParamMap::iterator it;
	float total = 0;
	float last_gauge = 1;

	for (it = m_DataMap.begin();
		it != m_DataMap.end();
		++it)
	{
		TeamUIParam& p = it->second;

		p.m_NowRenderRatio = p.m_NowShareRatio;

		if (p.m_NowRenderRatio <= min_value)
		{
			p.m_NowRenderRatio = min_value;
			last_gauge -= min_value;
		}else{
			total += p.m_NowRenderRatio;
			NotMinTeam.insert(
				TeamUIParamPtrMap::value_type(it->first, &p));
		}
	}

	if (total <= 0)
	{
		const float val = 1 / (float)m_DataMap.size();

		for (it = m_DataMap.begin();
			it != m_DataMap.end();
			++it)
		{
			TeamUIParam& p = it->second;
			p.m_NowRenderRatio = val;
		}
		return;
	}

	for (TeamUIParamPtrMap::iterator it = NotMinTeam.begin();
		it != NotMinTeam.end();
		++it)
	{
		TeamUIParam* p = it->second;
		p->m_NowRenderRatio = (p->m_NowRenderRatio / total)*last_gauge;
	}
}

void ColaShareGauge::GaugeRender(int w, int h, TEAM_TYPE you, TEAM_TYPE No1Team)
{
	const int gauge_x = (int)(m_GaugePos.x - m_GaugeSize.x / 2), gauge_y = (int)(m_GaugePos.y - m_GaugeSize.y / 2);
	const int gauge_x_size = (int)m_GaugeSize.x, gauge_y_size = (int)m_GaugeSize.y;
	const int default_logo_size_x = 120 / w, default_logo_size_y = 60 / h;
	const int logo_y = gauge_y + gauge_y_size / 2;
	const int cursor_size_x = 50 / w, cursor_size_y = 50 / h;
	const int cursor_y = logo_y + (50 / h);
	const int parcent_number_size_x = 20, parcent_number_size_y = 40;

	int logo_size_x, logo_size_y;
	int logo_x;
	int x_count = 0;
	int render_gauge_len = 0;

	const float upValue = sinf(m_AnimationCount*0.2f)*0.5f + 0.5f;

	COLORf color;

	m_pTextures[GAUGE_BAR]->Render(
		gauge_x,
		gauge_y,
		gauge_x_size,
		gauge_y_size,

		0,
		0,
		1535,
		128,

		RS_COPY
		);

	for (TeamUIParamMap::iterator it = m_DataMap.begin();
		it != m_DataMap.end();
		++it, x_count += render_gauge_len)
	{
		TeamUIParam& p = it->second;

		render_gauge_len = (int)(p.m_NowRenderRatio*(float)gauge_x_size);

		logo_x = gauge_x + x_count + (render_gauge_len / 2);

		logo_size_x = default_logo_size_x;
		logo_size_y = default_logo_size_y;

		m_GaugeCenters[p.m_Type].x = logo_x*(float)w;
		m_GaugeCenters[p.m_Type].y = 60;

		if (p.m_Type == you)
		{
			m_pTextures[CURSOR]->Render(
				logo_x - (cursor_size_x/2),
				cursor_y - (cursor_size_y / 2),
				cursor_size_x,
				cursor_size_y,

				0,
				0,
				256,
				256
				);
		}

		if (p.m_ShareUpAlpha > 0.1f)
		{
			logo_size_x += (int)(upValue * 25 * p.m_ShareUpAlpha) / w;
			logo_size_y += (int)(upValue * 25 * p.m_ShareUpAlpha) / h;
		}

		if (m_NowState == RESULT&&p.m_Type == No1Team)
		{
			p.m_ShareUpAlpha = 1;
			logo_size_x += (int)(upValue * 25 * p.m_ShareUpAlpha) / w;
			logo_size_y += (int)(upValue * 25 * p.m_ShareUpAlpha) / h;
		}

		m_pTextures[GAUGE_BAR]->Render(
			gauge_x + x_count,
			gauge_y,
			render_gauge_len,
			gauge_y_size,

			(int)(((float)x_count / (float)gauge_x_size) * 1535), 
			128,
			(int)(1535 * p.m_NowRenderRatio),
			128,

			RS_COPY,
			p.m_Color
			);

		color.SetColor(p.m_Color);
		color.a = p.m_ShareUpAlpha;

		m_pTextures[GAUGE_BAR]->Render(
			gauge_x + x_count,
			gauge_y,
			render_gauge_len,
			gauge_y_size,

			(int)(((float)x_count / (float)gauge_x_size) * 1535),
			128,
			(int)(1535 * p.m_NowRenderRatio),
			128,

			RS_ADD,
			color.toDWORD()
			);

		p.m_pLogoTex->Render(
			logo_x - (logo_size_x / 2),
			logo_y - (logo_size_y / 2), 
			logo_size_x,
			logo_size_y,

			0,
			0,
			1024,
			512
			);

		/*
		ParcentRender(
			p.m_NowShareRatio,
			logo_x - parcent_number_size_x,
			logo_y + logo_size_y / 2,
			parcent_number_size_x,
			parcent_number_size_y
			);
			*/
	}
}

void ColaShareGauge::UpdateAllTeamData(MapObjectManager::SHAREPARAM_ARRAY& array_share)
{
	for (TeamUIParamMap::iterator it = m_DataMap.begin();
		it != m_DataMap.end();
		++it)
	{
		it->second.m_Update = false;
	}

	for (MapObjectManager::SHAREPARAM_ARRAY::iterator it = array_share.begin();
		it != array_share.end();
		++it)
	{
		ShareRatioParam& param = *it;
		UpdateTeamData(param.team_type, param.share_power);
	}

	if (m_GameFirst)return;

	for (TeamUIParamMap::iterator it = m_DataMap.begin();
		it != m_DataMap.end();
		++it)
	{
		TeamUIParam& p = it->second;

		if (p.m_Update == false)
		{
			p.m_TargetShareRatio = 0;
		}
	}
}

void ColaShareGauge::CreateTeamData(const TeamUIParam& param)
{
	if (m_DataMap.count(param.m_Type))return;

	m_DataMap.insert(TeamUIParamMap::value_type(param.m_Type, param));
}

void ColaShareGauge::UpdateTeamData(TEAM_TYPE team, RATIO	ShareRatio)
{
	TeamUIParamMap::iterator it = m_DataMap.find(team);

	MyAssert(it != m_DataMap.end(), "作成されていないチームのシェア率更新が行われました チーム番号 %d ", (int)team);

	if (m_GameFirst&&ShareRatio > 0.1f)
	{
		for (TeamUIParamMap::iterator it = m_DataMap.begin();
			it != m_DataMap.end();
			++it)
		{
			TeamUIParam& p = it->second;
			p.m_NowShareRatio = p.m_TargetShareRatio = 1 / (float)m_DataMap.size();
		}
		m_GameFirst = false;
	}

	it->second.m_TargetShareRatio = ShareRatio;
	it->second.m_Update = true;
}


bool ColaShareGauge::GetTeamGaugePos(TEAM_TYPE team, Vector2& out)
{

	if (team < 0 || team >= m_TeamMax)
	{
		return false;
	}

	out = m_GaugeCenters[team];

	return true;
}

void ColaShareGauge::SetShareUpEffect(TEAM_TYPE team)
{
	TeamUIParamMap::iterator it = m_DataMap.find(team);

	if (it != m_DataMap.end())
		it->second.m_ShareUpAlpha = 1;
}

void ColaShareGauge::Update()
{
	for (TeamUIParamMap::iterator it = m_DataMap.begin();
		it != m_DataMap.end();
		++it)
	{
		TeamUIParam& p = it->second;
		
		p.m_NowShareRatio += (p.m_TargetShareRatio - p.m_NowShareRatio)*m_ChangeSpeed;
		p.m_ShareUpAlpha -= 0.05f;
		p.m_ShareUpAlpha = max(p.m_ShareUpAlpha, 0);
	}

	SetRenderRatio();
	GaugeMove();

	if (++m_AnimationCount >= 10000)m_AnimationCount = 0;

}

void ColaShareGauge::GoResultPos()
{
	m_NowState = RESULT;
}

void ColaShareGauge::Render(TEAM_TYPE you, TEAM_TYPE No1Team)
{
	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	GaugeRender(w, h, you, No1Team);
}


