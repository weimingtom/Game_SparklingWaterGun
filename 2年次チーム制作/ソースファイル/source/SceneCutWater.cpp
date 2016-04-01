#include "SceneCutWater.h"

SceneCutWater::SceneCutWater(
	LPIEX2DOBJ		pWaterTex,
	LPIEX2DOBJ		pSourTex,
	int				scr_width,
	int				scr_height):
	m_pWaterTex(pWaterTex),
	m_pSourTex(pSourTex),
	m_NowHeight(0),
	m_Scr_width(scr_width),
	m_Scr_height(scr_height),
	m_Count(0)
{

}

SceneCutWater::~SceneCutWater()
{

}

void SceneCutWater::UpdateSour()
{
	for (int i = 0; i < numSour; ++i)
	{
		m_SourParam[i].count += 1;
		m_SourParam[i].move.x += sinf(m_SourParam[i].count)*0.1f;
		//m_SourParam[i].
	}
}

void SceneCutWater::Update(int Speed)
{
	m_Count += 2;
	m_NowHeight += Speed;
	m_NowHeight = max(0, m_NowHeight);
	m_NowHeight = min(m_NowHeight, m_Scr_height+100);
}

void SceneCutWater::Render()
{
	m_pWaterTex->Render(
		0,
		m_Scr_height-m_NowHeight,
		1280,
		200,

		(int)m_Count,
		0,
		2048,
		512,
		RS_COPY
		);

	m_pWaterTex->Render(
		0,
		m_Scr_height - m_NowHeight+200,
		1280,
		720,

		0,
		300,
		2048,
		50,
		RS_COPY
		);

}