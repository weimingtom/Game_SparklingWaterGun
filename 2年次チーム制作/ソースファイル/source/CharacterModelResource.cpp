#include "CharacterModelResource.h"
#include "CharacterUtilityFunction.h"
#include "system\System.h"

#include "EffectResource.h"


static const int PARTS_BONE_DATA[chr_motion_parts::_PARTS_MAX][50] =
{
	{ 0,	1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
			11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
			21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
			31, 32, 33, 34, 35, 36, 37, 38, -1 },

	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, - 1 },	//low
	{ 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
	29, 30,
	31, 32, 33, 34, 35, 36, 37, 38,
	-1 },//up
};

static const int BodyTextureIndex = 0;

CharacterModelResouce::CharacterModelResouce(char *filename, LPIEX2DOBJ pBodyTexture) :
m_Mesh(filename),
m_BodyAngle(0,0,0)
{
	m_Mesh.SetTexure(BodyTextureIndex, pBodyTexture->GetTexture());
}

CharacterModelResouce::~CharacterModelResouce()
{
	m_Mesh.SetTexure(BodyTextureIndex, nullptr);
}

Vector3 CharacterModelResouce::GetToTargetAngle(LPCHARACTER C)
{
	Vector3 v1, v2=ChrFunc::GetFront(C),ret;

	ChrFunc::GetUI(C)->GetTargetPos(v1);
	v1 -= C->GetBodyCenter();

	v1.Normalize();
	v2.Normalize();
	
	ret.x = v1.y - v2.y;// +((PI / 4)*sinf(ChrFunc::GetParam(C).m_damage_reaction*PI));
	v1.y = 0; v2.y = 0;

	v1.Normalize();
	v2.Normalize();

	ret.y = acosf(Vector3Dot(v1,v2));
	Vector3 cr;
	Vector3Cross(cr, v1, v2);
	if (cr.y < 0)
	{
		ret.y = -ret.y;
	}
	
	ret.z = 0;

	if (isErrorValuef(ret.y))ret.y = 0;

	return ret;
}

void CharacterModelResouce::GetRotationValue(D3DXQUATERNION& out)
{
	D3DXQUATERNION temp;
	
	D3DXQuaternionIdentity(&out);

	if (isErrorVec(m_BodyAngle))
	{
		int a = 0;
	}

	D3DXQuaternionRotationAxis(&temp, (D3DXVECTOR3*)&VECTOR3AXISX, m_BodyAngle.y);
	out *= temp;

	D3DXQuaternionRotationAxis(&temp, (D3DXVECTOR3*)&VECTOR3AXISZ, m_BodyAngle.x);
	out *= temp;


}

void CharacterModelResouce::BodyRotate()
{
	const int body_bone = 12;

	D3DXQUATERNION q;
	m_Mesh.UpdateSkinMeshFrame();
	GetRotationValue(q);

	D3DXQUATERNION* br = (D3DXQUATERNION*)&m_Mesh.GetRot(body_bone);
	*br *= q;
	m_Mesh.UpdateBoneMatrix();
	m_Mesh.UpdateSkinMesh();
}

//モデルを描画
void CharacterModelResouce::Render(LPCHARACTER C)
{
	m_Mesh.Render();
}

void CharacterModelResouce::Render(LPCHARACTER C, char* shader_name)
{
	m_Mesh.Render(shader, shader_name);
}

void CharacterModelResouce::AnimationParts(
	CHR_MOTION_PARTS	parts,	//アニメーションをセットする部位
	float				speed,	//経過スピード
	int					line_no	//モーションライン
	)
{
	for (int i = 0; PARTS_BONE_DATA[parts][i] != -1; ++i)
	{
		m_Mesh.Animation(PARTS_BONE_DATA[parts][i], speed, line_no);
	}

}

//アニメーションさせる
void CharacterModelResouce::Animation(
	float	speed)	//経過フレーム
{
	m_Mesh.Animation(speed);
	BodyRotate();
}

void CharacterModelResouce::UsualAnimation()
{
	m_Mesh.UsualAnimation();
}

void CharacterModelResouce::SetBodyAngle(CRVector3 v)
{
	m_BodyAngle = v;
}

void CharacterModelResouce::SetMotion(
	CHR_MOTION_TYPE		motion,
	CHR_MOTION_PARTS	parts,
	int					line_no,
	RATIO				blend,
	bool				update
	)
{
	for (int i = 0; PARTS_BONE_DATA[parts][i] != -1; ++i)
	{
		if (update||m_Mesh.GetMotion(PARTS_BONE_DATA[parts][i], line_no) != (int)motion)
			m_Mesh.SetMotion(motion, PARTS_BONE_DATA[parts][i], line_no);
		m_Mesh.SetWeight(PARTS_BONE_DATA[parts][i], line_no, blend);
	}
}

void CharacterModelResouce::SetWeight(
	CHR_MOTION_PARTS	parts,
	int					line_no,
	RATIO				w
	)
{
	for (int i = 0; PARTS_BONE_DATA[parts][i] != -1; ++i)
	{
		m_Mesh.SetWeight(PARTS_BONE_DATA[parts][i], line_no, w);
	}
}

void CharacterModelResouce::SetSpeed(
	CHR_MOTION_PARTS	parts,
	int					line_no,
	RATIO				speed
	)
{
	for (int i = 0; PARTS_BONE_DATA[parts][i] != -1; ++i)
	{
		m_Mesh.SetSpeed(PARTS_BONE_DATA[parts][i], line_no, speed);
	}
}

void CharacterModelResouce::GetWeight(
	CHR_MOTION_PARTS	parts,
	RATIO*	pOutArray
	)
{
	for (int i = 0; i < 2; i++)
	{
		pOutArray[i] = m_Mesh.GetWeight(PARTS_BONE_DATA[parts][0], i);
	}
}

void CharacterModelResouce::SetMotionFrame(
	CHR_MOTION_PARTS	parts,
	int					line_no,
	float					frame)
{
	float motionFrame = m_Mesh.GetMotionOffset(
		m_Mesh.GetMotion(PARTS_BONE_DATA[parts][0], line_no)
		);

	for (int i = 0; PARTS_BONE_DATA[parts][i] != -1; ++i)
	{
		m_Mesh.SetFrame(motionFrame+frame, PARTS_BONE_DATA[parts][i], line_no);
	}
}

float CharacterModelResouce::GetFrame(
	CHR_MOTION_PARTS	parts,
	int					line_no
	)
{
	return m_Mesh.GetFrame(
		PARTS_BONE_DATA[parts][0],
		line_no
		);
}

const Matrix& CharacterModelResouce::GetBoneMatrix(CHARACTER_BONE_TYPE type)
{
	return *m_Mesh.GetBone((int)type);
}


float CharacterModelResouce::GetMotionFrame(
	CHR_MOTION_PARTS	parts,
	int					line_no
	)
{

	return 
		m_Mesh.GetFrame(PARTS_BONE_DATA[parts][0], line_no) -
		(float)m_Mesh.GetMotionOffset(m_Mesh.GetMotion(PARTS_BONE_DATA[parts][0], line_no));
}