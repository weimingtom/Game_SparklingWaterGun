#include "FriendMessage.h"
#include "ControllObject.h"
#include "ControllObjectFunction.h"
#include "GameMatchInfomation.h"

FriendMessageIcon::FriendMessageIcon(
	ENTITY_ID				Parent,
	LPIEX2DOBJ				pTexture,
	const TextureRect &		texRect,
	CRVector3				pos,
	CRVector2				size
	) :
	m_Pos(pos),
	m_Size(size),
	m_pTexture(pTexture),
	m_TextureRect(texRect),
	m_TeamType(-1),
	m_LiveTime(240),
	m_Parent(Parent),
	m_MaxSizeCount(1.5f),
	m_SizeCount(0)
{
	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	m_Size.x /= (float)w;
	m_Size.y /= (float)h;

	Setpos(pos);
}


FriendMessageIcon::~FriendMessageIcon()
{

}


bool FriendMessageIcon::Update()
{
	GameBaseEntity* pObj = IDMANAGER.GetPointer(m_Parent);

	if (m_SizeCount < m_MaxSizeCount)
	{
		m_SizeCount += 0.1f;
	}
	else m_SizeCount = m_MaxSizeCount;

	if ((pObj) == nullptr)
	{
		if (m_LiveTime > 60)
			m_LiveTime = 60;
	}
	else
	{
		ControllObject* pControllObj = (ControllObject*)pObj;
		Setpos(pControllObj->GetBodyCenter());
	}

	return --m_LiveTime > 0;
}

void FriendMessageIcon::Setpos(CRVector3 pos)
{
	m_Pos = pos + Vector3(0, 1, 0);
}

void FriendMessageIcon::SetTeamType(TEAM_TYPE	TeamType)
{
	m_TeamType = TeamType;
}


void FriendMessageIcon::Render(
	LPCONTROLLOBJCT		pViewOwner,
	LPTEAMDATA			pTeamData
	)
{
	if (ControllObjFunc::GetTeamType(pViewOwner) != m_TeamType)return;

	Vector2 EffectSize;
	Vector2 Size = m_Size;
	Vector3 Scrpos;
	const RECTI& rc = GAMEMATCHINFO.GetScreenSize();
	const float space = 0.95f;
	ControllObjFunc::GetCamera(pViewOwner)->ClientToScreen(Scrpos, m_Pos);
	const float effect_alpha = (1 - (m_SizeCount / m_MaxSizeCount))*0.5f;

	if (Scrpos.z < 0)return;

	Scrpos.y = -Scrpos.y;

	Scrpos.x = max(Scrpos.x, -space);
	Scrpos.x = min(Scrpos.x, space);

	Scrpos.y = max(Scrpos.y, -space);
	Scrpos.y = min(Scrpos.y, space);

	Scrpos.x = (Scrpos.x*0.5f + 0.5f)*(float)rc.width;
	Scrpos.y = (Scrpos.y*0.5f + 0.5f)*(float)rc.height;

	const float alpha = (m_LiveTime > 60) ? 1.0f : (float)m_LiveTime / 60.0f;

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
		D3DCOLOR_COLORVALUE(1, 1, 1, alpha)
		);

	
	if (effect_alpha <= 0)
	{
		return;
	}
	
	EffectSize = m_Size + m_Size*m_SizeCount;

	m_pTexture->Render(
		(int)(Scrpos.x - EffectSize.x / 2),
		(int)(Scrpos.y - EffectSize.y / 2),
		(int)EffectSize.x,
		(int)EffectSize.y,
		m_TextureRect.left,
		m_TextureRect.top,
		m_TextureRect.width,
		m_TextureRect.height,
		RS_ADD,
		D3DCOLOR_COLORVALUE(1, 1, 1, effect_alpha)
		);
}