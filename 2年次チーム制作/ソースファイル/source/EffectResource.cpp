#include "EffectResource.h"
#include <cassert>
#include "AI\CP11_Rand.h"
#include "DamageHit.h"
#include "GameSystem\GameIDManager.h"


PolygonEffect*PolygonEffect::m_pInstance = nullptr;


PolygonEffect::PolygonEffect():
m_EffectManager(1500)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		m_pTextures[i].m_pTexture = nullptr;
	}
}


PolygonEffect::~PolygonEffect()
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		delete m_pTextures[i].m_pTexture;
	}
}

TextureParameter& PolygonEffect::GetTexture(int effect_tex)
{
	return m_pTextures[effect_tex];
}

void PolygonEffect::UpdateBillboardVertex()
{
	Vector3
		right(matView._11, matView._21, matView._31),
		up(matView._12, matView._22, matView._32),
		front(matView._13, matView._23, matView._33);

	right.Normalize();
	up.Normalize();
	front.Normalize();

	ZeroMemory(m_vertex, sizeof(LVERTEX)* 4);

	m_vertex[0]._v = -right + up;
	m_vertex[1]._v = right + up;
	m_vertex[2]._v = -right - up;
	m_vertex[3]._v = right - up;
}

void PolygonEffect::GetBillboardVertex(LPLVERTEX vOut)
{
	for (int i = 0; i < 4; ++i)
	{
		vOut[i] = m_vertex[i];
	}
}

void PolygonEffect::GetAnimationBillboardVertex(LPLVERTEX v, int effect_tex, int dwTexFrame)
{
	GetBillboardVertex(v);
	const AnimationTextureParam& tex_param = m_pTextures[effect_tex].m_TexParam;

	v[0].tu = (float)(dwTexFrame%tex_param.w)*tex_param.sell_size.x;
	v[0].tv = (float)(dwTexFrame / tex_param.w)*tex_param.sell_size.y;

	v[1].tu = v[0].tu + tex_param.sell_size.x;
	v[1].tv = v[0].tv;

	v[2].tu = v[0].tu;
	v[2].tv = v[0].tv + tex_param.sell_size.y;

	v[3].tu = v[1].tu;
	v[3].tv = v[2].tv;
}

PolygonEffect& PolygonEffect::GetInstance()
{
	if (!m_pInstance)m_pInstance = new PolygonEffect;
	return *m_pInstance;
}

void PolygonEffect::Release()
{
	delete m_pInstance;
	m_pInstance = nullptr;
}

bool PolygonEffect::SetTexture(
	int index,
	LPIEX2DOBJ texture,
	int width,
	int height,
	int max_sell)
{
	if (m_pTextures[index].m_pTexture)return false;

	m_pTextures[index].m_pTexture = texture;

	m_pTextures[index].m_TexParam.w = width;
	m_pTextures[index].m_TexParam.h = height;
	m_pTextures[index].m_TexParam.maxSell = max_sell;

	m_pTextures[index].m_TexParam.sell_size.x = 1.0f / (float)width;
	m_pTextures[index].m_TexParam.sell_size.y = 1.0f / (float)height;

	return true;
}


PolygonEffect::INDEX PolygonEffect::SetEffect(LPANIMEEFFECT effect)
{
	return m_EffectManager.SetEffect(effect);
}


PolygonEffect::INDEX PolygonEffect::SetEffect(
	CRVector3 pos,
	CRVector3 move,
	CRVector3 power,
	float size,
	int texture_index,
	DWORD color ,
	DWORD dwFlags,
	int	live_frame //= TEXTURE_LIVE_FRAME_CELLNUM
	)
{
	assert(m_pTextures[texture_index].m_pTexture&&"テクスチャが登録されていない状態でエフェクトが呼び出されました");
	
	LPANIMEEFFECT e = new EffectClassAnimation(
		m_pTextures[texture_index].m_pTexture,
		m_pTextures[texture_index].m_TexParam,
		size,
		pos,
		move,
		power,
		color,
		dwFlags,
		(live_frame == TEXTURE_LIVE_FRAME_CELLNUM) ? (m_pTextures[texture_index].m_TexParam.maxSell):(live_frame)
		);

	INDEX ret = m_EffectManager.SetEffect(e);

	if (ret == -1)
	{
		delete e;
	}
	return ret;
}

PolygonEffect::INDEX PolygonEffect::SetDamageEffect(
	CRVector3 pos,
	CRVector3 move,
	CRVector3 power,
	float size,
	int texture_index,
	int damage_livetime,
	float damage_size,
	DamageObject* dmg,
	DWORD color ,
	DWORD dwFlags ,
	int	live_frame 
	)
{
	assert(m_pTextures[texture_index].m_pTexture&&"テクスチャが登録されていない状態でエフェクトが呼び出されました");

	LPANIMEEFFECT e = new DamageEffectAnimation(
		m_pTextures[texture_index].m_pTexture,
		m_pTextures[texture_index].m_TexParam,
		size,
		pos,
		move,
		power,
		color,
		dwFlags,
		(live_frame == TEXTURE_LIVE_FRAME_CELLNUM) ? (m_pTextures[texture_index].m_TexParam.maxSell) : (live_frame),
		damage_livetime,
		damage_size,
		dmg
		);

	INDEX ret = m_EffectManager.SetEffect(e);

	if (ret == -1)
	{
		delete e;
	}
	return ret;
}

PolygonEffect::INDEX PolygonEffect::SetSourEffect(
	CRVector3 pos,
	float size,
	float speed,
	int vn_frame,
	DWORD color ,
	DWORD dwFlags 
	)
{
	assert(m_pTextures[EffectTex::_SOUR].m_pTexture&&"泡テクスチャが登録されていない状態でエフェクトが呼び出されました");

	LPANIMEEFFECT e = new SourEffect(
		m_pTextures[EffectTex::_SOUR].m_pTexture,
		m_pTextures[EffectTex::_SOUR].m_TexParam,
		size,
		pos,
		speed,
		vn_frame,
		color,
		dwFlags);

	INDEX ret = m_EffectManager.SetEffect(e);

	if (ret == -1)
	{
		delete e;
	}
	return ret;
}

#define MAKEPATH(path) "DATA\\Effect\\Animation\\"##path

EffectClassAnimationNMove* PolygonEffect::GetNewEffectPtr(int texture_index)
{
	return new EffectClassAnimationNMove(
		m_pTextures[texture_index].m_pTexture,
		m_pTextures[texture_index].m_TexParam
		);
}

void PolygonEffect::EraceEffect(INDEX index)
{
	m_EffectManager.EraceEffect(index);
}

void PolygonEffect::LoadEffectTextureData()
{
	using namespace EffectTex;

	SetTexture(_EXPLODE, new iex2DObj(MAKEPATH("explode.png")),  8, 8, 64 );
	SetTexture(_KAMIHUBUKI, new iex2DObj(MAKEPATH("kamifubuki.png")),  13, 1, 13 );
	SetTexture(_COLA_HIT, new iex2DObj(MAKEPATH("HitEffect.png")),  4,4,16 );
	SetTexture(_SOUR, new iex2DObj(MAKEPATH("Explosion.png")),  4, 4, 16 );
	SetTexture(_SPLASH, new iex2DObj(MAKEPATH("Pushaa.png")), 4, 8, 32);
	SetTexture(_DUST, new iex2DObj(MAKEPATH("sand.png")),  5, 6, 30 );
	SetTexture(_SMOKE, new iex2DObj(MAKEPATH("Smoke.png")), 6, 5, 30);
	SetTexture(_STAR, new iex2DObj(MAKEPATH("kirakira.png")), 8, 8, 64);
	SetTexture(_SOUR_MANY, new iex2DObj(MAKEPATH("SourEffect.png")), 8, 8, 64);
	
}

void PolygonEffect::Update()
{
	m_EffectManager.Update();
}

void PolygonEffect::Render()
{
	m_EffectManager.Render();
}




//*******************************************************************************

SourEffect::SourEffect(
	LPIEX2DOBJ	texture,
	const AnimationTextureParam& tex_param,
	float		size,
	CRVector3	pos,
	float		speed,
	int			vn_frame,
	DWORD		color,
	DWORD		dwFlags
	) :
	texture(texture),
	tex_param(tex_param),
	size(size),
	pos(pos),
	color(color),
	dwFlags(dwFlags),
	count(0),
	speed(speed),
	vn_frame(vn_frame),
	move_vec(((float)rand() / RAND_MAX) - 0.5f, 0.01f, ((float)rand() / RAND_MAX) - 0.5f),
	seed(rand()%30)
{
}

SourEffect::~SourEffect()
{

}

void SourEffect::UpdateZ()
{
	const Vector3& v = pos;
	const Matrix& m = matView;
	m_sort_z = v.x*m._13 + v.y*m._23 + v.z*m._33 + m._43;
}

bool SourEffect::Update()
{
	const float s = sinf(static_cast<float>(count + seed)*speed)*0.1f;

	pos.x += move_vec.x*s;
	pos.z += move_vec.z*s;
	pos.y += move_vec.y;

	return ++count < vn_frame;
}

void SourEffect::Render(LPLVERTEX v)
{
	POLYGONEFFECT.GetBillboardVertex(v);

	for (int i = 0; i < 4; ++i)
	{
		v[i].color = color;

		v[i]._v.x *= size;
		v[i]._v.y *= size;
		v[i]._v.z *= size;

		v[i]._v.x += pos.x;
		v[i]._v.y += pos.y;
		v[i]._v.z += pos.z;
	}

	int dwTexFrame = tex_param.maxSell - (vn_frame - count);
	dwTexFrame = max(dwTexFrame, 0);
	
	v[0].tu = (float)(dwTexFrame%tex_param.w)*tex_param.sell_size.x;
	v[0].tv = (float)(dwTexFrame / tex_param.w)*tex_param.sell_size.y;

	v[1].tu = v[0].tu + tex_param.sell_size.x;
	v[1].tv = v[0].tv;

	v[2].tu = v[0].tu;
	v[2].tv = v[0].tv + tex_param.sell_size.y;

	v[3].tu = v[1].tu;
	v[3].tv = v[2].tv;

	MyRenderStateSet(dwFlags & 0x0F, texture->GetTexture());
	//	レンダリング
	iexSystem::Device->SetFVF(D3DFVF_LVERTEX);
	iexSystem::Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(LVERTEX));

}

///************************************************************************************

DamageEffectAnimation::DamageEffectAnimation(
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
	) :
	texture(texture),
	tex_param(tex_param),
	size(size),
	pos(pos),
	move(move),
	power(power),
	color(color),
	dwFlags(dwFlags),
	count(0),
	max_count(live_frame),
	m_pDamageObj(damage_obj),
	damage_livetime(damage_livetime)
{
	m_pDamageObj->SetPos(pos);
	m_pDamageObj->SetSize(damage_size);
	m_pDamageObj->Apper();
}

DamageEffectAnimation::~DamageEffectAnimation()
{
	delete m_pDamageObj;
}

bool DamageEffectAnimation::Update()
{
	pos += move;
	move += power;

	m_pDamageObj->SetPos(pos);

	if (m_pDamageObj->isEnable() &&
		(m_pDamageObj->param.hit_count > 0 || count >= damage_livetime))
	{
		m_pDamageObj->Exit();
	}

	return ++count < max_count;
}

void DamageEffectAnimation::Render(LPLVERTEX v)
{
	POLYGONEFFECT.GetBillboardVertex(v);

	for (int i = 0; i < 4; ++i)
	{
		v[i].color = color;

		v[i]._v.x *= size;
		v[i]._v.y *= size;
		v[i]._v.z *= size;

		v[i]._v.x += pos.x;
		v[i]._v.y += pos.y;
		v[i]._v.z += pos.z;
	}

	int dwTexFrame = min(count, tex_param.maxSell);

	v[0].tu = (float)(dwTexFrame%tex_param.w)*tex_param.sell_size.x;
	v[0].tv = (float)(dwTexFrame / tex_param.w)*tex_param.sell_size.y;

	v[1].tu = v[0].tu + tex_param.sell_size.x;
	v[1].tv = v[0].tv;

	v[2].tu = v[0].tu;
	v[2].tv = v[0].tv + tex_param.sell_size.y;

	v[3].tu = v[1].tu;
	v[3].tv = v[2].tv;

	MyRenderStateSet(dwFlags & 0x0F, texture->GetTexture());
	//	レンダリング
	iexSystem::Device->SetFVF(D3DFVF_LVERTEX);
	iexSystem::Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(LVERTEX));

}


void DamageEffectAnimation::UpdateZ()
{
	const Vector3& v = pos;
	const Matrix& m = matView;
	m_sort_z = v.x*m._13 + v.y*m._23 + v.z*m._33 + m._43;
}
