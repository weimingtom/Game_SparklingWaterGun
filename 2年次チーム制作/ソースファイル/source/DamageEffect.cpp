#include "DamageEffect.h"
#include "LuaLoadParam.h"
#include "Character.h"
#include "CharacterUtilityFunction.h"
#include "GameMatchInfomation.h"



DamageEffect::DamageEffect(LPIEX2DOBJ		pScrTexture) :
m_Alpha(0),
m_Count(0),
m_pCharacter(nullptr),
m_pScrTexture(pScrTexture)
{

}

DamageEffect::~DamageEffect()
{
	delete m_pScrTexture;
}

void DamageEffect::SetDamageEffect(RATIO power)
{
	m_Alpha = max(power, m_Alpha);
}

void DamageEffect::SetOwnerCharacter(
	LPCHARACTER		Character)
{
	m_pCharacter = Character;
}

void DamageEffect::Update()
{
	if (!m_pCharacter)
	{
		m_Alpha = 0;
		m_Count = 0;
		return;
	}

	const float max_hp = GetConstChrParam(m_pCharacter)->max_hp;
	const float now_hp = ChrFunc::GetParam(m_pCharacter).m_LifePoint;
	const float target_alpha = (1 - now_hp / max_hp - 0.5f)*2.0f;

	m_Count += 1;
	if (target_alpha < m_Alpha)
	{
		m_Alpha += (-m_Alpha)*0.025f;
	}
	else{
		m_Alpha = target_alpha;
	}
	m_Alpha = max(0, m_Alpha);

}

void DamageEffect::Render()
{
	const RECTI& rc = GAMEMATCHINFO.GetScreenSize();

	m_pScrTexture->Render(
		rc.x,
		rc.y,
		rc.width,
		rc.height,
		
		0,
		0,
		512,
		512,
		RS_COPY,
		D3DCOLOR_COLORVALUE(1, 0, 0, m_Alpha)
		);
}