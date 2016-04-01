#ifndef __GRENADE_H__
#define __GRENADE_H__

#include "ColaBullet.h"
#include "DamageHit.h"

class Grenade :public BulletBase
{
private:
	LPIEXMESH			m_pGrenadeMesh;
	TEAM_TYPE			m_TeamType;
	Vector3				m_pos, m_move, m_angle;
	const float			m_glavity;
	const float			m_size;
	DamageObject		m_damage_obj;
	bool				m_isHit;
	RATIO				m_power;
	int					m_DieTimer;
public:
	Grenade(
		LPIEXMESH				pGrenadeMesh,
		CRVector3				pos,
		CRVector3				move,
		float					glavity,
		RATIO					power,
		float					mesh_size,
		const DamageParam&		damage_param,
		TEAM_TYPE				TeamType
		);
	~Grenade();
private:
	void SmallExplode(ENTITY_ID rideID, CRVector3 normal);
	void Explode();
	void HitCheck();
public:
	static void GetMoveValue(
		Vector3&			out_move,
		CRVector3			start,
		CRVector3			goal,
		float				glavity
		);
public:
	bool isDelete();
	void Update();
	void Render();
};

#endif