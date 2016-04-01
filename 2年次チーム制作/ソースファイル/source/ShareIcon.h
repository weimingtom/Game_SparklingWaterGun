#ifndef __SHARE_ICON_H__
#define __SHARE_ICON_H__

#include "ScreenIcon.h"

class ShareIcon :public ScreenIconBase
{
private:
	LPIEX2DOBJ			m_pTexture;
	TextureRect			m_TextureRect;
	Vector3				m_Pos, m_Move, m_Power;
	const int			m_MaxTime;
	Vector2				m_Size;
	int					m_LiveCount;
	const ENTITY_ID		m_Owner;
	bool				m_Animation;
public:
	ShareIcon(
		LPIEX2DOBJ				pTexture,
		const TextureRect &		texRect,
		ENTITY_ID				Owner,
		CRVector3				pos,
		CRVector3				move,
		CRVector3				power,
		CRVector2				size,
		int						liveTime,
		bool					animation
		);
	~ShareIcon();
public:
	bool Update();
	void Render(
		LPCONTROLLOBJCT		pViewOwner,
		LPTEAMDATA			pTeamData
		);
};

#endif