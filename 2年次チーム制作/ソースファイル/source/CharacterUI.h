#ifndef __CHARACTER_UI_H__
#define __CHARACTER_UI_H__

#include "UserInterfaceBase.h"
#include "AimPoint.h"
#include "CODBOForwardDecl.h"
#include "ScreenPositionEffect.h"
#include "SunShine.h"
#include "DamageEffect.h"
#include "screen\Screen_Distortion.h"

//********************************************************
//	キャラクタのUI関連
//********************************************************




//********************************************************
//	キャラクタのUI関連をまとめたクラス
//********************************************************
class CharacterUI:public UserInterfaceBase
{
private:
	LPIEX2DOBJ			m_pAimTexture;
	AimPoint*			m_pAim_point;
	LPCOLAGAUGE			m_pColaGauge;
	LPCHARACTER			m_pOwner;
	LPPOSITION_EFFECT	m_pPositionEffect;
	LPSTARTCOUNTDOWN	m_pStartCountDown;
	SunShine			m_SunShine;
	DamageEffect		m_ScrDamageEffect;
public:
	CharacterUI();
	~CharacterUI();
private:
	void RenderScore();
	void RenderIcon(ControllObject& viewowner, TeamData& teamdata);
	void RenderTelop(ControllObject& viewowner, TeamData& teamdata);
	void RenderTimer();
	void CreateAimOwnerParam(AimOwnerInfo& out);
public:
	bool GetTargetID(AIMTARGET_ID& out);
	void GetTargetPos(Vector3& out);
	void GetTargetMove(Vector3& out);
	bool GetTarget(AimTargetInfo& out);
	RATIO GetColaGaugePower()const;
	bool isShuffle()const;
public:
	void SetOwner(LPCHARACTER p);
	void SetChangeTargetFlg(bool flg);
public:
	void ResetGauge();
	void SetGaugeMove(CRVector2 pos, CRVector2 size);
	void SetColaGaugeColor(DWORD color);
	void SetReaction(float power);
	void SetDamageEffect(PositionEffect::AnimeEffectParam& param,bool isDamage=true);
	void SetShuffleFlg(bool flg);
	void AddColaGauge(RATIO p);
	void Initialize();
	void Update();
	void Render(ControllObject& viewowner, TeamData& teamdata);
};



#endif