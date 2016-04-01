#include "ResultUI.h"
#include "ColaGameGauge.h"
#include "GameMatchInfomation.h"
#include "ControllerManager.h"
#include "GameSystem\GameIDManager.h"

ResultUI::ResultUI():
m_Back_alpha(0),
m_Work(0),
m_pGauge(nullptr),
m_Start(false),
m_End(false),
m_pNowUpdateState(nullptr),
m_pNowRenderState(nullptr),
m_StateTimer(0)
{
	m_pTextures[_FINISH] = new iex2DObj("DATA\\TEXTURE\\UI\\ReadyGo.png");
	m_pTextures[_WIN_LOSE] = new iex2DObj("DATA\\TEXTURE\\UI\\VorL.png");
	m_pTextures[_WATER_EFFECT] = new iex2DObj("DATA\\TEXTURE\\UI\\FinishWaterEffect.png");
}

ResultUI::~ResultUI()
{
	for (int i = 0; i < (int)__MAX; ++i)
	{
		delete m_pTextures[i];
	}
}

void ResultUI::PreBlack()
{
	if (m_StateTimer++>150)
	{
		m_StateTimer = 0;
		m_pNowUpdateState = &ResultUI::MoveGauge;
		m_pNowRenderState = &ResultUI::MoveGaugeR;
	}
}

void ResultUI::PreBlackR(
	LPCOLASHAREGAUGE		pGauge,
	TEAM_TYPE				type,
	TEAM_TYPE				no1_team)
{
	const RATIO t = (m_pNowUpdateState == &ResultUI::PreBlack) ?
		(min((float)m_StateTimer / 50.0f, 1)) : (1);

	const RECTI& rc = GAMEMATCHINFO.GetScreenSize();
	RECTI texture;

	texture.width = rc.width / 3;
	texture.height = rc.height / 5;
	texture.x = rc.width / 2 - texture.width / 2;
	texture.y = rc.height / 2 - texture.height / 2;

	texture.y -= (int)((float)(rc.height / 10)*t);
	
	m_pTextures[_FINISH]->Render(
		texture.x,
		texture.y,
		texture.width,
		texture.height,

		0,
		1024,
		1620,
		512,
		RS_COPY,
		D3DCOLOR_COLORVALUE(1, 1, 1, t)
		);
}

static bool select_player(LPCONTROLLER c)
{
	return c->GetMatchData().player_type == player_type::PLAYER;
}

void ResultUI::MoveGauge()
{
	if (m_StateTimer++>250)
	{
		m_StateTimer = 0;
		m_pNowUpdateState = &ResultUI::WinnerApper;
		m_pNowRenderState = &ResultUI::WinnerApperR;

		//終了演出のセット
		CONTROLLERLIST controller_list = 
			((ControllerManager*)IDMANAGER.GetPointer(game_id::id_controller_manager))->GetController(select_player);

		for (auto& it : controller_list)
		{
			if (it->GetControllObjectPtr())
			{
				ControllObjFunc::SetWaterDropParam(it->GetControllObjectPtr(), true);
			}
		}
	}

	m_pGauge->GoResultPos();

	m_Back_alpha += 0.02f;
	m_Back_alpha = min(0.7f, m_Back_alpha);
}

void ResultUI::MoveGaugeR(
	LPCOLASHAREGAUGE		pGauge,
	TEAM_TYPE				type,
	TEAM_TYPE				no1_team)
{
	const RECTI& rc = GAMEMATCHINFO.GetScreenSize();
	COLORf color(m_Back_alpha, 0, 0, 0);

	PreBlackR(pGauge, type, no1_team);

	iexPolygon::Rect(
		rc.x,
		rc.y,
		rc.width,
		rc.height,
		RS_COPY,
		color.toDWORD()
		);

	pGauge->Render(
		type, no1_team
		);

}

void ResultUI::WinnerApper()
{
	if (m_StateTimer++ > 280)
	{
		m_End = true;
	}
}

void ResultUI::WinnerApperR(
	LPCOLASHAREGAUGE		pGauge,
	TEAM_TYPE				type,
	TEAM_TYPE				no1_team)
{

	MoveGaugeR(
		pGauge,
		type,
		no1_team
		);

	const RECTI& rc = GAMEMATCHINFO.GetScreenSize();
	RECTI texture,rt;
	COLORf color;
	const RATIO t = min((float)m_StateTimer / 50.0f, 1);
	RATIO back_alpha = 1 - max((float)(280 - m_StateTimer), 0) / 50.f;
	back_alpha = max(0, back_alpha);
	back_alpha = min(1, back_alpha);

	texture.width = rc.width / 3;
	texture.height = rc.height / 5;
	texture.x = rc.width / 2;
	texture.y = rc.height / 2;

	rt.x = 0;
	rt.y = 0;
	rt.width = 1515;
	rt.height = 512;

	if (no1_team != type)
	{
		rt.y = 512;
	}

	m_pTextures[_WATER_EFFECT]->Render(
		texture.x - texture.width*3 / 2,
		texture.y - texture.height*3 / 2,
		texture.width*3,
		texture.height*3,

		0,
		0,
		512,
		512,
		RS_COPY,
		CODMgr::GetTeamColaWaterColor(no1_team)
		);

	color.SetColor(CODMgr::GetTeamColor(type));

	m_pTextures[_WIN_LOSE]->Render(
		texture.x - texture.width/2,
		texture.y - texture.height/2,
		texture.width,
		texture.height,

		rt.x,
		rt.y,
		rt.width,
		rt.height,
		RS_COPY,
		color.toDWORD()
		);

	texture.width = (int)((float)texture.width*2.5f*t);
	texture.height = (int)((float)texture.height*2.5f*t);

	color.a = (1 - t);

	m_pTextures[_WIN_LOSE]->Render(
		texture.x - texture.width/2,
		texture.y - texture.height/2,
		texture.width,
		texture.height,

		rt.x,
		rt.y,
		rt.width,
		rt.height,
		RS_ADD,
		color.toDWORD()
		);

	iexPolygon::Rect(
		rc.x,
		rc.y,
		rc.width,
		rc.height,
		RS_COPY,
		D3DCOLOR_COLORVALUE(1, 1, 1, back_alpha)
		);
}

void ResultUI::Start()
{
	m_Start = true;
	m_StateTimer = 0;
	m_pNowUpdateState = &ResultUI::PreBlack;
	m_pNowRenderState = &ResultUI::PreBlackR;
}

void ResultUI::Update(LPCOLASHAREGAUGE		pGauge)
{
	if (!m_Start)return;
	m_pGauge = pGauge;

	m_StateTimer++;

	if (m_pNowUpdateState)(this->*m_pNowUpdateState)();
}

void ResultUI::Render(
	LPCOLASHAREGAUGE		pGauge,
	TEAM_TYPE				type,
	TEAM_TYPE				no1_team
	)
{
	if (m_pNowRenderState)(this->*m_pNowRenderState)(
		pGauge,
		type,
		no1_team
		);
}

bool ResultUI::isEnd()const
{
	return m_End;
}