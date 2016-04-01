#ifndef __SHARE_RATIO_OBJECT_H__
#define __SHARE_RATIO_OBJECT_H__

#include "ShareRatioObjectAim.h"
#include "MapObject.h"
#include "../DamageHit.h"
#include "MapObjectParam.h"

static const RATIO SHARE_RATIO_MAX = 500.0f;

class ShareRatioObjectBase :public MapObjectInterface
{
protected:
	ShareRatioParam			m_ShareRatio;
	ShareRatioObjectAim		m_AimTarget;
	float					colli_size;
public:
	ShareRatioObjectBase(
		ENTITY_ID id,
		ObjectParam param,
		LPIEXMESH mesh,
		MAP_OBJ_TYPE type);
	virtual~ShareRatioObjectBase();
public:
	void Update();
	virtual const ShareRatioParam& GetShareParam()const = 0;

public:
	DAMAGEFLAGS HitCheck();

	virtual void Hit(DamageObject* p) = 0;

	bool RayPick(
		Vector3* out,
		Vector3* pos,
		Vector3*vec,
		float* dist
		)
	{
		return false;
	}
};


class PosterShareRatioObject :public ShareRatioObjectBase
{
public:
	PosterShareRatioObject(
		ENTITY_ID id,
		ObjectParam param,
		LPIEXMESH mesh,
		MAP_OBJ_TYPE type);
	~PosterShareRatioObject();
public:
	const ShareRatioParam& GetShareParam()const;
	void Hit(DamageObject* p);

	void ParentMove(
		LPCMATRIX pParentWorldMat,
		LPCMATRIX pParentWorldMatInv,
		LPCMATRIX pParentMoveMat,
		LPCMATRIX AffineMat);
};


#endif