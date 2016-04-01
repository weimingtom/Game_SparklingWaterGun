#include "CharacterAim.h"
#include "Character.h"
#include "CharacterUtilityFunction.h"
#include "ColorUtility.h"
#include "ControllObjectFunction.h"
#include "map/MapObject.h"
#include "EffectResource.h"


Vector3 AimTargetCharacter::tOut;
Vector3 AimTargetCharacter::tPos;
Vector3 AimTargetCharacter::tVec;
float AimTargetCharacter::tDist;

AimTargetCharacter::AimTargetCharacter():
m_pOwner(nullptr),
m_Switch(true)
{

}

void AimTargetCharacter::SetEnable(bool flg)
{
	m_Switch = flg;
}

void AimTargetCharacter::SetOwner(Character*	pOwner)
{
	m_pOwner = pOwner;
}

bool AimTargetCharacter::isTarget(AimTargetInfo &out, CRAimOwnerInfo info)
{
	static const float	max_cir_size = 100;

	if (m_Switch == false || 
		m_pOwner == nullptr ||
		m_pOwner->GetId()==info.id||
		ChrFunc::isDead(m_pOwner))return false;

	const Vector3 pos = m_pOwner->GetBodyCenter();
	const Vector3& cpos = info.pCamera->GetPos();
	Vector3 src_pos;
	
	if (info.pCamera->ClientToScreen(src_pos, pos) == false		||
		src_pos.z - info.camera_z+0.3f	>	info.range			||
		src_pos.z						<	info.camera_z		
		||Vector3XYLength(src_pos)		>	info.circle_size
		)return false;

	Vector3 toTarget(pos-info.position);
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
	

	out.weight = Character_Aim_Weight_Enemy;
	out.color = color_smp::RED;
	out.team_type = ControllObjFunc::GetMatchData(m_pOwner)->team_type;

	if (out.team_type == info.team_type)
	{
		out.weight = Character_Aim_Weight_Team;
		out.color = color_smp::GREEN;
	}

	out.scr_pos.x = src_pos.x;
	out.scr_pos.y = src_pos.y;
	float l = 1-(Vector2Length(out.scr_pos)) / info.circle_size;
	out.weight *= l;
	out.id = myId;
	out.cli_pos = pos;
	out.cli_move = m_pOwner->GetFrameInfo().frame_move;//ChrFunc::GetParam(m_pOwner).m_move;
	out.entity_id = m_pOwner->GetId();

	out.size = max_cir_size*(info.range - pos.z);

	return true;
}