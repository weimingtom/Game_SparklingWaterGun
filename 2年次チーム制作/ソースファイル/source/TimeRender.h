#ifndef __TIME_RENDER_H__
#define __TIME_RENDER_H__

#include "iextreme.h"
#include "GameSystem\GameBase.h"

struct TimerRenderMsgInfo
{
	TextureRect		tex_rc;
	COLOR			color;
};

///****************************************************
//		ŽžŠÔ•`‰æƒNƒ‰ƒX
//*****************************************************

class TimeRender:public GameBaseEntity
{
private:
	int			m_Max_time;
	int			m_Time_count;
	LPIEX2DOBJ	m_pNumberTex;
	LPIEX2DOBJ	m_pClockTex;
	bool		m_TexDelete;
public:
	TimeRender(
		LPIEX2DOBJ pNumberTex,
		LPIEX2DOBJ	pClockTex,
		bool	TexDelete);
	~TimeRender();
private:
	void RenderWaku(const TextureRenderRect& trr,COLOR color);
public:
	inline int GetCount()const
	{
		return m_Time_count;
	}
public:
	int	 GetRemainingFrame()const;
	void Reset();
	void SetTime(int frame);
	void SetMaxTime(int frame);
	void Update(int speed);
	void Render(CRTextureRect rc, COLOR color);
	bool HandleMessage(LPGameMessage msg);
};

#endif