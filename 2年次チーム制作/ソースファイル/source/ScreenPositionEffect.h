#ifndef __SCREEN_POSITION_EFFECT_H__
#define __SCREEN_POSITION_EFFECT_H__

#include "iextreme.h"
#include "EffectResource.h"
#include <list>

//********************************************************
//	ダメージと音を表示するエフェクト
//********************************************************
class PositionEffect
{
public:
	struct AnimeEffectParam
	{
		Vector2		pos;
		RADIAN		angle;
		Vector2		size;
		int			time;
		int			max_time;
		COLOR		color;
		int			texture_type;
	};
private:
	class AnimationEffect
	{
	private:
		AnimeEffectParam	m_param;
	public:
		AnimationEffect();
		AnimationEffect(
			const AnimeEffectParam&	param
			);
		~AnimationEffect(){ }
	public:
		void Update();
		void Render(
			char*				shader_name,
			LVERTEX			VertexArray[4],
			TextureParameter&	Anime_tex,
			LPIEX2DOBJ*			Position_tex
			);
		float GetZ()const;
		BOOL isEnable()const;
	};
private:
	static const int	m_maxAnime = 20;
	static const int	m_PositionTex = 3;
	TextureParameter	m_Animation_texparam;
	LPIEX2DOBJ			m_pPosition_tex[m_PositionTex];
	AnimationEffect		m_AnimeList[m_maxAnime];
	int					m_Set_index_count;
	const float			m_EffectCircleSize;
public:
	PositionEffect();
	~PositionEffect();
private:
	void ScrPosEffectUpdate();
	void WaterDropUpdate();
public:
	/**
	*@param1 ダメージの飛んできた方向(xz)
	*@param2 視線ベクトル(xz)
	*@return texture空間の座標(xy)
	*/
	void CalcScrPos(CRVector2 in_vec, CRVector2 view_vec, Vector2& out_vec);
	void SetEffect(const AnimeEffectParam& param);
public:
	void Update();
	void Render();
};


#endif