//#include "All.h"
//
//static const int PARTS_BONE_DATA[chr_motion_parts::_PARTS_MAX][20]=
//{
//	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,-1 },
//
//	{ 10, 11, 12, 13, 14, 15, 16, -1 },
//	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1 },
//};
//
//TestCharacterModel::TestCharacterModel(char *filename):
//m_model(filename)
//{
//}
//
//TestCharacterModel::~TestCharacterModel()
//{
//
//}
//
////���f����`��
//void TestCharacterModel::Render(LPCHARACTER C)
//{
//	const float scale = 0.180f;
//	ChrFunc::GetTransMatrix(C, m_model.TransMatrix, Vector3(scale, scale, scale));
//	m_model.Render();
//}
//
//void TestCharacterModel::Render(LPCHARACTER C, char* shader_name)
//{
//	const float scale = 0.180f;
//	ChrFunc::GetTransMatrix(C, m_model.TransMatrix, Vector3(scale, scale, scale));
//	m_model.Render(shader,shader_name);
//}
//
////�A�j���[�V����������
//void TestCharacterModel::Animation(
//	CHR_MOTION_PARTS parts,//�A�j���[�V���������镔��
//	float frame)	//�o�߃t���[��
//{
//	m_model.Animation();
//	m_model.Update();
//}
//
//
//
////�A�j���[�V����������t���[�����Z�b�g
//void TestCharacterModel::SetAnimationFrame(
//	CHR_MOTION_PARTS parts,//�A�j���[�V�������Z�b�g���镔��
//	float setframe)	//�o�߃t���[��
//{
//	for (int i = 0; PARTS_BONE_DATA[parts][i] != -1; i++)
//		m_model.SetFrame(PARTS_BONE_DATA[parts][i], (int)setframe);
//
//}
//
//
////���[�V�������Z�b�g
//void TestCharacterModel::SetMotion(
//	CHR_MOTION_PARTS parts,	//���[�V�������Z�b�g����̂̕���
//	CHR_MOTION_TYPE motion)//���[�V�����^�C�v
//{
//	for (int i = 0; PARTS_BONE_DATA[parts][i] != -1; i++)
//		m_model.SetMotion((int)motion, PARTS_BONE_DATA[parts][i]);
//}
//
////�u�����h���[�V�������Z�b�g
//void TestCharacterModel::SetBlendMotion(
//	CHR_MOTION_PARTS parts,	//���[�V�������Z�b�g����̂̕���
//	CHR_MOTION_TYPE motion1,//���[�V�����P
//	CHR_MOTION_TYPE motion2,//���[�V�����Q
//	RATE			rate	//���[�V����1�ɂQ���ǂꂾ���u�����h���邩
//	)
//{
//	for (int i = 0; PARTS_BONE_DATA[parts][i] != -1; i++)
//		m_model.SetMotion((int)motion1, PARTS_BONE_DATA[parts][i]);
//
//}
//
////�{�[���̍s��𓾂�
//bool TestCharacterModel::GetBoneMatrix(
//	LPMATRIX				pOut,		//�{�[���̍s����o��
//	CHARACTER_BONE_TYPE		bone_type	//�������{�[���̃^�C�v
//	)
//{
//	*pOut = *m_model.GetBone(bone_type);
//	return true;
//}