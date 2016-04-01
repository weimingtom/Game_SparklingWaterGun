#include "ShareObjIcon.h"
#include "ControllObject.h"
#include "ControllObjectFunction.h"
#include "GameMatchInfomation.h"

ShareObjIcon::ShareObjIcon(
	LPIEX2DOBJ				pTexture,
	const TextureRect &		texRect,
	CRVector3				pos,
	CRVector2				size
	) :
	m_Pos(pos),
	m_Size(size),
	m_pTexture(pTexture),
	m_TextureRect(texRect),
	m_TeamType(-1)
{
	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	m_Size.x /= (float)w;
	m_Size.y /= (float)h;
}


ShareObjIcon::~ShareObjIcon()
{

}


bool ShareObjIcon::Update()
{
	return true;
}

void ShareObjIcon::SetPos(CRVector3 pos)
{
	m_Pos = pos;
}

void ShareObjIcon::SetTeamType(TEAM_TYPE	TeamType)
{
	m_TeamType = TeamType;
}


void ShareObjIcon::Render(
	LPCONTROLLOBJCT		pViewOwner,
	LPTEAMDATA			pTeamData
	)
{
	if (ControllObjFunc::GetTeamType(pViewOwner) == m_TeamType)return;

	Vector3 Scrpos;
	const RECTI& rc = GAMEMATCHINFO.GetScreenSize();
	const float space = 0.95f;
	ControllObjFunc::GetCamera(pViewOwner)->ClientToScreen(Scrpos, m_Pos);

	Scrpos.y = -Scrpos.y;
#if 0
	if (Scrpos.z < 0)return;

	

	Scrpos.x = max(Scrpos.x, -space);
	Scrpos.x = min(Scrpos.x, space);

	Scrpos.y = max(Scrpos.y, -space);
	Scrpos.y = min(Scrpos.y, space);

	Scrpos.x = (Scrpos.x*0.5f + 0.5f)*(float)rc.width;
	Scrpos.y = (Scrpos.y*0.5f + 0.5f)*(float)rc.height;

#else 

	if (Scrpos.z < 0||
		Scrpos.x>1 || Scrpos.x<-1 ||
		Scrpos.y>1 || Scrpos.y<-1 
		)return;

	Scrpos.x = (Scrpos.x*0.5f + 0.5f)*(float)rc.width;
	Scrpos.y = (Scrpos.y*0.5f + 0.5f)*(float)rc.height;

#endif

	const float render_begin = 25.0f;
	const float render_end = 30.0f;


	if (Scrpos.z > render_end)
	{
		return;
	}

	if (Scrpos.z < render_begin)
	{
		Scrpos.z = 1;
	}
	else
	{
		Scrpos.z -= render_begin;
		Scrpos.z /= render_end - render_begin;

		if (Scrpos.z>1)
		{
			return;
		}
		Scrpos.z = 1 - Scrpos.z;
	}

	

	m_pTexture->Render(
		(int)(Scrpos.x - m_Size.x / 2),
		(int)(Scrpos.y - m_Size.y / 2),
		(int)m_Size.x,
		(int)m_Size.y,
		m_TextureRect.left,
		m_TextureRect.top,
		m_TextureRect.width,
		m_TextureRect.height,
		RS_COPY,
		D3DCOLOR_COLORVALUE(1, 1, 1, Scrpos.z)
		);
}