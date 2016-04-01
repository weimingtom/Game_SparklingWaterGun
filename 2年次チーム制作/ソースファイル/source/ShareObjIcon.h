#ifndef __SHARE_OBJ_ICON_H__
#define __SHARE_OBJ_ICON_H__

#include "ScreenIcon.h"

class ShareObjIcon :public ScreenIconBase
{
private:
	LPIEX2DOBJ			m_pTexture;
	TextureRect			m_TextureRect;
	Vector3				m_Pos;
	Vector2				m_Size;
	TEAM_TYPE			m_TeamType;
public:
	ShareObjIcon(
		LPIEX2DOBJ				pTexture,
		const TextureRect &		texRect,
		CRVector3				pos,
		CRVector2				size
		);
	~ShareObjIcon();
public:
	void SetPos(CRVector3 pos);
	void SetTeamType(TEAM_TYPE	TeamType);
	bool Update();
	void Render(
		LPCONTROLLOBJCT		pViewOwner,
		LPTEAMDATA			pTeamData
		);
};


#endif