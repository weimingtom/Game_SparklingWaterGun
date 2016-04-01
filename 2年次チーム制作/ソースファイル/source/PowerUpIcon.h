#ifndef __POWER_UP_ICON_H__
#define __POWER_UP_ICON_H__

#include "ScreenIcon.h"

class PowerUpIcon :public ScreenIconBase
{
private:
	LPIEX2DOBJ			m_pTexture;
	TextureRect			m_TextureRect;
	Vector2				m_Pos, m_Move, m_Power;
	const int			m_MaxTime;
	Vector2				m_Size;
	int					m_LiveCount;
	const ENTITY_ID		m_Owner;
public:
	PowerUpIcon(
		LPIEX2DOBJ				pTexture,
		const TextureRect &		texRect,
		ENTITY_ID				Owner,
		CRVector2				pos,
		CRVector2				move,
		CRVector2				power,
		CRVector2				size,
		int						liveTime
		);
	~PowerUpIcon();
public:
	bool Update();
	void Render(
		LPCONTROLLOBJCT		pViewOwner,
		LPTEAMDATA			pTeamData
		);
};


#endif