#ifndef __CHARACTER_AIM_H__
#define __CHARACTER_AIM_H__

#include "AimPoint.h"


static const float Character_Aim_Weight_Enemy = 3.0f;
static const float Character_Aim_Weight_Team = 3.0f;
static const float Character_Aim_Weight_Object = 1.0f;

class Character;

class AimTargetCharacter :public AimTarget
{
private:
	static Vector3 tOut, tPos, tVec;
	static float tDist;
	bool		m_Switch;
	Character*	m_pOwner;
public:
	AimTargetCharacter();
public:
	void SetEnable(bool flg);
	void SetOwner(Character*	pOwner);
	bool isTarget(AimTargetInfo &out, CRAimOwnerInfo info);
};


#endif