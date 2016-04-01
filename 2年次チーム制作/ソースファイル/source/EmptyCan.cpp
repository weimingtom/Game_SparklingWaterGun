#include "EmptyCan.h"
#include "GameMatchInfomation.h"
#include "map\MapObject.h"
#include "system\System.h"

EmptyCan::EmptyCan(
	LPIEXMESH			pMesh,
	LPIEX2DOBJ			pTexture,
	int					live_time,
	float				size,
	CRVector3			pos
	):
	m_pMesh(pMesh),
	m_pTexture(pTexture),
	m_Move(0,0,0),
	m_Size(size),
	m_live_time(live_time),
	m_live_timer(0)
{
	D3DXMatrixScaling(&m_Trans, m_Size, m_Size, m_Size);
	m_Trans._41 = pos.x;
	m_Trans._42 = pos.y;
	m_Trans._43 = pos.z;
}

EmptyCan::~EmptyCan()
{

}

void EmptyCan::ParentMove(
	LPCMATRIX pParentWorldMat,
	LPCMATRIX pParentWorldMatInv,
	LPCMATRIX pParentMoveMat,
	LPCMATRIX AffineMat)
{
	m_Trans *= *AffineMat;
}

bool EmptyCan::isDelete()
{
	return m_live_time <= m_live_timer;
}

void EmptyCan::Update()
{
	Vector3 out, pos(m_Trans._41, m_Trans._42, m_Trans._43), vec(0, -1, 0);
	float dist = 100000000;

	m_Move.y -= 0.000f;
	++m_live_timer;

	if (MAP_MANAGER.RayPick(
		&out,
		&pos,
		&vec,
		&dist
		) &&
		(out.y > m_Trans._42))
	{
		m_Trans._42 = out.y;
		m_Move.y = 0;
	}

	m_Trans._41 += m_Move.x;
	m_Trans._42 += m_Move.y;
	m_Trans._43 += m_Move.z;
}

void EmptyCan::Render()
{
	m_pMesh->TransMatrix = m_Trans;

	m_pMesh->SetTexure(
		0,
		m_pTexture->GetTexture()
		);

	shader->SetValue("outlinecolor_A", 1);
	shader->SetValue("outlinecolor_R", 0);
	shader->SetValue("outlinecolor_G", 0);
	shader->SetValue("outlinecolor_B", 0);
	shader->SetValue("OutlineSize", 0.1f);

	m_pMesh->Render(shader, "character");

	m_pMesh->SetTexure(
		0,
		nullptr
		);
}