#include "EffectClass.h"
#include "../EffectResource.h"

void MyRenderStateSet(DWORD state, Texture2D* lpTexture)
{
	switch (state){
	case RS_COPY:
		iexSystem::Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		iexSystem::Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		break;

	case RS_ADD:	//	加算合成
		iexSystem::Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		iexSystem::Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		break;
	}

	//	テクスチャ設定
	iexSystem::Device->SetTexture(0, lpTexture);
}

EffectClassAnimation::EffectClassAnimation(
	LPIEX2DOBJ	texture,
	const AnimationTextureParam& tex_param,
	float		size,
	CRVector3	pos,
	CRVector3	move,
	CRVector3	power,
	DWORD		color,
	DWORD		dwFlags,
	int			live_frame
	):
	texture(texture),
	tex_param(tex_param),
	size(size),
	pos(pos),
	move(move),
	power(power),
	color(color),
	dwFlags(dwFlags),
	count(0),
	max_count(live_frame)
{
}

EffectClassAnimation::~EffectClassAnimation()
{

}

void EffectClassAnimation::UpdateZ()
{
	const Vector3& v = pos;
	const Matrix& m = matView;
	m_sort_z = v.x*m._13 + v.y*m._23 + v.z*m._33 + m._43;
}

bool EffectClassAnimation::Update()
{
	pos += move;
	move += power;
	
	return ++count < max_count;
}

void EffectClassAnimation::Render(LPLVERTEX v)
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

	int dwTexFrame = max(count - (max_count - tex_param.maxSell), 0);

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


//******************************************************************


EffectClassAnimationNMove::EffectClassAnimationNMove(
	LPIEX2DOBJ	texture,
	const AnimationTextureParam& tex_param
	) :
	texture(texture),
	tex_param(tex_param),
	size(size),
	pos(pos),
	color(color),
	dwFlags(dwFlags),
	count(0)
{
}

EffectClassAnimationNMove::~EffectClassAnimationNMove()
{

}

void EffectClassAnimationNMove::UpdateZ()
{
	const Vector3& v = pos;
	const Matrix& m = matView;
	m_sort_z = v.x*m._13 + v.y*m._23 + v.z*m._33 + m._43;
}

bool EffectClassAnimationNMove::Update()
{
	return true;
}

void EffectClassAnimationNMove::Render(LPLVERTEX v)
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

