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
////モデルを描画
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
////アニメーションさせる
//void TestCharacterModel::Animation(
//	CHR_MOTION_PARTS parts,//アニメーションさせる部位
//	float frame)	//経過フレーム
//{
//	m_model.Animation();
//	m_model.Update();
//}
//
//
//
////アニメーションさせるフレームをセット
//void TestCharacterModel::SetAnimationFrame(
//	CHR_MOTION_PARTS parts,//アニメーションをセットする部位
//	float setframe)	//経過フレーム
//{
//	for (int i = 0; PARTS_BONE_DATA[parts][i] != -1; i++)
//		m_model.SetFrame(PARTS_BONE_DATA[parts][i], (int)setframe);
//
//}
//
//
////モーションをセット
//void TestCharacterModel::SetMotion(
//	CHR_MOTION_PARTS parts,	//モーションをセットする体の部位
//	CHR_MOTION_TYPE motion)//モーションタイプ
//{
//	for (int i = 0; PARTS_BONE_DATA[parts][i] != -1; i++)
//		m_model.SetMotion((int)motion, PARTS_BONE_DATA[parts][i]);
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
//	for (int i = 0; PARTS_BONE_DATA[parts][i] != -1; i++)
//		m_model.SetMotion((int)motion1, PARTS_BONE_DATA[parts][i]);
//
//}
//
////ボーンの行列を得る
//bool TestCharacterModel::GetBoneMatrix(
//	LPMATRIX				pOut,		//ボーンの行列を出力
//	CHARACTER_BONE_TYPE		bone_type	//得たいボーンのタイプ
//	)
//{
//	*pOut = *m_model.GetBone(bone_type);
//	return true;
//}