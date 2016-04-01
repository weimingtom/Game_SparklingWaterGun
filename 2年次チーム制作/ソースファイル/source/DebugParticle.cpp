#include "DebugParticle.h"
//#include	"BillBoard.h"
#include "EffectResource.h"
LPIEX2DOBJ debug_particle::g_pNumberTexture = nullptr;

void debug_particle::point(CRVector3 pos, CRVector3 move, float size, DWORD color, int liveFrame)
{
	//float a, r, g, b;

	//COLORtoF(a, r, g, b, color);
	//
	//iexParticle::Set(
	//	0,

	//	0,
	//	1,

	//	liveFrame,
	//	1,

	//	1,
	//	1,
	//	
	//	&Vector3(pos),
	//	&Vector3(move),
	//	&Vector3(0,0,0),
	//	
	//	r,g,b,
	//	size,
	//	RS_COPY

	//	);

	return;
	POLYGONEFFECT.SetEffect(
		pos,
		move,
		VECTOR3ZERO,
		size,
		EffectTex::_EXPLODE
		);
}

void	debug_particle::circle(const Vector3& center_Pos, const float radius, const float&	dencity,const DWORD& color,const float& size)
{	
	float a, r, g, b;
	int end_Frame = 3;
	COLORtoF(a, r, g, b, color);
	Vector3 zero(.0f, .0f, .0f);
	for (float angle = .0f; angle <=(PI *2.0f); angle += dencity)
	{
		Vector3 pos;
		pos.x = center_Pos.x + sinf(angle) * radius;
		pos.y = center_Pos.y;
		pos.z = center_Pos.z + cosf(angle) * radius;
		iexParticle::Set(0, 1, 1, end_Frame, 1, 1, 1, &pos, &zero, &zero, r, g, b, size, RS_COPY);
	}
}


void debug_particle::NumberRender(CRVector3 pos, float size, int n, DWORD color )
{
#if _DEBUG 
	LVERTEX v[4];
	Vector3 
		f(matView._13, matView._23, matView._33),
		r(matView._11, matView._21, matView._31), 
		u(matView._12, matView._22, matView._32);

	VECTOR3NORMALIZE(f);
	VECTOR3NORMALIZE(r);
	VECTOR3NORMALIZE(u);

	VECTOR3MULSCALAR(f, f, size);
	VECTOR3MULSCALAR(r, r, size);
	VECTOR3MULSCALAR(u, u, size);

	v[0]._v = pos - r + u;
	v[1]._v = pos + r + u;
	v[2]._v = pos - r - u;
	v[3]._v = pos + r - u;

	//v[0].tu=n&0x1F

	const float us = 1.0f / 8.0f;
	const float vs = 1.0f / 8.0f;

	v[0].tu = us*(float)(n % 8);
	v[0].tv = vs*(float)(n / 8);

	v[1].tu = v[0].tu + us;
	v[1].tv = v[0].tv;

	v[2].tu = v[0].tu;
	v[2].tv = v[0].tv + vs;

	v[3].tu = v[0].tu + us;
	v[3].tv = v[0].tv + vs;

	for (int i = 0; i < 4; ++i)
	{
		v[i].color = color;
	}

	iexPolygon::Render3D(v, 2, g_pNumberTexture, RS_COPY);

#endif
}