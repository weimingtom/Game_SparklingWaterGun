#ifndef __WATER_POOL_H__
#define __WATER_POOL_H__

#include "iextreme.h"
#include "GameSystem\GameBase.h"
#include "ManagerBase.h"
#include "ParentAndChild.h"

//******************************************
//		コーラ水たまりマネージャ
//******************************************

class WaterPoolManager :public GameBaseEntity
{
private:
	class WaterPool:public CHILD
	{
	public:
		DWORD		color;
		Vector3		pos, normal, right, front;
		float		target_size;
		float		angle;
		float		t;
		ENTITY_ID	parent_id;
	public:
		void ParentMove(
			LPCMATRIX pParentWorldMat,
			LPCMATRIX pParentWorldMatInv,
			LPCMATRIX pParentMoveMat,
			LPCMATRIX AffineMat);
	};
private:
	enum { _DATA_SIZE = 500 };
	int				m_IndexCount;
	iex2DObj		m_water_texture;
	WaterPool		m_WaterPool[_DATA_SIZE];
public:
	WaterPoolManager();
	~WaterPoolManager();
public:
	void SetPool(
		CRVector3 pos,
		CRVector3 normal,
		float size,
		float angle,
		DWORD color,
		ENTITY_ID id,
		float t = 0);
	void Update();
	void Render();
	bool HandleMessage(LPGameMessage msg);
};

#endif