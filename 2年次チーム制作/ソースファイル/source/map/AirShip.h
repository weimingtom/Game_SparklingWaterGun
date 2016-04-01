#ifndef __AIR_SHIP_H__
#define __AIR_SHIP_H__

#include "../ParentAndChild.h"
#include "../GameSystem\GameBase.h"
#include "ObjectLoader.h"
#include "MapObject.h"
#include "MapObjectFactory.h"
#include "ObjectLoader.h"


//*************************************************************
// ��s�D�N���X
//*************************************************************
class AirShip
	:public MapObjectInterface
{
private:
	const Vector3 rotation_center_;    // ��]�̒��S
	float                rotation_radius_;    // ��]�̔��a
	float                rotation_speed_;    // �p���x ��1�t���[��
	float                rotation_angle_;     // ���S����̉�]�p�x
public:
	AirShip(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~AirShip();

public:
	// �X�V�E�`��
	void Update()override;
	void Render()override;
	void Render_ShadowMap()override;
	bool RayPick(
		Vector3* out,
		Vector3* pos,
		Vector3*vec,
		float* dist
		)override
	{
		return false;
	}
};


//*************************************************************
// ��s�D�쐬�N���X
//*************************************************************
class AirShipFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __AIR_SHIP_H__

// EOF