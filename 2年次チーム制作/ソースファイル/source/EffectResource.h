#ifndef __EFFECT_RESOURCE_H__
#define __EFFECT_RESOURCE_H__

#include "PolygonEffect\EffectClass.h"
#include "CODBOForwardDecl.h"

namespace EffectTex
{
	enum
	{
		_EXPLODE,
		_KAMIHUBUKI,
		_COLA_HIT,
		_SOUR,
		_SOUR_MANY,
		_SPLASH,
		_DUST,
		_SMOKE,
		_STAR,
	};
}

struct TextureParameter
{
	LPIEX2DOBJ				m_pTexture;
	AnimationTextureParam	m_TexParam;
};


class PolygonEffect
{
public:
	typedef int INDEX;
private:

	enum
	{
		TEXTURE_MAX = 10,
		TEXTURE_LIVE_FRAME_CELLNUM=INT_MAX,	//マス目の数のフレームだけ生存
	};
private:
	static PolygonEffect*	m_pInstance;
	AnimationEffectManager	m_EffectManager;
	TextureParameter		m_pTextures[TEXTURE_MAX];
	LVERTEX					m_vertex[4];
private:
	PolygonEffect();
	~PolygonEffect();
public:
	static PolygonEffect& GetInstance();
	static void Release();
public:
	TextureParameter& GetTexture(int effect_tex);
	void UpdateBillboardVertex();
	void GetBillboardVertex(LPLVERTEX vOut);
	void GetAnimationBillboardVertex(LPLVERTEX vOut,int effect_tex,int sell_num);

	bool SetTexture(
		int index,
		LPIEX2DOBJ texture,
		int width,
		int height,
		int max_sell);

	INDEX SetEffect(LPANIMEEFFECT effect);
	INDEX SetEffect(
		CRVector3 pos,
		CRVector3 move,
		CRVector3 power,
		float size,
		int texture_index,
		DWORD color=0xFFFFFFFF,
		DWORD dwFlags = RS_ADD,
		int	live_frame = TEXTURE_LIVE_FRAME_CELLNUM
		);
	INDEX SetDamageEffect(
		CRVector3 pos,
		CRVector3 move,
		CRVector3 power,
		float size,
		int texture_index,
		int damage_livetime,
		float damage_size,
		DamageObject* dmg,
		DWORD color = 0xFFFFFFFF,
		DWORD dwFlags = RS_ADD,
		int	live_frame = TEXTURE_LIVE_FRAME_CELLNUM
		);
	INDEX SetSourEffect(
		CRVector3 pos,
		float size,
		float speed,
		int vn_frame,
		DWORD color = 0xFFFFFFFF,
		DWORD dwFlags = RS_ADD
		);
	EffectClassAnimationNMove* GetNewEffectPtr(int texture_index);
	void EraceEffect(INDEX index);
public:
	void LoadEffectTextureData();
public:
	void Update();
	void Render();
};

#define POLYGONEFFECT (PolygonEffect::GetInstance())




class SourEffect :public AnimationEffect
{
private:
	typedef const Vector3& CRVector3;
private:
	LPIEX2DOBJ				texture;
	AnimationTextureParam	tex_param;
	int						count;
	const int				vn_frame,seed;
	float					size;
	Vector3					pos;
	const Vector3			move_vec;
	const float				speed;
	DWORD					color;
	DWORD					dwFlags;
public:
	SourEffect(
		LPIEX2DOBJ	texture,
		const AnimationTextureParam& tex_param,
		float		size,
		CRVector3	pos,
		float		speed,
		int			vn_frame,
		DWORD		color,
		DWORD		dwFlags
		);
	~SourEffect();
public:
	void UpdateZ();
	bool Update();
	void Render(LPLVERTEX v);
};

//*********************************************************************

class DamageEffectAnimation :public AnimationEffect
{
private:
	typedef const Vector3& CRVector3;
private:
	DamageObject*	m_pDamageObj;
	LPIEX2DOBJ		texture;
	AnimationTextureParam tex_param;
	int				count;
	const int		max_count;
	const int		damage_livetime;
	float			size;
	Vector3			pos;
	Vector3			move;
	Vector3			power;
	DWORD			color;
	DWORD			dwFlags;
public:
	DamageEffectAnimation(
		LPIEX2DOBJ	texture,
		const AnimationTextureParam& tex_param,
		float		size,
		CRVector3	pos,
		CRVector3	move,
		CRVector3	power,
		DWORD		color,
		DWORD		dwFlags,
		int			live_frame,
		int			damage_livetime,
		float		damage_size,
		DamageObject*damage_obj
		);
	~DamageEffectAnimation();
public:
	void UpdateZ();
	bool Update();
	void Render(LPLVERTEX v);
};



#endif