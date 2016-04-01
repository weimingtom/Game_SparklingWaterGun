

#include "ShareRatioObjectAim.h"

#include "MapObject.h"

#include "ShareRatioObject.h"

#include "CharacterAim.h"

Vector3 ShareRatioObjectAim::tOut;
Vector3 ShareRatioObjectAim::tPos;
Vector3 ShareRatioObjectAim::tVec;
float ShareRatioObjectAim::tDist;

ShareRatioObjectAim::ShareRatioObjectAim() :
m_pOwner(nullptr)
{

}

ShareRatioObjectAim::~ShareRatioObjectAim()
{

}

void ShareRatioObjectAim::SetOwner(ShareRatioObjectBase* pOwner)
{
	m_pOwner = pOwner;
}

bool ShareRatioObjectAim::isTarget(AimTargetInfo &out, CRAimOwnerInfo info)
{
	static const float	max_cir_size = 100;

	if (m_pOwner == nullptr ||
		m_pOwner->GetId() == info.id)return false;

	const Vector3 pos = m_pOwner->GetParam().pos;
	const Vector3& cpos = info.pCamera->GetPos();
	Vector3 src_pos;

	if (info.pCamera->ClientToScreen(src_pos, pos) == false ||
		src_pos.z - info.camera_z + 0.3f	>	info.range ||
		src_pos.z						<	info.camera_z
		|| Vector3XYLength(src_pos)		>	info.circle_size
		)return false;

	Vector3 toTarget(pos - info.position);
	toTarget.Normalize();

	if (Vector3Dot(toTarget, info.view_vec) < 0.5f)return false;

	float camera_to_target;
	VECTOR3POINTTOPOINT(camera_to_target, pos, cpos, tVec);

	tPos = cpos;
	tDist = 1000;

	if (MAP_MANAGER.RayPick(&tOut, &tPos, &tVec, &tDist))
	{
		VECTOR3POINTTOPOINT(tDist, tOut, cpos, tVec);

		if (tDist < camera_to_target)
			return false;
	}

	out.color = color_smp::RED;

	if (m_pOwner->GetShareParam().team_type == info.team_type)
	{
		out.color = color_smp::GREEN;
	}

	out.weight = Character_Aim_Weight_Object;
	out.scr_pos.x = src_pos.x;
	out.scr_pos.y = src_pos.y;
	out.id = myId;
	out.cli_pos = pos;
	VECTOR3TOZERO(out.cli_move);

	out.size = max_cir_size*(info.range - pos.z);

	return true;
}
