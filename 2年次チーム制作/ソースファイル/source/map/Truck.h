#ifndef __MAP_TRUCK_OBJECT_H__
#define __MAP_TRUCK_OBJECT_H__

#include "ObjectLoader.h"
#include "MapObject.h"
#include "../DamageHit.h"
#include "MapObjectFactory.h"
#include "WayPointObject.h"
#include "ShareRatioObject.h"

// �p�x�R���g���[��
void angleContRoll(CRVector3 MyPos, CRVector3 ViewPos, float *angle, float MaxSpped);

//*************************************************************
// �g���b�N�N���X
//*************************************************************
class Truck
	:public ShareRatioObjectBase
{
	Vector3                  target_pos_;           // �^�[�Q�b�g�̍��W(�����Ɍ������Ĉړ�����)
	float                       speed_;                   // �ړ����x
	Vector3                  move_;                   // �ړ���
	WayPointObject*   target_point_;         // �^�[�Q�b�g��WayPointObject
	DamageObject       damage_object_;
	bool                       is_step_brakes_;     // true : ����   false : ����
public:
	Truck(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~Truck();
public:
	// �X�V�E�`��
	void Update()override;
	void Render()override;
	void Render_ShadowMap()override;

	//RayPick
	bool RayPick(
		Vector3* out,
		Vector3* pos,
		Vector3*vec,
		float* dist
		)override;

	// �I�u�W�F�N�g�̈ړ��ʎ擾
	Vector3 Get_MoveParameter()const override{ return move_; }
	
	void Hit(DamageObject* p)override;

	// �`�[���ύX
	void ChangeTeam(DamageObject* p)override;
private:
	// �O�ɖ��������邩�ǂ���
	bool Is_FriendForward();
	// �O�ɂق��̃g���b�N�����邩�ǂ���
	bool Is_TruckForward();
	// �O�ɉ���������Ǝ~�܂�
	void CheckForward();

	// ���x�X�V
	void UpdateSpeed();

	void UpdateTarget();
	void UpdateAngle();
};

//*************************************************************
// �g���b�N�쐬�N���X
//*************************************************************
class TruckFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __MAP_TRUCK_OBJECT_H__

// EOF