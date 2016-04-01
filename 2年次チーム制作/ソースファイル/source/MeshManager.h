#ifndef __MESH_MANAGER_H__
#define __MESH_MANAGER_H__

#include "iextreme.h"

//*************************************************************
//���b�V���Ǘ��w�b�_
//*************************************************************

typedef iexMesh	MYMESH;
typedef MYMESH* LPMYMESH;

//*************************************************************
//���b�V���Ǘ��N���X
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
	MESH_ID		MeshRegister(LPMYMESH pMesh);				//�����Ă���ID�Ƀ��b�V����o�^���A����ID�ԍ���Ԃ�
	bool		MeshRegister(LPMYMESH pMesh, MESH_ID id);	//���b�V����o�^
	LPMYMESH	GetClone(MESH_ID id);						//�w�肵���h�c�̃��b�V���𕡐����A����ւ̃|�C���^��Ԃ�
	LPMYMESH	GetMeshPtr(MESH_ID id);						//�w�肵���h�c�̃��b�V���ւ̃|�C���^��Ԃ�
};

#define MESHMGR (MeshManager::InstanceGet())

#endif