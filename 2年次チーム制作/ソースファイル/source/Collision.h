#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "iextreme.h"
#include "map\MapObject.h"
//*********************************************************************
//	あたり判定関数ヘッダ
//*********************************************************************

namespace collision
{
	void InitializeParam();
	//ダメージ判定

	//ボールとボールのあたり判定
	bool isHitBallBall(CRVector3 b1, float size1, CRVector3 b2, float size2);
	//カプセルとボール
	bool isHitCapsuleBall(CRVector3 cap_start, CRVector3 cap_end, float capsule_size, CRVector3 ball_pos, float ball_size);
	//カプセルとカプセル
	bool isHitCapsuleCapsule(CRVector3 cap_start1, CRVector3 cap_end1, float capsule_size1, CRVector3 cap_start2, CRVector3 cap_end2, float capsule_size2);


	//地形との判定

	bool GetWall(const Vector3& pos, Vector3& v, float& outLen);

	//壁とのあたり判定をRayを飛ばして実行する
	bool HitCheckWall(LPVECTOR3 pOutChecked, LPVECTOR3 pOut, LPVECTOR3 pPos, LPVECTOR3 pVec, float* pDist, float client_size, float* pOutWallDot);
	//二本Rayを飛ばすver
	bool HitCheckWallR2(LPVECTOR3 pOut, CRVector3 client_pos, CRVector3 client_move_vec, float client_size, float* pOutWallDot);

	//壁の法線もえる
	bool HitCheckWallGetNormal(LPVECTOR3 pOut, CRVector3 client_pos, CRVector3 client_move_vec, float client_size, Vector3& out_normal);
	bool HitCheckWall(LPVECTOR3 pOutChecked, LPVECTOR3 pOut, LPVECTOR3 pPos, LPVECTOR3 pVec, float* pDist, float client_size, float* pOutWallDot, Vector3& out_normal);

	//床とのあたり判定をとる(あたっていたオブジェクトのポインタを返す)
	LPMapObjectInterface HitCheckFloor(LPVECTOR3 pOut, CRVector3 client_pos, float ride_height, float adjustfloor, float character_size, float move);
};

#endif