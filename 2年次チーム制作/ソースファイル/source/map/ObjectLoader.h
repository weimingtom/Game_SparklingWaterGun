#pragma once

#include "iextreme.h"

//�I�u�W�F�N�g�̃f�[�^�\����
typedef struct ObjectData
{
	Vector3 pos;
	Vector3 angle;
	Vector3 scale;
	int type;            // �I�u�W�F�N�g�̃^�C�v
	int mesh_type;  // ���b�V���̃^�C�v
}ObjectData, *LPObjectData;

typedef std::list<ObjectData> ObjectDataList;
class textLoader;

//*************************************************************
//�I�u�W�F�N�g���[�h�N���X
//*************************************************************
class ObjectLoader
{
private:
	ObjectDataList      m_List;                 // ���o�͂Ɏg�p����f�[�^
	textLoader*          m_pTextLoader;   // �e�L�X�g���[�_
	LPSTR                   filename;              // �t�@�C����
	FILE*                    fp;                        // �t�@�C���|�C���^
public:
	ObjectLoader ( LPSTR filename );
	~ObjectLoader ();
public:
	ObjectDataList& GetList(){ return m_List; }
	// �ǂݍ���
	bool Load();
	// ��������
	bool Save ( ObjectDataList& List );
	// �I�u�W�F�N�g�P�ʂŏ�������
	void SaveObject ( const ObjectData& data );
	// �ǂݍ��񂾃f�[�^���o��
	void PrintLoadData ();
};



