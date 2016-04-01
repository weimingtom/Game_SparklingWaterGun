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
	//全チームのシェア率更新
	void UpdateAllTeamData(MapObjectManager::SHAREPARAM_ARRAY& array_share);
	//新たなチームデータを作成
	void CreateTeamData(const TeamUIParam& param);
	//既につくられているチームシェア率の更新
	void UpdateTeamData(TEAM_TYPE team, RATIO	ShareRatio);
	//ゲージの中心位置を取得
	bool GetTeamGaugePos(TEAM_TYPE team, Vector2& out);
	//シェアアップエフェクト
	void SetShareUpEffect(TEAM_TYPE team);
	//毎フレームの更新
	void Update();
	//結果発表の位置に移動
	void GoResultPos();
	//描画
	void Render(
		TEAM_TYPE you,		//描画する物のチーム番号(アイコン表示のため)
		TEAM_TYPE No1Team	//現在の一位チーム(演出を追加する)
		);
};

#endif