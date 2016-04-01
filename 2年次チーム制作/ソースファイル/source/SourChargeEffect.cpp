#include "SourChargeEffect.h"
#include "ControllObject.h"
#include "ControllObjectFunction.h"
#include "GameMatchInfomation.h"
#include "ScoreCalcManager.h"

SourChargeEffect::SourChargeEffect(
	LPIEX2DOBJ				pTexture,
	CRVector2				pos,
	CRVector2				size,
	CRVector2				move,
	CRVector2				target,
	float					speed,
	float					MaxSpeed,
	TEAM_TYPE				TeamType,
	ENTITY_ID				OwnerControllObjId
	):
	m_pTexture(pTexture),
	m_Pos(pos),
	m_Size(size),
	m_Move(move),
	m_Target(target),
	m_Speed(speed),
	m_MaxSpeed(MaxSpeed),
	m_TeamType(TeamType),
	m_OwnerControllObjId(OwnerControllObjId),
	m_LiveTimer(0)
{

	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	m_Size.x /= (float)w;
	m_Size.y /= (float)h;

	m_Pos.x /= (float)w;
	m_Pos.y /= (float)h;

	m_Move.x /= (float)w;
	m_Move.y /= (float)h;

	m_Target.x /= (float)w;
	m_Target.y /= (float)h;

}

SourChargeEffect::~SourChargeEffect()
{

}

void SourChargeEffect::SendTeamGaugeLightingMsg()
{
	((ScoreCalcManager*)IDMANAGER.GetPointer(game_id::id_score_calc_manager))->SetShareUpEffect(m_TeamType);
}

bool SourChargeEffect::Update()
{
	//m_Target�Ɍ������Ĉړ�����
	const Vector2 v = m_Target - m_Pos;
	const Vector2 nv = Vector2Normalize(v);
	const float vl = Vector2Length(v);

	//�ړ��ʍX�V
	m_Move += nv*m_Speed;
	m_LiveTimer += 0.05f;

	const float ml = Vector2Length(m_Move);

	//�ő呬�x��葁����΍ő呬�x�ɂ���
	if (ml > m_MaxSpeed)
	{
		m_Move = Vector2Normalize(m_Move);
		m_Move *= m_MaxSpeed;
	}

	if (m_LiveTimer > 1)
	{
		m_Move = nv*ml;
	}

	//�ړ�
	m_Pos += m_Move;

	//�ڕW�Ɖ������true��Ԃ�
	if (vl > m_MaxSpeed*1.1f)
	{
		return true;
	}

	SendTeamGaugeLightingMsg();

	return  false;
}

void SourChargeEffect::Render(
	LPCONTROLLOBJCT		pViewOwner,
	LPTEAMDATA			pTeamData
	)
{
	if (pViewOwner->GetId() != m_OwnerControllObjId)return;

	m_pTexture->Render(
		(int)(m_Pos.x - m_Size.x / 2),
		(int)(m_Pos.y - m_Size.y / 2),
		(int)m_Size.x,
		(int)m_Size.y,
		0,
		0,
		64,
		64,
		RS_COPY,
		CODMgr::GetTeamColaWaterColor(m_TeamType)
		);

}