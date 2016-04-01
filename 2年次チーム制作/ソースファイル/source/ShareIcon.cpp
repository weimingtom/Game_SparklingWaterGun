#include "ShareIcon.h"
#include "ControllObject.h"
#include "ControllObjectFunction.h"
#include "GameMatchInfomation.h"

ShareIcon::ShareIcon(
	LPIEX2DOBJ				pTexture,
	const TextureRect &		texRect,
	ENTITY_ID				Owner,
	CRVector3				pos,
	CRVector3				move,
	CRVector3				power,
	CRVector2				size,
	int						liveTime,
	bool					animation
	):
	m_Pos(pos),
	m_Move(move),
	m_Power(power),
	m_MaxTime(liveTime),
	m_LiveCount(liveTime),
	m_Owner(Owner),
	m_Size(size),
	m_pTexture(pTexture),
	m_TextureRect(texRect),
	m_Animation(animation)
{
	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	m_Size.x /= (float)w;
	m_Size.y /= (float)h;
}


ShareIcon::~ShareIcon()
{

}


bool ShareIcon::Update()
{
	m_Pos += m_Move;
	m_Move += m_Power;

	return (--m_LiveCount > 0);
}


void ShareIcon::Render(
	LPCONTROLLOBJCT		pViewOwner,
	LPTEAMDATA			pTeamData
	)
{
	if (m_Owner != pViewOwner->GetId())return;

	Vector3 pos;
	ControllObjFunc::GetCamera(pViewOwner)->ClientToScreen(pos, m_Pos);

	if (pos.z < 0)return;

	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w,h);

	pos.x = (pos.x + 1) * 640;
	pos.y = (-pos.y + 1) * 360;

	pos.x /= (float)w;
	pos.y /= (float)h;

	pos.x -= m_Size.x / 2;
	pos.y -= m_Size.y / 2;

	RATIO alpha = (float)m_LiveCount / 40.0f;
	alpha = min(1, alpha);

	if (m_Animation)
	{
		const int cellnum = min(60 - m_LiveCount, 30);
		Vector2 cell(512, 256);

		cell.x = cell.x*(float)(cellnum % 4);
		cell.y = cell.y*(float)(cellnum / 4);

		m_pTexture->Render(
			(int)pos.x,
			(int)pos.y,
			(int)m_Size.x,
			(int)m_Size.y,

			(int)cell.x,
			(int)cell.y,
			512,
			256,
			RS_COPY,
			D3DCOLOR_COLORVALUE(1, 1, 1, alpha)
			);

	}
	else{
		m_pTexture->Render(
			(int)pos.x,
			(int)pos.y,
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
}