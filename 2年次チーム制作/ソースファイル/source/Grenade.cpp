#include "Grenade.h"
#include "GameSystem\GameIDManager.h"
#include "map\MapObject.h"
#include "CharacterMoveFunction.h"
#include "LuaLoadParam.h"
#include "CharacterWepon.h"
#include "WaterPool.h"
#include "GameMatchInfomation.h"


Grenade::Grenade(
	LPIEXMESH				pGrenadeMesh,
	CRVector3				pos,
	CRVector3				move,
	float					glavity,
	RATIO					power,
	float					mesh_size,
	const DamageParam&		damage_param,
	TEAM_TYPE				TeamType) :
	m_pGrenadeMesh(pGrenadeMesh),
	m_pos(pos),
	m_move(move),
	m_glavity(glavity),
	m_angle(0,0,0),
	m_size(mesh_size),
	m_isHit(false),
	m_power(power),
	m_TeamType(TeamType),
	m_DieTimer(0),
	m_damage_obj((DamageObjectManager*)IDMANAGER.GetPointer(game_id::id_damage_manager))
{
	m_damage_obj.param = damage_param;
	if (m_power > 0.01f)
	{
		m_damage_obj.Apper();
	}
	else{
		m_damage_obj.Exit();
	}
}

void Grenade::SmallExplode(ENTITY_ID rideID, CRVector3 normal)
{

	BulletManager* blt = (BulletManager*)IDMANAGER.GetPointer(game_id::id_bullet_manager);

	LPWATERPOOLMANAGER pool = (LPWATERPOOLMANAGER)IDMANAGER.GetPointer(game_id::id_water_pool_manager);

	m_damage_obj.Exit();

	pool->SetPool(
		m_pos + normal*0.02f,
		normal,
		0.6f,
		0,
		m_damage_obj.param.color,
		rideID,
		2
		);

	POLYGONEFFECT.SetEffect(
		m_pos,
		VECTOR3ZERO,
		VECTOR3ZERO,
		0.5f,
		EffectTex::_SMOKE,
		0xFFFFFFFF,
		RS_COPY
		);

	m_isHit = true;
}

void Grenade::Explode()
{
	m_damage_obj.Exit();
	m_damage_obj.param.hit_count = 0;
	m_damage_obj.param.type = damage_type_bits::team_cure_damage | damage_type_bits::blow_off_damage | damage_type_bits::enemy_damage;
	m_damage_obj.param.val = GetConstChrParam(nullptr)->max_hp*(m_power*0.6f);
	m_damage_obj.param.vec = m_move;

	ChrMove::SetExplositon(m_pos, m_power, m_damage_obj.param, 3.2f * m_power);
	
	m_isHit = true;
}


Grenade::~Grenade()
{

}

void Grenade::HitCheck()
{
	Vector3 out, pos(m_pos), vec(m_move);
	float dist = 1000;

	vec.Normalize();
	MapObjectInterface* hitObj;
	float ml;

	if (m_pos.y<-20)m_isHit = true;

	if (m_damage_obj.param.hit_count&&m_isHit == false)
	{
		if (m_power <= 0.01f)
		{
			POLYGONEFFECT.SetEffect(
				m_pos,
				VECTOR3ZERO,
				VECTOR3ZERO,
				0.5f,
				EffectTex::_SMOKE,
				0xFFFFFFFF,
				RS_COPY
				);
		}
		else{
			Explode();
		}
		m_isHit = true;
	}
	else if ((hitObj = MAP_MANAGER.RayPick(&out, &pos, &vec, &dist)))
	{
		VECTOR3POINTTOPOINT(dist, m_pos, out, pos);
		ml = m_move.Length();
		if (dist < ml)
		{
			if (m_power <= 0.01f)
			{
				vec.Normalize();
				m_pos = out;
				if (m_DieTimer == 0)m_DieTimer = 1;
				m_move = Vector3Reflect(m_move, vec, 0.3f);

				if (m_DieTimer>20)
				{
					SmallExplode(hitObj->GetId(), vec);
				}
			}
			else{
				Explode();
			}
		}
	}
}

void Grenade::GetMoveValue(
	Vector3&			out_move,
	CRVector3			start,
	CRVector3			goal,
	float				glavity
	)
{
	const float touch_frame = 40;

	Vector3 target = goal;

	target.y += (-glavity / 2)*(touch_frame*touch_frame);
	out_move = target - start;

	const float len = Vector3Length(out_move);
	float v = len / touch_frame;

	out_move.Normalize();
	out_move *= v;

}

bool Grenade::isDelete()
{
	return m_isHit || m_DieTimer > 120;
}


void Grenade::Update()
{
	if (m_DieTimer > 0)m_DieTimer++;
	m_angle.x += 0.2f;
	m_angle.z += 0.2f;
	m_pos += m_move;
	m_move.y += m_glavity;

	m_damage_obj.param.pos = m_pos;
	if (m_damage_obj.isEnable())
		m_damage_obj.SetPos(m_pos);
	m_damage_obj.param.vec = m_move;

	HitCheck();
}


void Grenade::Render()
{
	m_pGrenadeMesh->SetTexure(
		0,
		GAMEMATCHINFO.GetTextureMgr().GetPetTexturePtr(m_TeamType)->GetTexture()
		);

	m_pGrenadeMesh->SetPos(m_pos);
	m_pGrenadeMesh->SetScale(m_size);
	m_pGrenadeMesh->SetAngle(m_angle);
	m_pGrenadeMesh->Update();
	m_pGrenadeMesh->Render();

	m_pGrenadeMesh->SetTexure(0, nullptr);
}