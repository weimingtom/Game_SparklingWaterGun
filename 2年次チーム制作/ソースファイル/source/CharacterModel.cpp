//#include "All.h"
//
//
//CharacterModel::CharacterModel(char *filename) :
//m_model(filename)
//{
//}
//
//CharacterModel::~CharacterModel()
//{
//
//}
//
////���f����`��
//void CharacterModel::Render(LPCHARACTER C)
//{
//	const CHRPARAM& p = ChrFunc::GetParam(C);
//	const float scale = 0.005f;
//
//	CRVector3 pos = ControllObjFunc::GetPos(C);
//	Vector3 front(p.m_view_vec), right(ChrFunc::GetRight(C)), up;
//	Vector3Cross(up, front, right);
//
//	Matrix mat;
//	D3DXMatrixIdentity(&mat);
//
//	front.Normalize();
//	right.Normalize();
//	up.Normalize();
//
//	front *= scale;
//	right *= scale;
//	up *= scale;
//
//	mat._11 = right.x;
//	mat._12 = right.y;
//	mat._13 = right.z;
//
//	mat._21 = up.x;
//	mat._22 = up.y;
//	mat._23 = up.z;
//
//	mat._31 = front.x;
//	mat._32 = front.y;
//	mat._33 = front.z;
//
//	mat._41 = pos.x;
//	mat._42 = pos.y;
//	mat._43 = pos.z;
//
//	m_model.Render(mat);
//}
//
//
////�A�j���[�V����������
//void CharacterModel::Animation(
//	CHR_MOTION_PARTS parts,//�A�j���[�V���������镔��
//	float frame)	//�o�߃t���[��
//{
//	m_model.Animation();
//}
//
//
//
////�A�j���[�V����������t���[�����Z�b�g
//void CharacterModel::SetAnimationFrame(
//	CHR_MOTION_PARTS parts,//�A�j���[�V�������Z�b�g���镔��
//	float setframe)	//�o�߃t���[��
//{
//	m_model.SetFrame((int)setframe);
//}
//
//
////���[�V�������Z�b�g
//void CharacterModel::SetMotion(
//	CHR_MOTION_PARTS parts,	//���[�V�������Z�b�g����̂̕���
//	CHR_MOTION_TYPE motion)//���[�V�����^�C�v
//{
//	if (m_model.GetMotion() != (int)motion)
//	{
//		m_model.SetMotion((int)motion);
//	}
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
//	if (m_model.GetMotion() != (int)motion1)
//	{
//		m_model.SetMotion((int)motion1);
//	}
//}