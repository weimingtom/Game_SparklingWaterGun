#ifndef __CONTROLL_OBJECT_FUNCTION_H__
#define __CONTROLL_OBJECT_FUNCTION_H__

#include "ControllObject.h"


class  ControllObjFunc :private ControllObject
{
private:
	ControllObjFunc();
	~ControllObjFunc();
public:
	//初期化
	static void InitControllObjectParam(LPCONTROLLOBJECTPARAM pOut);
public:
	//ゲッターたち
	static CRVector3 GetPos(LPCONTROLLOBJCT obj);
	static LPCAMERA GetCamera(LPCONTROLLOBJCT obj);
	static LPPLAYERMATCHDATA GetMatchData(LPCONTROLLOBJCT obj);
	static int GetTeamType(LPCONTROLLOBJCT obj);
	static TeamData* GetTeamData(LPCONTROLLOBJCT obj);
	static const ControllObjectParam& GetParam(LPCONTROLLOBJCT obj);
public:
	static void SetMatchDataPtr(LPCONTROLLOBJCT obj, LPPLAYERMATCHDATA data);
public:
	//描画関係
	static void SetBlur(LPCONTROLLOBJCT obj, float time, RATIO power, CRVector2 pos);
	static void ResetBlurParam(LPCONTROLLOBJCT obj);
	static void SetWaterDropParam(LPCONTROLLOBJCT obj,bool flg);
public:
	//スコア関連
	static void AddKillScore(LPCONTROLLOBJCT addObj);
	static void AddDeathScore(LPCONTROLLOBJCT addObj);
	static void AddChangeShareObjScore(LPCONTROLLOBJCT addObj);
public:
	//音関連
	static void PlayDamageHitSound(LPCONTROLLOBJCT to);
public:
	//カメラ関連
	static void SetShockVecCamera(LPCONTROLLOBJCT obj, CRVector2 vec, RADIAN power);
};

#endif