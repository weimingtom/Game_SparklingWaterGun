#ifndef __MAP_CRANE_H__
#define __MAP_CRANE_H__

#include "../ParentAndChild.h"
#include "../GameSystem\GameBase.h"
#include "ObjectLoader.h"
#include "MapObject.h"
#include "MapObjectFactory.h"
#include "ObjectLoader.h"
#include "SwingObject.h"

class CraneSteelFrame;


//*************************************************************
// �N���[���̃{�f�B�N���X
//*************************************************************
class CraneBody 
	:public MapObjectInterface
{
	float                         rotary_speed_;
	Vector3                    arm_position_;
	CraneSteelFrame*   SteelFrame_;
	bool                         initialized_;
	int                            timer_;
public:
	CraneBody(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	virtual~CraneBody();

	// �X�V�E�`��
	void Update()override;
	void Render()override;
	void Render_ShadowMap()override;

	// ������
	void Initialize(CraneSteelFrame* pSteelFrame);

	// �h�炷
	void AddForce(CRVector3 power);
	
	// �݂�ꏊ�擾
	Vector3 Get_ArmPosition()const { return arm_position_ + parameters_.pos; }
};

//*************************************************************
// �N���[���ɒ݂��Ă�����̃N���X
//*************************************************************
class CraneSteelFrame 
	:public MapObjectInterface
{
	int								ride_timer;
	int                             timer_;
	float                          max_wire_length_;
	CraneBody*              MyBody_;
	D3DXQUATERNION   pose_;
	SwingObject<1>       swing_object_;

	// ���݂̐݌v��AMeshManager����󂯎��Ƃ���
	// id ���킩��Ȃ����烁�b�V����CraneBody�ƕʂɂł��Ȃ��̂�
	// �Ƃ肠���������ō쐬����
	LPIEXMESH               wire_mesh_;
	Matrix                        wire_scale_;
public:
	const float AddPowerSize = 5.5f;
public:
	CraneSteelFrame(
		CraneBody*             pBody,
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	virtual~CraneSteelFrame();

	// �h�炷
	void AddForce(CRVector3 force);

	// �X�V�E�`��
	void Update()override;
	void Render()override;
	void Render_ShadowMap()override;

	// �����̎p����ς������Ƃ��͂����ł���
	void RotateSelf()override;

	// �e�̉e�����󂯂�
	void ParentMove(
		LPCMATRIX pParentWorldMat,
		LPCMATRIX pParentWorldMatInv,
		LPCMATRIX pParentMoveMat,
		LPCMATRIX AffineMat
		)override;

	bool HandleMessage(
		LPGameMessage msg)override;
};


//*************************************************************
// �N���[�����쐬����N���X
//*************************************************************
class CraneFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __MAP_CRANE_H__

// EOF