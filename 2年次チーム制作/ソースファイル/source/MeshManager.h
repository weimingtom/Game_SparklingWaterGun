#ifndef __MESH_MANAGER_H__
#define __MESH_MANAGER_H__

#include "iextreme.h"

//*************************************************************
//メッシュ管理ヘッダ
//*************************************************************

typedef iexMesh	MYMESH;
typedef MYMESH* LPMYMESH;

//*************************************************************
//メッシュ管理クラス
//*************************************************************
class MeshManager
{
public:
	enum MESH_ID
	{
		ERR_ID,
	};
private:
	const int	m_MeshMax;
	LPMYMESH*	m_pMesh_array;
public:
	MeshManager(int	MeshMax);
	~MeshManager();
public:
	MESH_ID		MeshRegister(LPMYMESH pMesh);				//あいているIDにメッシュを登録し、そのID番号を返す
	bool		MeshRegister(LPMYMESH pMesh, MESH_ID id);	//メッシュを登録
	LPMYMESH	GetClone(MESH_ID id);						//指定したＩＤのメッシュを複製し、それへのポインタを返す
	LPMYMESH	GetMeshPtr(MESH_ID id);						//指定したＩＤのメッシュへのポインタを返す
};

#define MESHMGR (MeshManager::InstanceGet())

#endif