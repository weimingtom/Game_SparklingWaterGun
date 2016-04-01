#include "All.h"


ColaGun::ColaGun(
	const WeaponParam&	p,
	LPCHARACTER			pOwner
	):
	m_param(p),
	m_pOwner(pOwner),
	m_count(0),
	m_func(nullptr),
	m_shot_effect_count(0),
	m_Sound_timer(0),
	m_NotFire_timer(0)
{

}

ColaGun::~ColaGun()
{

}

void ColaGun::InitWeponParam(WeaponParam& out)
{
//	out.damage_min = 0.5f;
//	out.damage_max = 1.5f;

	out.rate = 1;

	if (GAMEMATCHINFO.GetMatchData().player_num > 2)
	{
		out.rate=2;
	}
#if 0
	out.damage_min = 1 * (float)out.rate;
	out.damage_max = 2 * (float)out.rate;
#else
	out.damage_min = 1.8f * (float)out.rate;
	out.damage_max = 1.8f * (float)out.rate;
#endif


	out.range_max = 0.9f;
	out.range_min = 0.07f;
	
	out.power = (RATIO)0.1f;
}

bool ColaGun::UsualColaFactory(
	ColaWater**ppOut, 
	LPCHARACTER pOwner,
	const WeaponParam& wp)
{
	ColaWaterParam cwp;
	LPCHARACTER_UI pUI = ChrFunc::GetUI(pOwner);
	Vector3	GunPos;
	float length;
	float move_gauge_power = wp.power / 0.5f;
	move_gauge_power = min(move_gauge_power, 1);
	float move_power = fLERP(wp.range_min, wp.range_max, move_gauge_power);
	Vector3 v1, v2, axis;

	pUI->GetTargetPos(v1);
	pUI->GetTargetMove(v2);

	//GunPos = pOwner->GetBodyCenter() + Vector3Rand()*0.1f + Vector3Normalize(ChrFunc::GetParam(pOwner).m_view_vec)*0.2f;
	ChrFunc::GetBonePos(pOwner, chr_bone_type::RIGHT_ARM_WEAPON, GunPos);

	length = Vector3Length(v1 - GunPos) / move_power;

	v2 = (v1 + v2*length) - GunPos;
	v1 -= GunPos;
	v2.y = v1.y;
	v1.Normalize(); v2.Normalize();

	VECTOR3CROSS(axis,v1,v2);

	if (Vector3isZero(axis))
	{
		cwp.move = v1;
	}
	else{

		float angle = acosf(VECTOR3DOT(v1, v2));
		float rotate = fLERP(0, PI / 3, move_gauge_power);

		if (angle > rotate)
			angle = rotate;
		if (angle > 0.01f)
			cwp.move = Vector3RotateAxis(axis, angle, v1);
		else
			cwp.move = v1;
	}

	cwp.move.Normalize();

#ifdef _DEBUG_WEPON
	cwp.move *= 0.6f;
#else
	cwp.move *= move_power;
#endif
	
	cwp.move += ChrFunc::GetParam(pOwner).m_move * (1 - move_gauge_power);
	cwp.pos = GunPos;

	cwp.power = Vector3(0, -0.001f, 0); //Vector3(0, -0.002f, 0);
	cwp.m_scale = fRAND(0.02f, 0.1f);
	cwp.m_live_timer = 0;
	cwp.color = CODMgr::GetTeamColaWaterColor(ControllObjFunc::GetMatchData(pOwner)->team_type);

	*ppOut = new ColaWater(
		cwp,
		65,
		GAMEMATCHINFO.GetMeshMgr().GetMeshPtr((MeshManager::MESH_ID)GameMatchInfomation::GAME_MESH_ID::_COLA_WATER)
		);

	(*ppOut)->m_damage_obj.param.parent = pOwner->GetId();
	(*ppOut)->m_damage_obj.param.type = damage_type_bits::team_cure_damage | damage_type_bits::except_myself | damage_type_bits::enemy_damage;
	(*ppOut)->m_damage_obj.SetSize(0.1f);
	(*ppOut)->m_damage_obj.param.val = fLERP(wp.damage_min, wp.damage_max, wp.power);

	return true;
}


void ColaGun::ParamColaFactory(
	ColaWater**ppOut,
	CRVector3 pos,
	DWORD color,
	CRVector3 move,
	float dmg,
	ENTITY_ID id,
	DAMAGEFLAGS dFlg
	)
{
	ColaWaterParam cwp;
	cwp.move = move;
	cwp.pos = pos;
	cwp.power = Vector3(0, -0.002f, 0);
	cwp.m_scale = fRAND(0.02f, 0.1f);
	cwp.m_live_timer = 0;
	cwp.color = color;

	*ppOut = new ColaWater(
		cwp,
		36,
		GAMEMATCHINFO.GetMeshMgr().GetMeshPtr((MeshManager::MESH_ID)GameMatchInfomation::GAME_MESH_ID::_COLA_WATER)
		);

	(*ppOut)->m_damage_obj.param.parent = id;
	(*ppOut)->m_damage_obj.param.type = dFlg; 
	(*ppOut)->m_damage_obj.SetSize(0.1f);
	(*ppOut)->m_damage_obj.param.val = dmg;

}


RATIO ColaGun::GetWeponChargePower(LPCHARACTER pOwner,float toEnemyLength)
{
	const WeaponParam& wp = ChrFunc::GetWepon(pOwner)->GetParam();
	RATIO ret;
	float Max, Min;
	Max = wp.range_max * 60;
	Min = wp.range_min * 60;

	toEnemyLength -= Min;
	ret = toEnemyLength / (Max-Min);

	ret = max(0, ret);
	ret = min(1, ret);

	return min(static_cast<RATIO>(sinf(ret))+0.2f,1);
}

bool ColaGun::Fire()
{
	m_NotFire_timer = 0;

	ChrMove::AddShuffleGauge(m_pOwner, -GetConstChrParam(m_pOwner)->cola_gauge_fire_down_speed*m_param.rate);

	if (m_count < m_param.rate)return false;
	


	ColaWater* pWater;

	MyAssert(m_func&&m_func(&pWater, m_pOwner,m_param), "ƒR[ƒ‰’e¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");

	Vector3 pos, move, rv, ans, glavity(0, -0.002f, 0);
	float mPower;

	pWater->GetMoveAndPos(move, pos);
	mPower = move.Length()*1.2f;
	pos += Vector3Normalize(move)*0.45f;

	if (rand()%3==0)
	{
		if (mPower > 0.0f)
		{
			VECTOR3DIVSCALAR(move, move, mPower);

			for (int i = 0; i < 2; i++)
			{
				rv.x = fRAND(-0.15f, 0.15f);
				rv.y = fRAND(-0.15f, 0.15f);
				rv.z = fRAND(-0.15f, 0.15f);

				VECTOR3PLUS(ans, move, rv);
				VECTOR3NORMALIZE(ans);
				ans *= mPower;

				POLYGONEFFECT.SetEffect(
					pos,
					ans,
					glavity,
					0.03f,
					EffectTex::_SOUR,
					CODMgr::GetTeamColaWaterColor(ControllObjFunc::GetMatchData(m_pOwner)->team_type),
					RS_COPY,
					40
					);
			}
		}
	}

	if (m_shot_effect_count <= 0)
	{
		m_shot_effect_count = 2;

		pos += Vector3Rand()*0.02f;

		POLYGONEFFECT.SetEffect(
			pos,
			ChrFunc::GetParam(m_pOwner).m_move,
			VECTOR3ZERO,
			0.45f,
			EffectTex::_SPLASH,
			CODMgr::GetTeamColaWaterColor(ControllObjFunc::GetMatchData(m_pOwner)->team_type),
			RS_COPY
			);
	}

	GameMessage msg;

	msg.exinfo = pWater;
	msg.from = m_pOwner->GetId();
	msg.to = game_id::id_bullet_manager;
	msg.type = msg_t::add_bullet;

	IDMANAGER.Dispatch(&msg);

	m_count = 0;
	

	/*
	SOUNDPLAYER.PlaySE(
		sound_t::shot_weapon,
		SOUNDPLAYER.GetSoundVolume(pos, sound_t::shot_weapon)
		);
		*/

	return true;
}

void ColaGun::Update()
{
	if (m_count < m_param.rate)++m_count;

	if (m_NotFire_timer++ <= 0)
	{
		m_Sound_timer++;
	}
	else m_Sound_timer = 0;

	if (m_Sound_timer == 1)
	{
		SOUNDPLAYER.PlaySE(
			sound_t::shot_weapon_begin,
			SOUNDPLAYER.GetSoundVolume(m_pOwner->GetBodyCenter(), sound_t::shot_weapon_begin)
			);
	}

	if (m_Sound_timer % 10 == 5)
	{
		SOUNDPLAYER.PlaySE(
			sound_t::shot_weapon,
			SOUNDPLAYER.GetSoundVolume(m_pOwner->GetBodyCenter(), sound_t::shot_weapon)
			);
	}
	m_shot_effect_count = max(m_shot_effect_count - 1, 0);
	
}

void ColaGun::SetPower(RATIO val)

{
	m_param.power = val;
}

void ColaGun::SetColawaterFactory(COLA_FACTORY_FUNC func)
{
	m_func = func;
}

const WeaponParam& ColaGun::GetParam()const
{
	return m_param;
}

void ColaGun::Render(TEAM_TYPE team, bool shuffle)
{
	LPIEXMESH pMesh = GAMEMATCHINFO.GetMeshMgr().GetMeshPtr((MeshManager::MESH_ID)GameMatchInfomation::_COLA_GUN);
	Matrix mat = ChrFunc::GetModelPtr(m_pOwner)->GetBoneMatrix(chr_bone_type::RIGHT_ARM_WEAPON);
	Matrix R;
	Matrix& tr = pMesh->TransMatrix;

	if (!shuffle)
	{
		D3DXMatrixRotationY(&R, -PI_d2);
		mat *= ChrFunc::GetModelPtr(m_pOwner)->GetMatrix();
		tr = R*mat;
	}
	else
	{
		D3DXMatrixRotationX(&R, PI);
		mat *= ChrFunc::GetModelPtr(m_pOwner)->GetMatrix();
		tr = R*mat;
	}
	shader->SetValue("OutlineSize", 0.03f);
	pMesh->SetTexure(0, GAMEMATCHINFO.GetTextureMgr().GetPetTexturePtr(team)->GetTexture());
	pMesh->Render(shader,"character");
	pMesh->SetTexure(0, nullptr);
	//m_mes
	
}