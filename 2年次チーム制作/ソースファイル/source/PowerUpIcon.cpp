#include "PowerUpIcon.h"
#include "ControllObject.h"
#include "ControllObjectFunction.h"
#include "GameMatchInfomation.h"

PowerUpIcon::PowerUpIcon(
	LPIEX2DOBJ				pTexture,
	const TextureRect &		texRect,
	ENTITY_ID				Owner,
	CRVector2				pos,
	CRVector2				move,
	CRVector2				power,
	CRVector2				size,
	int						liveTime
	) :
	m_Pos(pos),
	m_Move(move),
	m_Power(power),
	m_MaxTime(liveTime),
	m_LiveCount(liveTime),
	m_Owner(Owner),
	m_Size(size),
	m_pTexture(pTexture),
	m_TextureRect(texRect)
{
	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	m_Size.x /= (float)w;
	m_Size.y /= (float)h;
}


PowerUpIcon::~PowerUpIcon()
{

}


bool PowerUpIcon::Update()
{
	m_Pos += m_Move;
	m_Move += m_Power;

	return (--m_LiveCount > 0);
}


void PowerUpIcon::Render(
	LPCONTROLLOBJCT		pViewOwner,
	LPTEAMDATA			pTeamData
	)
{
	if (m_Owner != pViewOwner->GetId())return;

	RATIO alpha = (float)m_LiveCount / 40.0f;
	alpha = min(1, alpha);

	m_pTexture->Render(
		(int)(m_Pos.x-m_Size.x/2),
		(int)(m_Pos.y-m_Size.y/2),
		(int)m_Size.x,
		(int)m_Size.y,
		m_TextureRect.left,
		m_TextureRect.top,
		m_TextureRect.width,
		m_TextureRect.height,
		RS_COPY,
		D3DCOLOR_COLORVALUE(1, 1, 1, alpha)
		);
}