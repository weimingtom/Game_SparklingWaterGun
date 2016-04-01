#ifndef __CONTROLL_OBJECT_FUNCTION_H__
#define __CONTROLL_OBJECT_FUNCTION_H__

#include "ControllObject.h"


class  ControllObjFunc :private ControllObject
{
private:
	ControllObjFunc();
	~ControllObjFunc();
public:
	//������
	static void InitControllObjectParam(LPCONTROLLOBJECTPARAM pOut);
public:
	//�Q�b�^�[����
	static CRVector3 GetPos(LPCONTROLLOBJCT obj);
	static LPCAMERA GetCamera(LPCONTROLLOBJCT obj);
	static LPPLAYERMATCHDATA GetMatchData(LPCONTROLLOBJCT obj);
	static int GetTeamType(LPCONTROLLOBJCT obj);
	static TeamData* GetTeamData(LPCONTROLLOBJCT obj);
	static const ControllObjectParam& GetParam(LPCONTROLLOBJCT obj);
public:
	static void SetMatchDataPtr(LPCONTROLLOBJCT obj, LPPLAYERMATCHDATA data);
public:
	//�`��֌W
	static void SetBlur(LPCONTROLLOBJCT obj, float time, RATIO power, CRVector2 pos);
	static void ResetBlurParam(LPCONTROLLOBJCT obj);
	static void SetWaterDropParam(LPCONTROLLOBJCT obj,bool flg);
public:
	//�X�R�A�֘A
	static void AddKillScore(LPCONTROLLOBJCT addObj);
	static void AddDeathScore(LPCONTROLLOBJCT addObj);
	static void AddChangeShareObjScore(LPCONTROLLOBJCT addObj);
public:
	//���֘A
	static void PlayDamageHitSound(LPCONTROLLOBJCT to);
public:
	//�J�����֘A
	static void SetShockVecCamera(LPCONTROLLOBJCT obj, CRVector2 vec, RADIAN power);
};

#endif