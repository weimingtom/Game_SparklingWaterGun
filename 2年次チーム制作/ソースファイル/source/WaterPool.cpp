#include "WaterPool.h"
#include "GameSystem\GameIDManager.h"
#include "map/MapObject.h"
#include "EffectResource.h"
#include "system\System.h"

void WaterPoolManager::WaterPool::ParentMove(
	LPCMATRIX pParentWorldMat,
	LPCMATRIX pParentWorldMatInv,
	LPCMATRIX pParentMoveMat,
	LPCMATRIX AffineMat)
{
	static Matrix m;
	m = *pParentMoveMat;

	m._41 = m._42 = m._43 = 0.0f;

	pos=Vector3MulMatrix(pos, *AffineMat);

	normal=Vector3MulMatrix(normal, m);
	front = Vector3MulMatrix(front, m);
	right = Vector3MulMatrix(right, m);
}

//******************************************
//		コーラ水たまりマネージャ
//******************************************

WaterPoolManager::WaterPoolManager():
GameBaseEntity(game_id::id_water_pool_manager),
m_water_texture("DATA\\TEXTURE\\UI\\damage1.png"),
m_IndexCount(0)
{
	for (int i = 0; i < _DATA_SIZE; ++i)
	{
		m_WaterPool[i].angle = 0;
		m_WaterPool[i].color = 0;
		m_WaterPool[i].parent_id = game_id::id_error;
		VECTOR3TOZERO(m_WaterPool[i].normal);
		VECTOR3TOZERO(m_WaterPool[i].front);
		VECTOR3TOZERO(m_WaterPool[i].right);
		VECTOR3TOZERO(m_WaterPool[i].pos);
		m_WaterPool[i].t = FLT_MAX;
		m_WaterPool[i].target_size = 0;
	}
}

WaterPoolManager::~WaterPoolManager()
{

}

void WaterPoolManager::SetPool(
	CRVector3 pos,
	CRVector3 normal,
	float size,
	float angle,
	DWORD color,
	ENTITY_ID id,
	float t)
{
	for (int i = 0; i < _DATA_SIZE; ++i)
	{
		if (m_WaterPool[m_IndexCount].t <=100)
		{
			m_IndexCount = (m_IndexCount + 1) % _DATA_SIZE;
			continue;
		}
		m_WaterPool[m_IndexCount].angle = angle;
		m_WaterPool[m_IndexCount].color = color;
		m_WaterPool[m_IndexCount].normal = normal;
		m_WaterPool[m_IndexCount].pos = pos;
		m_WaterPool[m_IndexCount].t = t;
		m_WaterPool[m_IndexCount].target_size = size;
		m_WaterPool[m_IndexCount].parent_id = id;

		Vector3Cross(m_WaterPool[m_IndexCount].front, VECTOR3AXISX, m_WaterPool[i].normal);
		if (Vector3isZero(m_WaterPool[m_IndexCount].front))
		{
			Vector3Cross(m_WaterPool[m_IndexCount].front, VECTOR3AXISZ, m_WaterPool[i].normal);
		}
		Vector3Cross(m_WaterPool[m_IndexCount].right, m_WaterPool[m_IndexCount].front, m_WaterPool[i].normal);

		VECTOR3NORMALIZE(m_WaterPool[m_IndexCount].right);
		VECTOR3NORMALIZE(m_WaterPool[m_IndexCount].front);

		if (id != game_id::id_error)
		{
			MapObjectInterface* obj = (MapObjectInterface*)IDMANAGER.GetPointer(id);
			if (obj)obj->AddChild(&m_WaterPool[m_IndexCount]);
		}
		
		m_IndexCount = (m_IndexCount + 1) % _DATA_SIZE;
		return;
	}
}

void WaterPoolManager::Update()
{
	const float speed = 0.01f;
	const float speedx2 = speed*2.0f;
	float size;
	MapObjectInterface*	obj;

	for (int i = 0; i < _DATA_SIZE; ++i)
	{
		if (m_WaterPool[i].t > PI&&m_WaterPool[i].t < PI+1)
		{
			m_WaterPool[i].t = FLT_MAX;
			obj = (MapObjectInterface*)IDMANAGER.GetPointer(m_WaterPool[i].parent_id);
			if (!obj)continue;
			obj->ReleaseChild(&m_WaterPool[i]);

			continue;
		}

		if (m_WaterPool[i].t < 2.0f)
		{
			if(rand()%40==0)//if (rand() % 200 <= (int)(m_WaterPool[i].t * 10))
			{
				size = sinf(m_WaterPool[i].t)*m_WaterPool[i].target_size / 2.0f;

				POLYGONEFFECT.SetSourEffect(
					m_WaterPool[i].pos + m_WaterPool[i].front*fRAND(-size, size) + m_WaterPool[i].right*fRAND(-size, size), 
					fRAND(0.03f, 0.1f),
					fRAND(0.006f, 0.025f),
					36 + rand() % 10,
					m_WaterPool[i].color,
					RS_COPY);
			}
			m_WaterPool[i].t += speedx2;
		}
		else 
			m_WaterPool[i].t += speed;
	}
}

void WaterPoolManager::Render()
{
	LVERTEX v[4];
	float size;

	v[0].tu = .0f;
	v[0].tv = .0f;

	v[1].tu = 1.0f;
	v[1].tv = .0f;

	v[2].tu = .0f;
	v[2].tv = 1.0f;

	v[3].tu = 1.0f;
	v[3].tv = 1.0f;

	COLOR color;
	RATIO t;

	for (int i = 0; i < _DATA_SIZE; ++i)
	{
		if (m_WaterPool[i].t>PI)continue;

		//size = m_WaterPool[i].target_size;
		t = (m_WaterPool[i].t > 2.54f) ? (1 - (m_WaterPool[i].t - 2.54f) * 2) : 1;
		t = max(0, t);
		size = t*m_WaterPool[i].target_size;
	
		Vector3Cross(m_WaterPool[i].front, VECTOR3AXISX, m_WaterPool[i].normal);
		if (Vector3isZero(m_WaterPool[i].front))
		{
			Vector3Cross(m_WaterPool[i].front, VECTOR3AXISZ, m_WaterPool[i].normal);
		}
		Vector3Cross(m_WaterPool[i].right, m_WaterPool[i].front, m_WaterPool[i].normal);

		VECTOR3NORMALIZE(m_WaterPool[i].right);
		VECTOR3NORMALIZE(m_WaterPool[i].front);

		color = (m_WaterPool[i].color & 0x00FFFFFF) |// (0xFF000000);
			((DWORD)(t * 255) << 24);
			//(DWORD(((PI_d2-max(m_WaterPool[i].t - PI_d2, 0)) / PI_d2) * 255) << 24);

		for (int j = 0; j < 4; ++j)
			v[j].color = color;

		v[0]._v = m_WaterPool[i].pos + (m_WaterPool[i].front + m_WaterPool[i].right)*size;
		v[1]._v = m_WaterPool[i].pos + (m_WaterPool[i].front - m_WaterPool[i].right)*size;
		v[2]._v = m_WaterPool[i].pos + (m_WaterPool[i].right - m_WaterPool[i].front)*size;
		v[3]._v = m_WaterPool[i].pos + (-m_WaterPool[i].right - m_WaterPool[i].front)*size;

		iexPolygon::Render3D(v, 2, &m_water_texture,shader,"cola_pool");
	//	iexPolygon::Render3D(v, 2, &m_water_texture,RS_ADD);
	}
}

bool WaterPoolManager::HandleMessage(LPGameMessage msg)
{
	return false;
}