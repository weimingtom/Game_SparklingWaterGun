#ifndef __OBJECT_AIM_TARGET_H__
#define __OBJECT_AIM_TARGET_H__

#include "AimPoint.h"

//***********************************************************
//	ステージ上のオブジェクトが持つエイムターゲットクラス
//***********************************************************

class ObjectAimTarget: public AimTarget
{
public:
	typedef bool(*isCanAimCharacter)(CRAimOwnerInfo info);	//キャラクタによって狙うかどうかを判断する関数
private:
	Vector3				position;	//エイムされる場所
	isCanAimCharacter	exp;		//キャラクタによって狙うかどうかを判断する関数
public:
	ObjectAimTarget();
	~ObjectAimTarget();
private:
	bool isTarget(AimTargetInfo &out, CRAimOwnerInfo info);	//情報を得る (ターゲットでない場合はfalseを返す)
public:
	void SetPos(CRVector3 pos);
	void SetExp(isCanAimCharacter exp);	//キャラクタによって狙うかどうかを判断する関数をセット(デフォルトではすべてのキャラクタが狙える)
};

#endif