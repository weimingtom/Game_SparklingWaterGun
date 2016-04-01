#ifndef __DAMAGE_EFFECT_H__
#define __DAMAGE_EFFECT_H__

#include "iextreme.h"
#include "CODBOForwardDecl.h"

class DamageEffect
{
private:
	RATIO			m_Alpha;		//�G�t�F�N�g�A���t�@
	float			m_Count;		//���o�p�J�E���g
	LPCHARACTER		m_pCharacter;	//������L�����N�^�ւ̃|�C���^
	LPIEX2DOBJ		m_pScrTexture;	//��ʃG�t�F�N�g�e�N�X�`��
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