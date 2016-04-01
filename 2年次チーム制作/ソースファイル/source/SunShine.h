#include "iextreme.h"

#ifndef __LENSFLARE_H__
#define __LENSFLARE_H__

#include "CODBOForwardDecl.h"


class SunShine
{
private:
	static const int LENSEFFECT = 8;
	iex2DObj	*lpObj;
	struct LensEffect{
		int		type;
		POINT	pos;
		float	scale;
		float	alpha;
	};
	LensEffect	data[LENSEFFECT];
	float		LensAlpha;
	bool		visible;		//	â¬éãÅEïsâ¬éã
	float		timer;
	float		center_len;
	Vector3		dirLightvec;
private:
	void	UpDate(LPCAMERA camera);
	bool	RayCheck(CRVector3 cpos, CRVector3 vec);
public:
	SunShine(const Vector3& LighDirVec);
	~SunShine();

	void Render(LPCAMERA camera);
	//	åvéZèàóù
	//float Dot( const Vector3& v1, const Vector3& v2 );
	Vector3 Normalize(Vector3& v);

};


#endif