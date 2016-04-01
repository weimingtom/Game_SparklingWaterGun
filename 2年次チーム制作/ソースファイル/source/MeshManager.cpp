#include "MeshManager.h"

MeshManager::MeshManager(int	MeshMax):
m_MeshMax(MeshMax)
{
	m_pMesh_array = new LPMYMESH[m_MeshMax];
	for (int i = 0; i < m_MeshMax; ++i)
	{
		m_pMesh_array[i] = nullptr;
	}
}

MeshManager::~MeshManager()
{
	for (int i = 0; i < m_MeshMax; ++i)
	{
	//	MyDebugString("delete mesh= %d\n", i);
		delete m_pMesh_array[i];
	}
	delete[]m_pMesh_array;
}

MeshManager::MESH_ID MeshManager::MeshRegister(LPMYMESH pMesh)
{
	for (int i = 0; i < m_MeshMax; i++)
	{
		if (m_pMesh_array[i])continue;
		m_pMesh_array[i] = pMesh;
		return (MESH_ID)i;
	}
	delete pMesh;
	MyAssert(false, "メッシュ管理総数が最大値に達しました。");

	return ERR_ID;
}

bool MeshManager::MeshRegister(LPMYMESH pMesh, MESH_ID id)
{
	MyAssert(!m_pMesh_array[id], "重複してメッシュが登録されました。ID番号= %d",id);
	m_pMesh_array[id] = pMesh;
	return true;
}

LPMYMESH MeshManager::GetClone(MESH_ID id)
{
	MyAssert(m_pMesh_array[id], "登録されていないメッシュのクローンが要求されました。ID番号= %d", id);
	return m_pMesh_array[id]->Clone();
}


LPMYMESH MeshManager::GetMeshPtr(MESH_ID id)
{
	MyAssert(m_pMesh_array[id], "登録されていないメッシュが要求されました。ID番号= %d", id);
	return m_pMesh_array[id];
}