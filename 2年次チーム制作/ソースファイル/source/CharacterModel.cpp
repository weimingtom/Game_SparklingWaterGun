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
////モデルを描画
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
////アニメーションさせる
//void CharacterModel::Animation(
//	CHR_MOTION_PARTS parts,//アニメーションさせる部位
//	float frame)	//経過フレーム
//{
//	m_model.Animation();
//}
//
//
//
////アニメーションさせるフレームをセット
//void CharacterModel::SetAnimationFrame(
//	CHR_MOTION_PARTS parts,//アニメーションをセットする部位
//	float setframe)	//経過フレーム
//{
//	m_model.SetFrame((int)setframe);
//}
//
//
////モーションをセット
//void CharacterModel::SetMotion(
//	CHR_MOTION_PARTS parts,	//モーションをセットする体の部位
//	CHR_MOTION_TYPE motion)//モーションタイプ
//{
//	if (m_model.GetMotion() != (int)motion)
//	{
//		m_model.SetMotion((int)motion);
//	}
//}
//
////ブレンドモーションをセット
//void TestCharacterModel::SetBlendMotion(
//	CHR_MOTION_PARTS parts,	//モーションをセットする体の部位
//	CHR_MOTION_TYPE motion1,//モーション１
//	CHR_MOTION_TYPE motion2,//モーション２
//	RATE			rate	//モーション1に２をどれだけブレンドするか
//	)
//{
//	if (m_model.GetMotion() != (int)motion1)
//	{
//		m_model.SetMotion((int)motion1);
//	}
//}