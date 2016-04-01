#ifndef __CHARACTER_MODEL_RESOUCE_H__
#define __CHARACTER_MODEL_RESOUCE_H__

#include "CharacterMeshManager.h"
#include "BlendAnimationMesh.h"


//**********************************************************************************
//		キャラクタモデル管理ヘッダー(部位モーションとモーションblend)
//**********************************************************************************

class CharacterModelResouce :public CharacterModelBase
{
private:
	BlendAnimationMesh	m_Mesh;
	Vector3				m_BodyAngle;
public:
	CharacterModelResouce(char *filename, LPIEX2DOBJ pBodyTexture);
	~CharacterModelResouce();
private:
	void GetRotationValue(D3DXQUATERNION& out);
	void BodyRotate();
public:
	static Vector3 GetToTargetAngle(LPCHARACTER C);

	//モデルを描画
	void Render(LPCHARACTER C);
	void Render(LPCHARACTER C, char* shader_name);

	const Matrix& GetMatrix()const
	{
		return m_Mesh.TransMatrix;
	}

	void SetMatrix(CRMatrix mat)
	{
		m_Mesh.TransMatrix = mat;
	}

	void AnimationParts(
		CHR_MOTION_PARTS	parts,	//アニメーションする部位
		float				speed,	//経過スピード
		int					line_no	//モーションライン
		);

	void Animation(float	speed);//経過スピード

	void UsualAnimation();

	void SetBodyAngle(CRVector3 v);
	void SetRotateMatrix(CRMatrix mat);

	void SetMotion(
		CHR_MOTION_TYPE		motion,
		CHR_MOTION_PARTS	parts,
		int					line_no,
		RATIO				blend,	//ライン１のBlend比率
		bool				update
		);

	void SetWeight(
		CHR_MOTION_PARTS	parts,
		int					line_no,
		RATIO				w
		);

	void SetSpeed(
		CHR_MOTION_PARTS	parts,
		int					line_no,
		RATIO				speed
		);

	void GetWeight(
		CHR_MOTION_PARTS	parts,
		RATIO*	pOutArray
		);

	void SetMotionFrame(
		CHR_MOTION_PARTS	parts,
		int					line_no,
		float				frame);

	float GetFrame(
		CHR_MOTION_PARTS	parts,
		int					line_no
		);

	float GetMotionFrame(
		CHR_MOTION_PARTS	parts,
		int					line_no
		);

	const Matrix& GetBoneMatrix(CHARACTER_BONE_TYPE type);
};

#endif