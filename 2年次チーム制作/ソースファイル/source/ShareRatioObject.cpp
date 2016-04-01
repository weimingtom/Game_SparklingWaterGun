#include "GameSystem\GameIDManager.h"
#include "DamageHit.h"

#include "AimPoint.h"
#include "ShareRatioObjectAim.h"

#include "MapObject.h"

#include "ShareRatioObject.h"
#include "ControllObjectFunction.h"

#include "../MeshManager.h"


ShareRatioObjectBase::ShareRatioObjectBase(
	ENTITY_ID id,
	ObjectParam param,
	LPIEXMESH mesh,
	MAP_OBJ_TYPE type
	) :
	MapObjectInterface(id, param, mesh, type)
{
	m_ShareRatio.share_power = 0.0f;
	m_ShareRatio.team_type = 0;

	m_AimTarget.SetOwner(this);

	colli_size = 1.0f;
}

ShareRatioObjectBase::~ShareRatioObjectBase()
{

}


void ShareRatioObjectBase::Update()
{
	HitCheck();
	MapObjectInterface::Update();
}


DAMAGEFLAGS ShareRatioObjectBase::HitCheck()
{
	DamageObjectManager* damage = (DamageObjectManager*)IDMANAGER.GetPointer(game_id::id_damage_manager);
	if (!damage)return (DAMAGEFLAGS)0;

	DAMAGEVECTOR damage_array;
	damage_array = damage->GetInDamage(m_param.pos, 1.0f);

	int i = (int)damage_array.size();

	if (i <= 0)return (DAMAGEFLAGS)0;
	--i;

	DAMAGEFLAGS ret = 0x0;

	for (DamageObject** first = &damage_array.at(0); i >= 0; --i)
	{
		if (VECTOR3LENGTH((first[i]->param.pos - m_param.pos)) < (colli_size + first[i]->GetSize()))
		{
			//‚ ‚½‚Á‚½Žž‚Ìˆ—‘‚­
			Hit(first[i]);
		}
	}
	return ret;
}




//**********************************************
// test
//**********************************************
PosterShareRatioObject::PosterShareRatioObject(
	ENTITY_ID id,
	ObjectParam param,
	LPIEXMESH mesh,
	MAP_OBJ_TYPE type) :
	ShareRatioObjectBase(id, param, mesh, type)
{
	colli_size = 1.0f;
}

PosterShareRatioObject::~PosterShareRatioObject()
{

}

const ShareRatioParam& PosterShareRatioObject::GetShareParam()const
{
	return m_ShareRatio;
}


void PosterShareRatioObject::Hit(DamageObject* p)
{
	float val =  p->param.val;

	LPCONTROLLOBJCT pOwner=nullptr;
	p->param.hit_count++;
	if (!IS_CONTROLLOBJ_ID(p->param.parent))return;

	
	pOwner = (LPCONTROLLOBJCT)IDMANAGER.GetPointer(p->param.parent);
	if (!pOwner)return;

	int team_type = ControllObjFunc::GetMatchData(pOwner)->team_type;

	if (m_ShareRatio.team_type == team_type)
		m_ShareRatio.share_power += val;
	else
		m_ShareRatio.share_power -= val;

	if (m_ShareRatio.share_power > SHARE_RATIO_MAX){
		m_ShareRatio.share_power = SHARE_RATIO_MAX;
	}

	if (m_ShareRatio.share_power < 0.0f)
	{
		m_ShareRatio.share_power = 0.0f;
		m_ShareRatio.team_type = (m_ShareRatio.team_type ? 0 : 1);

		//ƒƒbƒVƒ…•ÏX
		MeshManager* mesh_mgr = nullptr;
		MAP_MANAGER.GetMeshManager(&mesh_mgr);
		if (mesh_mgr)
		{
			m_pMesh = mesh_mgr->GetMeshPtr((MeshManager::MESH_ID)(m_ShareRatio.team_type + 5));
		}
	}
}


void PosterShareRatioObject::ParentMove(
	LPCMATRIX pParentWorldMat,
	LPCMATRIX pParentWorldMatInv,
	LPCMATRIX pParentMoveMat,
	LPCMATRIX AffineMat
	)
{
	Vector3 new_position = Vector3MulMatrix(m_param.pos, *AffineMat);
	m_param.pos = new_position;
}
