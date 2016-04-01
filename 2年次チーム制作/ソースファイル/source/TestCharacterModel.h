//#ifndef __TEST_CHARACTER_MODEL_H__
//#define __TEST_CHARACTER_MODEL_H__
//
//#include "CharacterMeshManager.h"
//#include "PartsMotionMesh.h"
//
//
//
////**********************************************************************************
////		�e�X�g�p�̃��f���Ǘ��w�b�_�[(���ʃ��[�V�����ƃ��[�V����blend����)
////**********************************************************************************
//
//class TestCharacterModel:public CharacterModelBase
//{
//private:
//	PartsMotionMesh	m_model;
//	int				m_motion;
//public:
//	TestCharacterModel(char *filename);
//	~TestCharacterModel();
//public:
//	//���f����`��
//	void Render(LPCHARACTER C);
//	void Render(LPCHARACTER C,char* shader_name);
//
//	//�A�j���[�V����������
//	void Animation(
//		CHR_MOTION_PARTS parts,//�A�j���[�V���������镔��
//		float frame);	//�o�߃t���[��
//
//	//�A�j���[�V����������t���[�����Z�b�g
//	void SetAnimationFrame(
//		CHR_MOTION_PARTS parts,//�A�j���[�V�������Z�b�g���镔��
//		float setframe);	//�o�߃t���[��
//
//	//���[�V�������Z�b�g
//	void SetMotion(
//		CHR_MOTION_PARTS parts,	//���[�V�������Z�b�g����̂̕���
//		CHR_MOTION_TYPE motion);//���[�V�����^�C�v
//
//	//�u�����h���[�V�������Z�b�g
//	void SetBlendMotion(
//		CHR_MOTION_PARTS parts,	//���[�V�������Z�b�g����̂̕���
//		CHR_MOTION_TYPE motion1,//���[�V�����P
//		CHR_MOTION_TYPE motion2,//���[�V�����Q
//		RATE			rate	//���[�V����1�ɂQ���ǂꂾ���u�����h���邩
//		);
//
//	//���[���h��ԏ�ł̃{�[���̍s��𓾂�
//	bool GetBoneMatrix(
//		LPMATRIX				pOut,		//�{�[���̍s����o��
//		CHARACTER_BONE_TYPE		bone_type	//�������{�[���̃^�C�v
//		);
//};
//
//#endif