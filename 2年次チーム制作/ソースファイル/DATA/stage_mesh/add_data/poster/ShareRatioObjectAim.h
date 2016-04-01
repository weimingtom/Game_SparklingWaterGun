#ifndef __SHARE_RATIO_OBJECT_AIM_H__
#define __SHARE_RATIO_OBJECT_AIM_H__

#include "../AimPoint.h"

class ShareRatioObjectBase;

class ShareRatioObjectAim :public AimTarget
{
private:
	static Vector3 tOut, tPos, tVec;
	static float tDist;
	ShareRatioObjectBase* m_pOwner;
public:
	ShareRatioObjectAim();
	~ShareRatioObjectAim();
public:
	void SetOwner(ShareRatioObjectBase* pOwner);
	bool isTarget(AimTargetInfo &out, CRAimOwnerInfo info);
};


#endif
