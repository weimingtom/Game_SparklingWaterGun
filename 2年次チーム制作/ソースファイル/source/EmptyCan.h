#ifndef __EMPTY_CAN_H__
#define __EMPTY_CAN_H__

#include "ColaBullet.h"
#include "iextreme.h"
#include "ParentAndChild.h"


class EmptyCan :public BulletBase,public CHILD
{
private:
	LPIEXMESH			m_pMesh;
	LPIEX2DOBJ			m_pTexture;
	Matrix				m_Trans;
	Vector3				m_Move;
	const float			m_Size;
	const int			m_live_time;
	int					m_live_timer;
public:
	EmptyCan(
		LPIEXMESH			pMesh,
		LPIEX2DOBJ			pTexture,
		int					live_time,
		float				size,
		CRVector3			pos
		);
	~EmptyCan();
public:
	void ParentMove(
		LPCMATRIX pParentWorldMat,
		LPCMATRIX pParentWorldMatInv,
		LPCMATRIX pParentMoveMat,
		LPCMATRIX AffineMat)override;

	bool isDelete()override;
	void Update()override;
	void Render()override;
};

#endif