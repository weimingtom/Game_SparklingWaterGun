#ifndef __SHARE_RATIO_OBJECT_AIM_H__
#define __SHARE_RATIO_OBJECT_AIM_H__

#include "../AimPoint.h"

class ShareRatioObjectBase;

//*************************************************************
// �V�F�A�I�u�W�F�N�g�p�G�C���^�[�Q�b�g�N���X
// AimTargetCharacter�N���X���Q�l�ɍ쐬
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
