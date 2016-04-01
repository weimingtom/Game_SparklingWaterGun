
#include "ShareRatioObjectAim.h"

#include "MapObject.h"

#include "ShareRatioObject.h"

#include "../CharacterAim.h"

//*************************************************************
// シェアオブジェクト用エイムターゲットクラス
// AimTargetCharacterクラスを参考に作成
//*************************************************************

Vector3 ShareRatioObjectAim::target_out_;
Vector3 ShareRatioObjectAim::target_pos_;
Vector3 ShareRatioObjectAim::target_vec_;
float ShareRatioObjectAim::target_dist_;

ShareRatioObjectAim::ShareRatioObjectAim() :
owner_(nullptr)
{

}


ShareRatioObjectAim::~ShareRatioObjectAim()
{

}


void ShareRatioObjectAim::SetOwner(ShareRatioObjectBase* pOwner)
{
	owner_ = pOwner;
}


bool ShareRatioObjectAim::isTarget(AimTargetInfo &out, CRAimOwnerInfo info)
{
	static const float	max_cir_size = 100;

	if (owner_ == nullptr ||
		owner_->Get_ShareParam().team_type == info.team_type ||
		owner_->GetId() == info.id)return false;

	const Vector3 pos = owner_->Get_DamagePos();
	const Vector3& cpos = info.pCamera->GetPos();
	Vector3 src_pos;
	MapObjectInterface* pHitObj = nullptr;

	if (info.pCamera->ClientToScreen(src_pos, pos) == false ||
		src_pos.z - info.camera_z + 0.3f	>	info.range ||
		src_pos.z						<	info.camera_z
		|| Vector3XYLength(src_pos)		>	info.circle_size
		)return false;

	Vector3 toTarget(pos - info.position);
	toTarget.Normalize();

	if (Vector3Dot(toTarget, info.view_vec) < 0.5f)return false;

	float camera_to_target;
	VECTOR3POINTTOPOINT(camera_to_target, pos, cpos, target_vec_);

	target_pos_ = cpos;
	target_dist_ = 1000;

	if ((pHitObj = MAP_MANAGER.RayPick(&target_out_, &target_pos_, &target_vec_, &target_dist_)) &&
		pHitObj != owner_)
	{
		VECTOR3POINTTOPOINT(target_dist_, target_out_, cpos, target_vec_);

		if (target_dist_ < camera_to_target)
			return false;
	}

	out.color = color_smp::RED;

	if (owner_->Get_ShareParam().team_type == info.team_type)
	{
		out.color = color_smp::GREEN;
	}

	out.weight = Character_Aim_Weight_Object*(1 - Vector3XYLength(src_pos));
	out.scr_pos.x = src_pos.x;
	out.scr_pos.y = src_pos.y;
	out.id = myId;
	out.cli_pos = pos;
	out.cli_move = owner_->Get_MoveParameter();
	out.team_type = -1;
	out.size = max_cir_size*(info.range - pos.z);
	out.entity_id = owner_->GetId();

	return true;
}

// EOF