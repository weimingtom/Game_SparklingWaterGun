//#ifndef __TEST_CHARACTER_MODEL_H__
//#define __TEST_CHARACTER_MODEL_H__
//
//#include "CharacterMeshManager.h"
//#include "PartsMotionMesh.h"
//
//
//
////**********************************************************************************
////		テスト用のモデル管理ヘッダー(部位モーションとモーションblend無し)
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
//	//モデルを描画
//	void Render(LPCHARACTER C);
//	void Render(LPCHARACTER C,char* shader_name);
//
//	//アニメーションさせる
//	void Animation(
//		CHR_MOTION_PARTS parts,//アニメーションさせる部位
//		float frame);	//経過フレーム
//
//	//アニメーションさせるフレームをセット
//	void SetAnimationFrame(
//		CHR_MOTION_PARTS parts,//アニメーションをセットする部位
//		float setframe);	//経過フレーム
//
//	//モーションをセット
//	void SetMotion(
//		CHR_MOTION_PARTS parts,	//モーションをセットする体の部位
//		CHR_MOTION_TYPE motion);//モーションタイプ
//
//	//ブレンドモーションをセット
//	void SetBlendMotion(
//		CHR_MOTION_PARTS parts,	//モーションをセットする体の部位
//		CHR_MOTION_TYPE motion1,//モーション１
//		CHR_MOTION_TYPE motion2,//モーション２
//		RATE			rate	//モーション1に２をどれだけブレンドするか
//		);
//
//	//ワールド空間上でのボーンの行列を得る
//	bool GetBoneMatrix(
//		LPMATRIX				pOut,		//ボーンの行列を出力
//		CHARACTER_BONE_TYPE		bone_type	//得たいボーンのタイプ
//		);
//};
//
//#endif