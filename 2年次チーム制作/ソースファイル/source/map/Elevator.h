#ifndef __ELEVATOR_H__
#define __ELEVATOR_H__

#include "../ParentAndChild.h"
#include "../GameSystem\GameBase.h"
#include "ObjectLoader.h"
#include "MapObject.h"
#include "MapObjectFactory.h"
#include "ObjectLoader.h"


//*************************************************************
// �G���x�[�^�[�N���X
//*************************************************************
class Elevator
	:public MapObjectInterface
{
protected:
	enum
	{
		_wait_root,              // k_root_pos_�ő҂�
		_wait_target,           // k_target_pos_�ő҂�
		_move_to_target,    // k_root_pos_�Ɍ������ē���
		_move_to_root,       // k_target_pos_�Ɍ������ē���
	};

	int                    state_;               // ���݂̏��
	int                    timer_;               // �^�C�}�[
	const Vector3   k_target_pos_;  // ������
	const Vector3   k_root_pos_;     // ������
	Vector3            move_;               // �ړ���
public:
	Elevator(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~Elevator();

public:
	// �X�V�E�`��
	void Update()override;
	void Render()override;
	void Render_ShadowMap()override;

protected:
	// �X�e�[�g�X�V
	void UpdateState();
	
	// �X�e�[�g�֐�
	void WaitRoot();
	void WaitTarget();
	void MoveToRoot();
	void MoveToTarget();

};

//*************************************************************
// �G���x�[�^�[�쐬�N���X
//*************************************************************
class ElevatorFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};


#endif // __ELEVATOR_H__

// EOF