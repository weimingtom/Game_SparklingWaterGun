#include "All.h"

//0.923879504f

static const int HitCheckRayRotateQuaternionCount = 4;
static DXQuaternion HitCheckRayRotateQuaternion[HitCheckRayRotateQuaternionCount];

void collision::InitializeParam()
{
	const float rad[HitCheckRayRotateQuaternionCount] =
	{
		0,
		PI / 4,
		-(PI / 4),
		PI
	};
	const D3DXVECTOR3 axis(0, 1, 0);
	for (int i = 0; i < HitCheckRayRotateQuaternionCount; i++)
	{
		if (rad[i] != 0)
			D3DXQuaternionRotationAxis(&HitCheckRayRotateQuaternion[i], &axis, rad[i]);
		else
			D3DXQuaternionIdentity(&HitCheckRayRotateQuaternion[i]);
	}
}

//ボールとボールのあたり判定
bool collision::isHitBallBall(CRVector3 b1, float size1, CRVector3 b2, float size2)
{
	float out;
	Vector3 t;
	VECTOR3POINTTOPOINT(out, b1, b2, t);
	return (out < size1 + size2);
}

//カプセルとボール
bool collision::isHitCapsuleBall(CRVector3 cap_start, CRVector3 cap_end, float capsule_size, CRVector3 ball_pos, float ball_size)
{
	Vector3 cap_vec;
	VECTOR3MINUS(cap_vec,cap_end,cap_start);

	float caplen = VECTOR3LENGTH(cap_vec);

	VECTOR3DIVSCALAR(cap_vec, cap_vec, caplen);
	float l = Vector3Dot(cap_vec, ball_pos - cap_start);
	
	l = max(l, 0);
	l = min(l, caplen);
	
	VECTOR3MULSCALAR(cap_vec, cap_vec, l);
	VECTOR3PLUS(cap_vec, cap_vec, cap_start);
	VECTOR3MINUS(cap_vec, cap_vec, ball_pos);

	return VECTOR3LENGTH(cap_vec) < (capsule_size + ball_size);
}

//カプセルとカプセル
bool collision::isHitCapsuleCapsule(CRVector3 cap_start1, CRVector3 cap_end1, float capsule_size1, CRVector3 cap_start2, CRVector3 cap_end2, float capsule_size2);


bool collision::GetWall(const Vector3& pos, Vector3& v, float& outLen)
{
	Vector3 out;
	outLen = 10000.0f;
	Vector3 p = pos;

	if (MAP_MANAGER.RayPick(&out, &p, &v, &outLen))
	{
		out -= pos;
		outLen = out.Length();

		return true;
	}

	return false;
}


//壁とのあたり判定をRayを飛ばして実行する
bool collision::HitCheckWall(
	LPVECTOR3 pOutChecked,
	LPVECTOR3 pOut,
	LPVECTOR3 pPos,
	LPVECTOR3 pVec,
	float* pDist,
	float client_size,
	float* pOutWallDot)
{
	*pOutChecked = *pPos;

	if (GetWall(*pPos, *pVec, *pDist)&&
		*pDist<client_size)
	{
		pVec->Normalize();	//かべの法線

		*pDist = client_size - *pDist;//調整する長さ

		*pPos = *pVec**pDist;	//補正ベクトル

		*pDist = pPos->Length();	//補正距離

		*pVec = *pPos;
		pVec->Normalize();	//補正ベクトル正規化

		*pOutWallDot = *pDist;//補正距離temp

		if (!GetWall(*pPos, *pVec, *pDist)||
			*pOutWallDot<*pDist)
		{
			*pOutChecked += *pPos;
		}
		return true;
	}

	return false;
}

//二本Rayを飛ばすver
bool collision::HitCheckWallR2(LPVECTOR3 pOut, CRVector3 client_pos, CRVector3 client_move_vec, float client_size, float* pOutWallDot)
{
	Vector3 move_vec(client_move_vec), out, pos(client_pos), vec;
	float dist;
	float mMinusDot = 0;
	bool ret = false;
	move_vec.y = 0;

	move_vec.Normalize();
	
	for (int i = 0; i < HitCheckRayRotateQuaternionCount; i++)
	{
		vec = Vector3RotateQuaternion(HitCheckRayRotateQuaternion[i], move_vec);
		dist = 100;

		if (HitCheckWall(pOut, &out, &pos, &vec, &dist, client_size, pOutWallDot))
		{
			mMinusDot = min(*pOutWallDot, mMinusDot);
			pos = *pOut;
			ret = true;
		}
	}
	*pOutWallDot = mMinusDot;
	return ret;
}

bool collision::HitCheckWallGetNormal(
	LPVECTOR3 pOut, 
	CRVector3 client_pos,
	CRVector3 client_move_vec,
	float client_size,
	Vector3& out_normal)
{
	Vector3 move_vec(client_move_vec), out, pos(client_pos), vec;
	float dist;
	float mMinusDot = 0;
	bool ret = false;
	move_vec.y = 0;

	move_vec.Normalize();

	for (int i = 0; i < HitCheckRayRotateQuaternionCount; i++)
	{
		vec = Vector3RotateQuaternion(HitCheckRayRotateQuaternion[i], move_vec);
		dist = 100;

		if (HitCheckWall(pOut, &out, &pos, &vec, &dist, client_size, &mMinusDot, out_normal))
		{
			pos = *pOut;
			ret = true;
		}
	}

	return ret;
}

bool collision::HitCheckWall(
	LPVECTOR3 pOutChecked, LPVECTOR3 pOut, LPVECTOR3 pPos, LPVECTOR3 pVec, float* pDist, float client_size, float* pOutWallDot, Vector3& out_normal)
{
	*pOutChecked = *pPos;

	if (GetWall(*pPos, *pVec, *pDist) &&
		*pDist<client_size)
	{
		pVec->Normalize();	//かべの法線
		out_normal = *pVec;

		*pDist = client_size - *pDist;//調整する長さ

		*pPos = *pVec**pDist;	//補正ベクトル

		*pDist = pPos->Length();	//補正距離

		*pVec = *pPos;
		pVec->Normalize();	//補正ベクトル正規化

		*pOutWallDot = *pDist;//補正距離temp

		if (!GetWall(*pPos, *pVec, *pDist) ||
			*pOutWallDot<*pDist)
		{
			*pOutChecked += *pPos;
		}
		return true;
	}

	return false;
}

//床とのあたり判定をとる(あたっていたオブジェクトのポインタを返す)
LPMapObjectInterface collision::HitCheckFloor(LPVECTOR3 pOut, CRVector3 client_pos, float ride_height, float adjustfloor, float character_size,float move)
{
	LPMapObjectInterface pRideObject = nullptr;
	
	Vector3 out, pos(client_pos), vec(0,-1,0),tmp;
	float dist = 100000;

	pos.y += ride_height;

	float pdv[4][2]=
	{
		{0,1},
		{1,0,},
		{0,-1},
		{-1,0}
	};

	for (int i = 0; i<4; i++)
	for (int j = 0; j < 2; j++)
		pdv[i][j] *= character_size;
	
	for (int i = 0; i<4; i++)
	{
		tmp = pos;
		tmp.x += pdv[i][0];
		tmp.z += pdv[i][1];

		if ((pRideObject = MAP_MANAGER.RayPick(&out, &tmp, &vec, &dist)) &&
			out.y + adjustfloor > client_pos.y - move)
		{
			*pOut = out;
		}
		else{
			pRideObject = nullptr;
		}
		if (pRideObject)break;

		dist = 10000000;
		vec.x = vec.z = 0;
		vec.y = -1;
	}
	return pRideObject;
}