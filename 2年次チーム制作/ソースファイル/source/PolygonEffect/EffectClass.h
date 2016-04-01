#ifndef __EFFECT_CLASS_H__
#define __EFFECT_CLASS_H__

#include "AnimationEffect.h"
#include "iextreme.h"

//*************************************************************
//	コマ送りテクスチャエフェクトクラス
//*************************************************************
class AnimationTextureParam
{
public:
	int		w,	h,	maxSell;
	Vector2	sell_size;
public:
	inline void SetSellUV(TLVERTEX v[4], int sell_num)
	{
		v[0].tu = sell_size.x*(float)(sell_num % w);
		v[0].tv = sell_size.y*(float)(sell_num / w);

		v[1].tu = v[0].tu + sell_size.x;
		v[1].tv = v[0].tv;

		v[2].tu = v[0].tu;
		v[2].tv = v[0].tv + sell_size.y;

		v[3].tu = v[1].tu;
		v[3].tv = v[2].tv;
	}

	inline void SetSellUV(LVERTEX v[4], int sell_num)
	{
		v[0].tu = sell_size.x*(float)(sell_num % w);
		v[0].tv = sell_size.y*(float)(sell_num / w);

		v[1].tu = v[0].tu + sell_size.x;
		v[1].tv = v[0].tv;

		v[2].tu = v[0].tu;
		v[2].tv = v[0].tv + sell_size.y;

		v[3].tu = v[1].tu;
		v[3].tv = v[2].tv;
	}
};



class EffectClassAnimation:public AnimationEffect
{
private:
	LPIEX2DOBJ	texture;
	AnimationTextureParam tex_param;
	int			count;
	const int	max_count;
	float		size;
	Vector3		pos;
	Vector3		move;
	Vector3		power;
	DWORD		color;
	DWORD		dwFlags;
public:
	EffectClassAnimation(
		LPIEX2DOBJ	texture,
		const AnimationTextureParam& tex_param,
		float		size,
		CRVector3	pos,
		CRVector3	move,
		CRVector3	power,
		DWORD		color,
		DWORD		dwFlags,
		int			live_frame
		);
	~EffectClassAnimation();
public:
	void UpdateZ();
	bool Update();
	void Render(LPLVERTEX v);
};

//***************************************************************


class EffectClassAnimationNMove :public AnimationEffect
{
private:
	LPIEX2DOBJ	texture;
	AnimationTextureParam tex_param;
public:
	int			count;
	float		size;
	Vector3		pos;
	DWORD		color;
	DWORD		dwFlags;
public:
	EffectClassAnimationNMove(
		LPIEX2DOBJ	texture,
		const AnimationTextureParam& tex_param
		);
	~EffectClassAnimationNMove();
public:
	void UpdateZ();
	bool Update();
	void Render(LPLVERTEX v);
};


void MyRenderStateSet(DWORD state, Texture2D* lpTexture);


#endif