#include "All.h"
#include <array>

int ColaWater::ApperTagCount = 0;
int ColaWater::UpdateTag = 0;
LVERTEX	ColaWater::lVertex[4];


ColaWater::ColaWater(
	const ColaWaterParam& param,
	int liveTime,
	LPIEXMESH pMesh):
	m_Updatetag(ApperTagCount),
	m_param(param),
	m_DeleteFlag(false),
	m_damage_obj((DamageObjectManager*)IDMANAGER.GetPointer(game_id::id_damage_manager)),
	m_live_time(liveTime)
{
	m_pEffect = POLYGONEFFECT.GetNewEffectPtr(EffectTex::_SOUR);
	m_effect_index = POLYGONEFFECT.SetEffect(m_pEffect);

	m_param.HitObjectId = game_id::id_error;
	m_damage_obj.Apper();
	m_damage_obj.param.color = m_param.color;
	m_damage_obj.SetPos(m_param.pos);

	GoalPosUpdate();

	ApperTagCount++;
	ApperTagCount %= UpdateFrame;

	m_pEffect->color = m_param.color;
	m_pEffect->dwFlags = RS_COPY;
	m_pEffect->count = 0;
	m_pEffect->pos = m_param.pos;
	m_pEffect->size = m_param.m_scale;

	m_DeleteFlag = HitCheck();
	if (m_damage_obj.param.hit_count)
	{
		m_DeleteFlag = true;

		GameMessage msg;

		msg.from = m_damage_obj.param.parent;
		msg.to = m_damage_obj.param.parent;
		msg.type = msg_t::hit_my_attack;

		IDMANAGER.Dispatch(&msg);

	}
	
}

ColaWater::~ColaWater()
{
	POLYGONEFFECT.EraceEffect(m_effect_index);
	delete m_pEffect;
}


bool ColaWater::HitCheck()
{
	static Vector3 temp;

	if (UpdateTag == m_Updatetag)
	{
		GoalPosUpdate();
	}
	float l;
	VECTOR3POINTTOPOINT(l, m_param.pos, m_param.m_goal_position, temp);

	if (l < 1)
	{
		if (rand() % 3 == 0)
		{	
			LPWATERPOOLMANAGER pool = (LPWATERPOOLMANAGER)IDMANAGER.GetPointer(game_id::id_water_pool_manager);
			pool->SetPool(
				m_param.m_goal_position + m_param.m_goal_normal*0.02f,
				m_param.m_goal_normal,
				//1.25f,
				0.6f,
				0,
				m_param.color,
				m_param.HitObjectId,
				2.0f
				);
		}/*
		else{

			POLYGONEFFECT.SetEffect(
				m_param.pos,
				Vector3Reflect(m_param.move*0.2f, m_param.m_goal_normal, 0.1f),
				m_param.power,
				m_param.m_scale,
				EffectTex::_SOUR,
				m_param.color,
				RS_COPY,
				rand() % 20 + 50);
		}*/

		return true;
	}
	return  m_param.m_live_timer >= m_live_time;
}

void ColaWater::GoalPosUpdate()
{
	static Vector3 out, pos, vec;
	static float dist;

	pos = m_param.pos;
	vec = m_param.move;
	vec.Normalize();
	dist = 100;

	MapObjectInterface* mapObj;

	if ((mapObj=MAP_MANAGER.RayPick(&out, &pos, &vec, &dist)))
	{
		m_param.m_goal_normal = vec;
		m_param.m_goal_normal.Normalize();
		m_param.HitObjectId = mapObj->GetId();
	}
	else{
		out = m_param.pos;
		out.x += 100;
	}
		
	m_param.m_goal_position = out;
}

void ColaWater::GetMoveAndPos(Vector3& move, Vector3& pos)
{
	move = m_param.move;
	pos = m_param.pos;
}

void ColaWater::AddUpdateTag()
{
	UpdateTag++;
	UpdateTag %= UpdateFrame;
}

bool ColaWater::isDelete()
{
	return m_DeleteFlag;
}

void ColaWater::Update()
{
	++m_param.m_live_timer;

	//消去フラグを更新
	if (!m_DeleteFlag)
	{
		m_DeleteFlag = HitCheck();
		if (m_damage_obj.param.hit_count)
		{
			m_DeleteFlag = true;

			GameMessage msg;
			
			msg.from = m_damage_obj.param.parent;
			msg.to = m_damage_obj.param.parent;
			msg.type = msg_t::hit_my_attack;

			IDMANAGER.Dispatch(&msg);

		}
	}
	
	//移動
	VECTOR3PLUS(m_param.pos, m_param.pos, m_param.move);

	//重力を加算
	VECTOR3PLUS(m_param.move, m_param.move, m_param.power);

	m_damage_obj.SetPos(m_param.pos);

	m_damage_obj.param.pos = m_param.pos;
	m_damage_obj.param.vec = m_param.move;

	m_pEffect->count = max(0, (m_param.m_live_timer + 16) - m_live_time);
	m_pEffect->pos = m_param.pos;
}

void ColaWater::Render()
{

}