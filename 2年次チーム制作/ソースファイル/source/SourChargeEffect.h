#ifndef __SOUR_CHARGE_EFFECT_H__
#define __SOUR_CHARGE_EFFECT_H__

#include "ScreenIcon.h"

class SourChargeEffect :public ScreenIconBase
{
private:
	LPIEX2DOBJ			m_pTexture;
	Vector2				m_Pos, m_Size, m_Move, m_Target;
	const float			m_Speed;
	const float			m_MaxSpeed;
	const TEAM_TYPE		m_TeamType;
	const ENTITY_ID		m_OwnerControllObjId;
	RATIO				m_LiveTimer;
public:
	SourChargeEffect(
		LPIEX2DOBJ				pTexture,
		CRVector2				pos,
		CRVector2				size,
		CRVector2				move,
		CRVector2				target,
		float					speed,
		float					MaxSpeed,
		TEAM_TYPE				TeamType,
		ENTITY_ID				OwnerControllObjId
		);

	~SourChargeEffect();
private:
	void SendTeamGaugeLightingMsg();
public:
	bool Update();
	void Render(
		LPCONTROLLOBJCT		pViewOwner,
		LPTEAMDATA			pTeamData
		);
};


#endif