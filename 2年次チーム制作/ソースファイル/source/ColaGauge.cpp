#include "ColaGauge.h"
#include "stencil\StencilClip.h"
#include "GlobalTexture.h"
#include "GameMatchInfomation.h"
#include "system\System.h"
#include "GameMatchInfomation.h"
#include "CODPlaySound.h"
#include "GameSound\GameSound.h"

ColaGauge::ColaGauge(
	RATIO			PowerUpSpeed,
	RATIO			PowerDownSpeed,
	CRVector2		default_position) :
	m_state(_APPER),
	m_pCola(nullptr),
	m_Power(0),
	m_isShuffle(false),
	m_count(0),
	m_PowerUpSpeed(PowerUpSpeed),
	m_PowerDownSpeed(PowerDownSpeed),
	m_pos(default_position),
	m_default_position(default_position),
	m_size(300, 300),
	m_max_lighting(false),
	m_color(0xFFFFFFFF),
	m_enable_danger_effect_power(0.4f),
	m_danger_effect_count(0),
	m_SoundTimer(0),
	m_default_position_save(default_position)
{
	m_pCola = GAMEMATCHINFO.GetTextureMgr().GetTexturePtr(TextureManager::COLA_GAUGE);
	m_pColaSour = GAMEMATCHINFO.GetTextureMgr().GetTexturePtr(TextureManager::COLA_GAUGE_SOUR);
	m_pColaClip = GAMEMATCHINFO.GetTextureMgr().GetTexturePtr(TextureManager::COLA_GAUGE_CLIP);

	RollingParam.scr_pos = Vector2(0, 0);
	RollingParam.speed = 1;
	m_sour_count = 0;
	m_sour_speed = 0;

	m_pos.x -= 300;

	RECTI rc = GAMEMATCHINFO.GetScreenSize();

	m_size.x *= (float)rc.width / 1280;
	m_size.y *= (float)rc.height / 720;

	default_size = m_size;
}

ColaGauge::~ColaGauge()
{
	
}

void ColaGauge::RenderDangerEffect(RATIO t, CRVector2 size, RATIO alpha)
{
	const Vector2 rsize = size*(1 + t*0.5f);

	m_pCola->RenderPlus(
		m_pos,
		rsize,
		0,
		VECTOR2ZERO,
		Vector2(256, 256),
		RS_COPY,
		COLORf((1 - t)*alpha, 1, 0, 0).toDWORD()
		);
}

void ColaGauge::SetColaVertex(CRVector2 scale)
{
	static float positions[cola_vertex_num][2] =
	{
		{115,30},		//0
		{140,30},		//1
		{113,45},		//2
		{142,45},		//3
		{107,60},		//4
		{149,50},		//5
		{100,75},		//6
		{156,75},		//7
		{95,90},		//8
		{161,90},		//9
		{92,100},		//10
		{163,100},		//11
		{92,105},		//12
		{164,105},		//13
		{92,155},		//14
		{163,155},		//15
		{92,159},		//16
		{164,159},		//17
		{98,185},		//18
		{158,185},		//19
		{98,198},		//20
		{ 158, 198 },	//21
		{92,212},		//22
		{164,212},		//23
		{92,234},		//24
		{164,234},		//25
		{95,246},		//26
		{162,246}		//27
	};

	const Vector2 center(
		(256) / 2.0f,
		(256) / 2.0f
		);

	const float height = fLERP(positions[cola_vertex_num - 1][1], positions[0][1], m_Power);

	for (int i = 0; i < cola_vertex_num; i++)
	{

		m_GaugeVertex[i].sx = positions[i][0];
		m_GaugeVertex[i].sy = positions[i][1];

		if (m_GaugeVertex[i].sy < height)m_GaugeVertex[i].sy = height;

		m_GaugeVertex[i].sx -= center.x;
		m_GaugeVertex[i].sy -= center.y;

		m_GaugeVertex[i].sx *= scale.x;
		m_GaugeVertex[i].sy *= scale.y;

		m_GaugeVertex[i].sx += m_pos.x;
		m_GaugeVertex[i].sy += m_pos.y;
		m_GaugeVertex[i].sz = 0;

		m_GaugeVertex[i].tu = 0;
		m_GaugeVertex[i].tv = 0;
		m_GaugeVertex[i].rhw = 1;
		m_GaugeVertex[i].color = 0xFFFFFFFF;
	}
}

void ColaGauge::UpdateSound(CRVector3 pos)
{
	const float sn = sinf((float)m_count*0.4f);

	if (m_SoundTimer == 0 &&
		m_gauge_shuffle_speed >= 0.3f &&
		(sn >= 0.96f || sn <= -0.96f))
	{
		m_SoundTimer = 10;

		SOUNDPLAYER.PlaySE(
			sound_t::shake,
			SOUNDPLAYER.GetSoundVolume(pos, sound_t::shake)
			);
	}
}

void ColaGauge::SetPowerUpSpeed(RATIO p)
{
	m_PowerUpSpeed = p;
}

void ColaGauge::SetnextState(COLA_GAUGE_STATE	state)
{
	m_state = state; 
	m_count = 0;
}

void ColaGauge::AddPower(RATIO p)
{
	m_Power += p;
	m_Power = max(0, m_Power);
	m_Power = min(m_Power, 1);
}

void ColaGauge::ShuffleState_Apper()
{
	Vector2 m = m_default_position - m_pos;
	if (Vector2Length(m) < 2.0f)
	{
		m_pos = m_default_position;
		SetnextState(_GAUGE);
		{
			m_sour_speed = max(m_sour_speed - 0.04f, 4);
			m_gauge_shuffle_speed -= 0.04f;
			m_gauge_shuffle_speed = max(0, m_gauge_shuffle_speed);
		}
		return;
	}

	m *= 0.4f;
	if (Vector2Length(m) > 5)
	{
		m = Vector2Normalize(m);
		m *= 5.0f;
	}
	m_pos += m;
}

void ColaGauge::ShuffleState_Gauge()
{
	RATIO p;
	if (++m_count == 5000)m_count = 0;

	if (m_isShuffle)
	{
		p = m_PowerUpSpeed;
		m_gauge_shuffle_speed += 0.02f;
		m_gauge_shuffle_speed = min(1, m_gauge_shuffle_speed);
		m_sour_speed = min(m_sour_speed + 0.5f, 13);
	}
	else{
		m_sour_speed = max(m_sour_speed - 0.04f, 4);
		p = -m_PowerDownSpeed;
		m_gauge_shuffle_speed -= 0.04f;
		m_gauge_shuffle_speed = max(0, m_gauge_shuffle_speed);
	}

	m_pos = m_default_position;
	m_pos.y += sinf((float)m_count*0.4f)*m_gauge_shuffle_speed*60;

	m_Power += p;
	m_Power = max(0, m_Power);
	m_Power = min(m_Power, 1);
}

void ColaGauge::ShuffleState_Rolling()
{
	Vector2 m = RollingParam.scr_pos - m_pos;

	if (Vector2Length(m) < 1)
	{
		m_pos = RollingParam.scr_pos;
		return;
	}

	m_size += (RollingParam.size - m_size)*0.2f;
	m *= RollingParam.speed;

	m_pos += m;
}

void ColaGauge::SetColor(DWORD	 color)
{
	m_color = color;
}

bool ColaGauge::isShuffle()const
{
	return m_isShuffle;
}

RATIO ColaGauge::GetPower()const
{
	return m_Power;
}

void ColaGauge::SetShuffleFlg(bool flg)
{
	m_isShuffle = flg;
}

void ColaGauge::Reset()
{
	m_size = default_size;
	m_pos = m_default_position;
	m_pos.x -= 500;

	m_Power = 0;

	SetnextState(_APPER);
}

void ColaGauge::GoGauge(CRVector2 scr_pos, CRVector2 scr_size, RATIO speed)
{
	RollingParam.size = scr_size;
	RollingParam.scr_pos = scr_pos;
	RollingParam.speed = speed;
	SetnextState(_ROLLING);
}

void ColaGauge::Update()
{
	if (isErrorValuef(m_default_position.x) ||
		isErrorValuef(m_default_position.y))
	{
		m_default_position = m_default_position_save;
	}
	if (m_sour_count < 10000.0f)
		m_sour_count += m_sour_speed;
	else
		m_sour_count = 0;

	if (m_SoundTimer>0)m_SoundTimer--;

	if (m_Power < m_enable_danger_effect_power&&
		m_danger_effect_count < 10000)
	{
		m_danger_effect_count += max((m_enable_danger_effect_power - m_Power)*0.13f, 0.03f);
	}
	else{
		m_danger_effect_count = 0;
	}

	if (m_Power < 0.8f)m_max_lighting = false;
	if (m_Power >= 1)m_max_lighting = true;

	static void(ColaGauge::*functions[3])() =
	{
		&ColaGauge::ShuffleState_Apper,
		&ColaGauge::ShuffleState_Gauge,
		&ColaGauge::ShuffleState_Rolling
	};
	(this->*functions[m_state])();
}

void ColaGauge::Render()
{
	float gauge_size_up = (sinf(m_sour_count*0.03f)*0.5f + 0.5f)*(max(m_Power - 0.75f, 0));
	Vector2 rsize = m_size;
	rsize.x *= 1 + gauge_size_up + (m_Power*0.1f);
	rsize.y *= 1 + gauge_size_up*0.5f;
	Vector2 scale(rsize);

	scale.x *= 0.004f;
	scale.y *= 0.004f;
	
	SetColaVertex(scale);

	{
		//クリッピング作成
		m_StencilClip.regionBegin(iexSystem::Device);
		m_StencilClip.setWriteMaskColor(StencilClip::MaskColor_Fill);

		iexPolygon::Render2D(
			m_GaugeVertex,
			(cola_vertex_num - 2),
			0,
			RS_COPY
			);

		m_StencilClip.regionEnd();
	}

	m_pCola->RenderPlus(
		m_pos,
		rsize,
		0,
		VECTOR2ZERO,
		Vector2(256,256)
		);


	if (m_Power < m_enable_danger_effect_power)
	{
		float alpha = (m_enable_danger_effect_power - m_Power) / 0.1f;
		alpha = min(0.8f, alpha);

		RenderDangerEffect(
			fmodf(m_danger_effect_count, 1),
			rsize,
			alpha
			);
	}
	m_StencilClip.setRefMaskColor(StencilClip::MaskColor_Trans);
	m_StencilClip.drawBegin();


	m_pColaSour->RenderPlus(
		m_pos,
		rsize,
		0,
		Vector2(0, m_sour_count),
		Vector2(1600, 1600),
		RS_COPY,
		D3DCOLOR_COLORVALUE(1, 1, 1, min((m_Power*2),1))
		);

	m_pColaClip->RenderPlus(
		m_pos,
		rsize,
		0,
		VECTOR2ZERO,
		Vector2(256, 256),
		RS_COPY,
		m_color
		);

	if (m_max_lighting)
	{
		m_pColaClip->RenderPlus(
			m_pos,
			rsize,
			0,
			VECTOR2ZERO,
			Vector2(256, 256),
			RS_ADD,
			D3DCOLOR_COLORVALUE(1, 1, 1, sinf(m_sour_count*0.01f)*0.1f + 0.1f)
			);
	}

	m_StencilClip.drawEnd();
}