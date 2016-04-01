#ifndef __VENDING_MACHINE_H__
#define __VENDING_MACHINE_H__


#include "ObjectLoader.h"
#include "MapObject.h"
#include "../DamageHit.h"
#include "MapObjectFactory.h"
#include "WayPointObject.h"
#include "ShareRatioObject.h"

//*************************************************************
// �����̔��@�N���X
//*************************************************************
class VendingMachine
	:public ShareRatioObjectBase
{
private:

public:
	VendingMachine(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~VendingMachine();
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

	void Hit(DamageObject* p)override;

	// �`�[���ύX
	void ChangeTeam(DamageObject* p)override;
};

//*************************************************************
// �����̔��@�쐬�N���X
//*************************************************************
class VendingMachineFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif