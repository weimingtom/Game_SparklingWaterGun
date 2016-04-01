#ifndef __DAMAGE_EFFECT_H__
#define __DAMAGE_EFFECT_H__

#include "iextreme.h"
#include "CODBOForwardDecl.h"

class DamageEffect
{
private:
	RATIO			m_Alpha;		//エフェクトアルファ
	float			m_Count;		//演出用カウント
	LPCHARACTER		m_pCharacter;	//持ち主キャラクタへのポインタ
	LPIEX2DOBJ		m_pScrTexture;	//画面エフェクトテクスチャ
public:
	DamageEffect(LPIEX2DOBJ		pScrTexture);
	~DamageEffect();
public:
	void SetDamageEffect(RATIO power);
	void SetOwnerCharacter(LPCHARACTER		m_Character);
	void Update();
	void Render();
};

#endif