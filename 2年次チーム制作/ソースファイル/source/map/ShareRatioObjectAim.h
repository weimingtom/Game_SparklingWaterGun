#ifndef __SHARE_RATIO_OBJECT_AIM_H__
#define __SHARE_RATIO_OBJECT_AIM_H__

#include "../AimPoint.h"

class ShareRatioObjectBase;

//*************************************************************
// シェアオブジェクト用エイムターゲットクラス
// AimTargetCharacterクラスを参考に作成
//*************************************************************
class ShareRatioObjectAim 
	:public AimTarget
{
private:
	static Vector3                target_out_;
	static Vector3                target_pos_; 
	static Vector3                target_vec_;
	static float                     target_dist_;
	ShareRatioObjectBase* owner_;
public:
	ShareRatioObjectAim();
	~ShareRatioObjectAim();
public:
	void SetOwner(ShareRatioObjectBase* pOwner);
	bool isTarget(AimTargetInfo &out, CRAimOwnerInfo info);
};


#endif
