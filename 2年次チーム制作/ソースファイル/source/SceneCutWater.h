#ifndef __SCENE_CUT_WATER_H__
#define __SCENE_CUT_WATER_H__

#include "iextreme.h"

class SceneCutWater
{
private:
	struct SourParam
	{
		Vector2 pos;
		Vector2 move;
		float size;
		float count;
	};
private:
	static const int numSour = 100;
	SourParam		m_SourParam[numSour];
	LPIEX2DOBJ		m_pWaterTex;
	LPIEX2DOBJ		m_pSourTex;
	LVERTEX			m_Vertex[4];
	int				m_NowHeight;
	int				m_Scr_width;
	int				m_Scr_height;
	float			m_Count;
public:
	SceneCutWater(
		LPIEX2DOBJ		pWaterTex,
		LPIEX2DOBJ		pSourTex,
		int				scr_width,
		int				scr_height
		);
	~SceneCutWater();
private:
	void UpdateSour();
public:
	void Update(int Speed);
	void Render();
};

#endif
