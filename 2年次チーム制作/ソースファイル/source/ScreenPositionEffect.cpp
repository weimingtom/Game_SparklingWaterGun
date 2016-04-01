#include "ScreenPositionEffect.h"
#include "GameMatchInfomation.h"
#include "system\System.h"
#include <new>
#include "EffectResource.h"

PositionEffect::PositionEffect() :
m_EffectCircleSize(0.75f),
m_Set_index_count(0)
{
	m_pPosition_tex[0] = new iex2DObj("DATA\\TEXTURE\\UI\\damage1.png");
	m_pPosition_tex[1] = new iex2DObj("DATA\\TEXTURE\\UI\\damage2.png");
	m_pPosition_tex[2] = new iex2DObj("DATA\\TEXTURE\\UI\\damage3.png");

	m_Animation_texparam = POLYGONEFFECT.GetTexture(EffectTex::_SOUR_MANY);
}

PositionEffect::~PositionEffect()
{
	for (int i = 0; i < m_PositionTex; ++i)
	{
		delete m_pPosition_tex[i];
	}
}

void PositionEffect::CalcScrPos(
	CRVector2 in_vec,
	CRVector2 view_vec,
	Vector2& out_vec)
{
	float angle = -atan2f(view_vec.x, view_vec.y);

	const float rd = m_EffectCircleSize*0.25f;
	RECTI rc = GAMEMATCHINFO.GetScreenSize();
	Vector2 rc_vec((float)rc.width / 2, (float)rc.height / 2);

	out_vec.x = in_vec.x*cosf(angle) + in_vec.y*sinf(angle);
	out_vec.y = in_vec.x*sinf(angle) + in_vec.y*-cosf(angle);

	out_vec *= m_EffectCircleSize;

	out_vec.x += fRAND(-rd, rd);
	out_vec.y += fRAND(-rd, rd);

	out_vec.x *= rc_vec.x;
	out_vec.y *= rc_vec.y;

	out_vec += rc_vec;
}

void PositionEffect::SetEffect(const AnimeEffectParam& param)
{
#undef new

	m_AnimeList[m_Set_index_count].~AnimationEffect();

	new(&m_AnimeList[m_Set_index_count]) AnimationEffect(
				param
			);
	
	if (++m_Set_index_count >= m_maxAnime)
	{
		m_Set_index_count = 0;
	}

#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
}


void PositionEffect::ScrPosEffectUpdate()
{
	for (int i = 0; i < m_maxAnime; ++i)
	{
		if (m_AnimeList[i].isEnable())
		{
			m_AnimeList[i].Update();
		}
	}
}

void PositionEffect::Update()
{
	ScrPosEffectUpdate();
}

void PositionEffect::Render()
{
	LVERTEX	VertexArray[4];
	RECTI rc = GAMEMATCHINFO.GetScreenSize();
	COLORf color;

	for (int i = 0; i < m_maxAnime; ++i)
	{
		if (m_AnimeList[i].isEnable())
		{
			m_AnimeList[i].Render(
				"screenposition_effect",
				VertexArray,
				m_Animation_texparam,
				m_pPosition_tex
				);
		}
	}
}

PositionEffect::AnimationEffect::AnimationEffect()
{
	m_param.angle = 0;
	m_param.color = 0;
	m_param.max_time = 1;
	m_param.time = 0;
	m_param.size.x = 1;
	m_param.size.y = 1;
	m_param.pos.x = 0;
	m_param.pos.y = 0;
}

PositionEffect::AnimationEffect::AnimationEffect(
	const AnimeEffectParam&	param)
{
	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w,h);
	m_param = param;
	m_param.size.x /= static_cast<float>(w);
	m_param.size.y /= static_cast<float>(h);
	m_param.texture_type = rand() % PositionEffect::m_PositionTex;
}

void PositionEffect::AnimationEffect::Update()
{
	--m_param.time;
	float t = (float)m_param.time / 40;

	if (t > 1)t = 1;

	m_param.color = (m_param.color & 0x00FFFFFF) | ((DWORD)((t)* 255) << 24);
}

void PositionEffect::AnimationEffect::Render(
	char*				shader_name,
	LVERTEX				VertexArray[4],
	TextureParameter&	Anime_tex,
	LPIEX2DOBJ*			Position_tex)
{
	RECTI rect = GAMEMATCHINFO.GetScreenSize();

	VertexArray[0].x = -m_param.size.x;
	VertexArray[0].y = -m_param.size.y;

	VertexArray[1].x = m_param.size.x;
	VertexArray[1].y = -m_param.size.y;

	VertexArray[2].x = -m_param.size.x;
	VertexArray[2].y = m_param.size.y;

	VertexArray[3].x = m_param.size.x;
	VertexArray[3].y = m_param.size.y;


	for (int i = 0; i < 4; ++i)
	{
		VertexArray[i].color = m_param.color;
		VertexArray[i].x += m_param.pos.x;
		VertexArray[i].y += m_param.pos.y;
		VertexArray[i].z = 0;

		VertexArray[i].x /= rect.width;
		VertexArray[i].y /= rect.height;
	}


	VertexArray[0].tu = 0;
	VertexArray[0].tv = 0;

	VertexArray[1].tu = 1;
	VertexArray[1].tv = 0;

	VertexArray[2].tu = 0;
	VertexArray[2].tv = 1;

	VertexArray[3].tu = 1;
	VertexArray[3].tv = 1;

	iexPolygon::Render3D(VertexArray, 2, Position_tex[m_param.texture_type], shader, shader_name);


	if (Anime_tex.m_TexParam.maxSell>(m_param.max_time - m_param.time))
	{
		for (int i = 0; i < 4; ++i)
		{
			VertexArray[i].x *= rect.width;
			VertexArray[i].y *= rect.height;
		}

		VertexArray[0].y -= m_param.size.y * 6;
		VertexArray[1].y -= m_param.size.y * 6;

		VertexArray[0].x -= m_param.size.x*2;
		VertexArray[1].x += m_param.size.x*2;
		VertexArray[2].x -= m_param.size.x*2;
		VertexArray[3].x += m_param.size.x*2;

		Anime_tex.m_TexParam.SetSellUV(VertexArray, m_param.max_time - m_param.time);

		TLVERTEX v[4];
		for (int i = 0; i < 4; ++i)
		{
			v[i].sx = VertexArray[i].x;
			v[i].sy = VertexArray[i].y;
			v[i].sz = VertexArray[i].z;
			v[i].tu = VertexArray[i].tu;
			v[i].tv = VertexArray[i].tv;
			v[i].color = VertexArray[i].color;
			v[i].rhw = 1;
		}

		iexPolygon::Render2D(v, 2, Anime_tex.m_pTexture, RS_COPY);
	}
}


float PositionEffect::AnimationEffect::GetZ()const
{
	return (float)(m_param.max_time - m_param.time);
}

BOOL PositionEffect::AnimationEffect::isEnable()const
{
	return static_cast<BOOL>(m_param.time > 0);
}