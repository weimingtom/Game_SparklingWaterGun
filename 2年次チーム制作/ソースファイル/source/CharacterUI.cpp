#include "All.h"
#include "ColaGameGauge.h"
#include "StartCountDown.h"
#include "NewsTelopWord.h"


//********************************************************
//	キャラクタのUI関連をまとめたクラス
//********************************************************
CharacterUI::CharacterUI():
m_pOwner(nullptr),
m_pColaGauge(nullptr),
m_SunShine(Vector3Normalize(Vector3(-1,0.5f,1))),
m_ScrDamageEffect(new iex2DObj("DATA\\TEXTURE\\UI\\Gradation.png"))
{
	m_pStartCountDown = new StartCountDown(new iex2DObj("DATA\\TEXTURE\\Start_End.png"));
	m_pAimTexture = new iex2DObj("DATA\\TEXTURE\\UI\\circle.png");

	m_pAim_point = new AimPoint(
		m_pAimTexture,
		GetConstChrParam(nullptr)->default_auto_aim_area_size,
		GetConstChrParam(nullptr)->default_sight_size,
		0.14f,
		0.085f
		);

	Vector2 gpos(0.1f,0.6f);
	RECTI rc = GAMEMATCHINFO.GetScreenSize();

	gpos.x *= (float)rc.width;
	gpos.y *= (float)rc.height;

	m_pColaGauge = new ColaGauge(
		GetConstChrParam(nullptr)->cola_gauge_up_speed,
		GetConstChrParam(nullptr)->cola_gauge_down_speed,
		gpos
		);

	m_pPositionEffect = new PositionEffect();

	
}


CharacterUI::~CharacterUI()
{
	SAFE_DELETE(m_pAimTexture);
	SAFE_DELETE(m_pAim_point);
	SAFE_DELETE(m_pColaGauge);
	SAFE_DELETE(m_pPositionEffect);
	SAFE_DELETE(m_pStartCountDown);
}

bool CharacterUI::GetTargetID(AIMTARGET_ID& out)
{
	out = m_pAim_point->GetTarget().id;
	return out != ERR_AIMTARGET_ID;
}

void CharacterUI::GetTargetPos(Vector3& out)
{
	m_pAim_point->GetReactionTargetPos(m_pOwner->GetBodyCenter(), out);
}

void CharacterUI::GetTargetMove(Vector3& out)
{
	out = m_pAim_point->GetTarget().cli_move;
}

bool CharacterUI::GetTarget(AimTargetInfo& out)
{
	out = m_pAim_point->GetTarget();
	return out.id != ERR_AIMTARGET_ID;
}

RATIO CharacterUI::GetColaGaugePower()const
{
	return m_pColaGauge->GetPower();
}

bool CharacterUI::isShuffle()const
{
	return m_pColaGauge->isShuffle();
}

void CharacterUI::SetOwner(LPCHARACTER p)
{
	m_pOwner = p;
	m_ScrDamageEffect.SetOwnerCharacter(p);
}

void CharacterUI::SetChangeTargetFlg(bool flg)
{
	m_pAim_point->SetChangeTarget(flg);
}

void CharacterUI::ResetGauge()
{
	m_pColaGauge->Reset();
}

void CharacterUI::SetGaugeMove(CRVector2 pos, CRVector2 size)
{
	m_pColaGauge->GoGauge(pos, size, 0.2f);
}

void CharacterUI::SetColaGaugeColor(DWORD color)
{
	m_pColaGauge->SetColor(color);
}

void	CharacterUI::SetReaction(float power)
{
	m_pAim_point->SetReaction(power);
}

void CharacterUI::SetDamageEffect(
	PositionEffect::AnimeEffectParam& param,
	bool isDamage)
{
	const Vector3& v = ChrFunc::GetParam(m_pOwner).m_view_vec;
	Vector2 temp = param.pos;
	m_pPositionEffect->CalcScrPos(temp, Vector2Normalize(Vector2(v.x, v.z)), param.pos);
	m_pPositionEffect->SetEffect(param);

	if (isDamage)
	{
		ControllObjFunc::SetShockVecCamera(m_pOwner, param.pos, PI_d4*0.05f);
		m_ScrDamageEffect.SetDamageEffect(0.5f);
	}
}

void CharacterUI::SetShuffleFlg(bool flg)
{
	if (flg&&m_pOwner)
	{
		m_pColaGauge->SetPowerUpSpeed(GetConstChrParam(nullptr)->cola_gauge_up_speed + ChrFunc::GetParam(m_pOwner).UpValue.ChargeSpeed);
	}
	m_pColaGauge->SetShuffleFlg(flg);
}

void CharacterUI::AddColaGauge(RATIO p)
{
	m_pColaGauge->AddPower(p);
}

void CharacterUI::Initialize()
{

}

void CharacterUI::Update()
{
	if (m_pOwner == nullptr)return;

	AimOwnerInfo info;

	CreateAimOwnerParam(info);
	m_pAim_point->Update(info);

	m_ScrDamageEffect.Update();
	m_pPositionEffect->Update();
	
	SAFE_EXE(m_pColaGauge, Update);
	SAFE_EXE(m_pColaGauge, UpdateSound, m_pOwner->GetBodyCenter());

	m_pStartCountDown->Update();
}

void CharacterUI::RenderScore()
{
	GameMessage		msg;

	msg.from = m_pOwner->GetId();
	msg.to = game_id::id_score_calc_manager;
	msg.type = msg_t::render_score_msg;
	msg.exinfo = &ControllObjFunc::GetMatchData(m_pOwner)->team_type;

	IDMANAGER.Dispatch(
		&msg
		);
}

void CharacterUI::CreateAimOwnerParam(AimOwnerInfo& info)
{
	Vector3 tmp;

	info.pCamera = ControllObjFunc::GetCamera(m_pOwner);
	info.circle_size = GetConstChrParam(m_pOwner)->default_auto_aim_area_size;
	info.id = m_pOwner->GetId();
	info.position = ControllObjFunc::GetPos(m_pOwner);
	info.range = 30;
	info.team_type = ControllObjFunc::GetMatchData(m_pOwner)->team_type;
	info.pCamera->ClientToScreen(tmp, info.position);
	info.camera_z = tmp.z;
	info.view_vec = ChrFunc::GetParam(m_pOwner).m_view_vec;
	info.view_vec.Normalize();
}

void CharacterUI::RenderIcon(ControllObject& viewowner, TeamData& teamdata)
{
	GameMessage		msg;
	RenderOwnerMsgExinfo exInfo;

	exInfo.pTeamData = &teamdata;
	exInfo.pViewOwner = &viewowner;

	msg.from = m_pOwner->GetId();
	msg.to = game_id::id_screen_icon_manager;
	msg.type = msg_t::icon_render_msg;
	msg.exinfo = &exInfo;

	IDMANAGER.Dispatch(
		&msg
		);
}

void CharacterUI::RenderTelop(ControllObject& viewowner, TeamData& teamdata)
{
	GameMessage		msg;
	RenderOwnerMsgExinfo exInfo;

	exInfo.pTeamData = &teamdata;
	exInfo.pViewOwner = &viewowner;

	msg.from = m_pOwner->GetId();
	msg.to = game_id::id_news_telop_manager;
	msg.type = msg_t::news_telop_render_msg;
	msg.exinfo = &exInfo;

	IDMANAGER.Dispatch(
		&msg
		);
}

void CharacterUI::RenderTimer()
{
	RECTI screen = GAMEMATCHINFO.GetScreenSize();
	GameMessage		msg;
	TimerRenderMsgInfo info;

	info.tex_rc.width = screen.width / 8;
	info.tex_rc.height = screen.height / 16;
	info.tex_rc.left = screen.width / 64;
	info.tex_rc.top = screen.height / 32;

	info.color = CODMgr::GetTeamColor(ControllObjFunc::GetTeamType(m_pOwner));

	msg.from = m_pOwner->GetId();
	msg.to = game_id::id_timer;
	msg.type = msg_t::timer_render_msg;
	msg.exinfo = &info;

	IDMANAGER.Dispatch(
		&msg
		);
}

void CharacterUI::Render(ControllObject& viewowner, TeamData& teamdata)
{
	m_SunShine.Render(ControllObjFunc::GetCamera(&viewowner));

	m_pAim_point->Render();

	RECTI screen = GAMEMATCHINFO.GetScreenSize();
	int size = screen.width / 4;

	SAFE_EXE(m_pColaGauge,Render);

	m_ScrDamageEffect.Render();
	m_pPositionEffect->Render();

	RenderIcon( viewowner,  teamdata);
	RenderTelop(viewowner, teamdata);
	RenderTimer();
	RenderScore();
}