#ifndef __COLA_GAUGE_H__
#define __COLA_GAUGE_H__

#include "iextreme.h"
#include "stencil\StencilClip.h"

class ColaGauge
{
private:
	enum :int
	{
		cola_vertex_num = 28
	};
private:
	enum COLA_GAUGE_STATE
	{
		_APPER,
		_GAUGE,
		_ROLLING,
	};
private:
	COLA_GAUGE_STATE	m_state;
	LPIEX2DOBJ			m_pCola;
	LPIEX2DOBJ			m_pColaClip;
	LPIEX2DOBJ			m_pColaSour;
	RATIO				m_Power;
	bool				m_isShuffle;
	int					m_count;
	RATIO				m_PowerUpSpeed;
	RATIO				m_PowerDownSpeed;
	RATIO				m_gauge_shuffle_speed;
	Vector2				m_pos, m_size, default_size;
	Vector2				m_default_position;
	StencilClip			m_StencilClip;
	TLVERTEX			m_GaugeVertex[cola_vertex_num];
	DWORD				m_color;
	float				m_sour_count;
	const RATIO			m_enable_danger_effect_power;
	float				m_danger_effect_count;
	float				m_sour_speed;
	bool				m_max_lighting;
	int					m_SoundTimer;
	const Vector2		m_default_position_save;
	struct
	{
		Vector2		size;
		Vector2		scr_pos;
		RATIO		speed;
	}
	RollingParam;
public:
	ColaGauge(
		RATIO			PowerUpSpeed,
		RATIO			PowerDownSpeed,
		CRVector2		default_position
		);
	~ColaGauge();
private:
	void SetnextState(COLA_GAUGE_STATE	state);
private:
	void ShuffleState_Apper();
	void ShuffleState_Gauge();
	void ShuffleState_Rolling();
private:
	void RenderDangerEffect(RATIO t, CRVector2 size, RATIO alpha);
	void SetColaVertex(CRVector2 scale);
public:
	void UpdateSound(CRVector3 pos);
	void SetPowerUpSpeed(RATIO p);
	void SetColor(DWORD	 color);
	bool isShuffle()const;
	RATIO GetPower()const;
	void AddPower(RATIO p);
	void SetShuffleFlg(bool flg);					//シャッフルするかどうかをセット
	void Reset();
	void GoGauge(CRVector2 scr_pos, CRVector2 scr_size, RATIO speed);	//ゲージを画面引数の画面位置に引数のスピードで移動する
	void Update();					//毎フレームの更新
	void Render();									//描画
};

#endif