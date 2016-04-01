#ifndef __RESULT_UI_H__
#define __RESULT_UI_H__

#include "YenRender.h"
#include "map\MapObject.h"


class ResultUI
{
private:
	typedef void(ResultUI::*StateFunc)();
	typedef void(ResultUI::*StateRenderFunc)(LPCOLASHAREGAUGE, TEAM_TYPE, TEAM_TYPE);

	static const int	Team_max = 2;
	enum TEXTURE_TYPE
	{
		_FINISH,
		_WIN_LOSE,
		_WATER_EFFECT,
		__MAX,
	};
	RATIO				m_Back_alpha;
	bool				m_Start;
	bool				m_End;
	LPIEX2DOBJ			m_pTextures[__MAX];
	StateFunc			m_pNowUpdateState;
	StateRenderFunc		m_pNowRenderState;
	float				m_Work;
	int					m_StateTimer;
	LPCOLASHAREGAUGE	m_pGauge;
public:
	ResultUI();
	~ResultUI();
private:
	void PreBlack();
	void MoveGauge();
	void WinnerApper();
private:
	void PreBlackR(LPCOLASHAREGAUGE, TEAM_TYPE, TEAM_TYPE);
	void MoveGaugeR(LPCOLASHAREGAUGE, TEAM_TYPE, TEAM_TYPE);
	void WinnerApperR(LPCOLASHAREGAUGE, TEAM_TYPE, TEAM_TYPE);
public:
	void Start();
	void Update(LPCOLASHAREGAUGE		pGauge);
	void Render(
		LPCOLASHAREGAUGE		pGauge,
		TEAM_TYPE				type,
		TEAM_TYPE				no1_team);
	bool isEnd()const;
};


#endif