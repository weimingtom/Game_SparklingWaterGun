#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "iextreme.h"
#include "map\MapObject.h"
//*********************************************************************
//	�����蔻��֐��w�b�_
//*********************************************************************

namespace collision
{
	void InitializeParam();
	//�_���[�W����

	//�{�[���ƃ{�[���̂����蔻��
	bool isHitBallBall(CRVector3 b1, float size1, CRVector3 b2, float size2);
	//�J�v�Z���ƃ{�[��
	bool isHitCapsuleBall(CRVector3 cap_start, CRVector3 cap_end, float capsule_size, CRVector3 ball_pos, float ball_size);
	//�J�v�Z���ƃJ�v�Z��
	bool isHitCapsuleCapsule(CRVector3 cap_start1, CRVector3 cap_end1, float capsule_size1, CRVector3 cap_start2, CRVector3 cap_end2, float capsule_size2);


	//�n�`�Ƃ̔���

	bool GetWall(const Vector3& pos, Vector3& v, float& outLen);

	//�ǂƂ̂����蔻���Ray���΂��Ď��s����
	bool HitCheckWall(LPVECTOR3 pOutChecked, LPVECTOR3 pOut, LPVECTOR3 pPos, LPVECTOR3 pVec, float* pDist, float client_size, float* pOutWallDot);
	//��{Ray���΂�ver
	bool HitCheckWallR2(LPVECTOR3 pOut, CRVector3 client_pos, CRVector3 client_move_vec, float client_size, float* pOutWallDot);

	//�ǂ̖@��������
	bool HitCheckWallGetNormal(LPVECTOR3 pOut, CRVector3 client_pos, CRVector3 client_move_vec, float client_size, Vector3& out_normal);
	bool HitCheckWall(LPVECTOR3 pOutChecked, LPVECTOR3 pOut, LPVECTOR3 pPos, LPVECTOR3 pVec, float* pDist, float client_size, float* pOutWallDot, Vector3& out_normal);

	//���Ƃ̂����蔻����Ƃ�(�������Ă����I�u�W�F�N�g�̃|�C���^��Ԃ�)
	LPMapObjectInterface HitCheckFloor(LPVECTOR3 pOut, CRVector3 client_pos, float ride_height, float adjustfloor, float character_size, float move);
};

#endif