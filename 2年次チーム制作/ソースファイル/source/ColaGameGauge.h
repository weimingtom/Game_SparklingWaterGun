#ifndef __COLA_GAME_GAUGE_H__
#define __COLA_GAME_GAUGE_H__

#include "iextreme.h"
#include "CODBOForwardDecl.h"
#include "map\MapObject.h"
#include <map>

class ColaShareGauge
{
public:
	struct TeamUIParam
	{
		TEAM_TYPE		m_Type;
		LPIEX2DOBJ		m_pLogoTex;
		COLOR			m_Color;
		RATIO			m_TargetShareRatio;
		RATIO			m_NowShareRatio;
		RATIO			m_NowRenderRatio;
		RATIO			m_ShareUpAlpha;
		bool			m_Update;
	};
private:
	typedef std::map<TEAM_TYPE, TeamUIParam> TeamUIParamMap;
	typedef std::map<TEAM_TYPE, TeamUIParam*> TeamUIParamPtrMap;
	enum TextureType
	{
		GAUGE_BAR = 0, 
		FIRE_ANIME,
		MINI_CHR,
		PARCENT_NUMBER,
		CURSOR,
		_TEXMAX
	};
	enum MoveState
	{
		USUAL,
		RESULT,
	};
private:
	static const int	m_TeamMax = 4;
	LPIEX2DOBJ			m_pTextures[TextureType::_TEXMAX];
	MoveState			m_NowState;
	Vector2				m_GaugeCenters[m_TeamMax];
	TeamUIParamMap		m_DataMap;
	const RATIO			m_ChangeSpeed;
	int					m_AnimationCount;
	bool				m_GameFirst;
	const Vector2		m_UsualGaugeSize, m_UsualGaugePos;
	Vector2				m_GaugeSize, m_GaugePos;
public:
	ColaShareGauge();
	~ColaShareGauge();
private:
	void GaugeMove();
	void ParcentRender(RATIO value, int x, int y, int sizeX, int sizeY);
	void SetRenderRatio();
	void GaugeRender(int scr_w, int scr_h, TEAM_TYPE you, TEAM_TYPE No1Team);
public:
	//�S�`�[���̃V�F�A���X�V
	void UpdateAllTeamData(MapObjectManager::SHAREPARAM_ARRAY& array_share);
	//�V���ȃ`�[���f�[�^���쐬
	void CreateTeamData(const TeamUIParam& param);
	//���ɂ����Ă���`�[���V�F�A���̍X�V
	void UpdateTeamData(TEAM_TYPE team, RATIO	ShareRatio);
	//�Q�[�W�̒��S�ʒu���擾
	bool GetTeamGaugePos(TEAM_TYPE team, Vector2& out);
	//�V�F�A�A�b�v�G�t�F�N�g
	void SetShareUpEffect(TEAM_TYPE team);
	//���t���[���̍X�V
	void Update();
	//���ʔ��\�̈ʒu�Ɉړ�
	void GoResultPos();
	//�`��
	void Render(
		TEAM_TYPE you,		//�`�悷�镨�̃`�[���ԍ�(�A�C�R���\���̂���)
		TEAM_TYPE No1Team	//���݂̈�ʃ`�[��(���o��ǉ�����)
		);
};

#endif