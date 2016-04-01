#ifndef __CHARACTER_MODEL_H__
#define __CHARACTER_MODEL_H__

//#include "CharacterMeshManager.h"
//
//class ShadowMap;
//
//class RenderParam
//{
//public:
//
//	bool	shadow_Flg;		//影があるかどうか	
//	bool	wall_Hack;		//透視されているか
//	float	outline_Size;	//輪郭線のサイズ
//	union
//	{
//		float color[4];
//		struct
//		{
//			float outcolor_R, outcolor_G, outcolor_B, outcolor_A;
//		};
//	};
//	RenderParam()
//	{
//		shadow_Flg = true;
//		wall_Hack = false;
//		outline_Size = 1.0f;
//		outcolor_A = 1.0f;
//		outcolor_B = outcolor_G = outcolor_R = 1.0f;
//	}
//};

//**********************************************************************************
//		本チャンモデルヘッダ（モーションブレンドと部位モーションありver）
//**********************************************************************************
//
//class CharacterModel :public CharacterModelBase
//{
//
//public:
//	//モデルを描画
//	void Render(LPCHARACTER C);
//	void Render(LPCHARACTER C,char* shaderName);
//
//	const Matrix& GetMatrix()const
//	{
//		return Matrix();
//	}
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
//};

#endif