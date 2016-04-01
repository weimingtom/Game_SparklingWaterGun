#ifndef __CAMERA_MOVE_H__
#define __CAMERA_MOVE_H__

#include "iextreme.h"
#include "Camera.h"
#include "Character.h"

//****************************************************************
//	�w�肵���ꏊ�ŃX�e�B�b�N�Ŏ��_�����R�ɉ񂹂�J����
//****************************************************************
class  RunAfterPlayerCamera :public CAMERASTATE
{
private:
	static const float up_angle_max, down_angle_max;
	const float				rotate_speed;				//�J������]�X�s�[�h
	D3DXQUATERNION			rotq, targetrotq;			//���݂̉�]�N�H�[�^�j�I���@�Ɓ@��]�ڕW�N�H�[�^�j�I��
	const Vector3			rotatePos, upvec, adjust;	//��]�����鏉���ʒu �Ɓ@Y��]��
	const Vector3&			FollowPos;
public:
	RunAfterPlayerCamera(
		_client_type_ptr p,
		float rotate_speed,
		const Vector3&		FollowPos,
		const Vector3&		adjust);
	~RunAfterPlayerCamera();
private:
	//�����̃N�H�[�^�j�I���ɂ���ĉ�]������̃J�����̈ʒu�𓾂�
	Vector3 GetCameraQuaternionPos(const D3DXQUATERNION& q);
	//�J�����̉E�����x�N�g��
	Vector3 GetRightVec();
	//�J�����̃L�[��]������
	void CameraRotate();
	//�J�����̈ʒu��␳����
	void CameraAdjust();
public:
	void Enter(_client_type_ptr p);			//	������
	void Execute(_client_type_ptr p);		//	���s
	void Exit(_client_type_ptr p);			//	�I��
};

//****************************************************************
//	�������Ȃ��J����
//****************************************************************
class FreezeCamera :public CAMERASTATE
{
public:
	void Enter(_client_type_ptr p);			//	������
	void Execute(_client_type_ptr p);	//	���s
	void Exit(_client_type_ptr p);			//	�I��
};

//****************************************************************
//	�f�o�b�O�p�H�ړ��J����
//****************************************************************
class FreeMoveCamera :public CAMERASTATE
{
private:

public:
	void Enter(_client_type_ptr p);			//	������
	void Execute(_client_type_ptr p);	//	���s
	void Exit(_client_type_ptr p);			//	�I��
};

//****************************************************************
//	�v���C���[�ʏ��Ԃł̃J����
//****************************************************************
class UsualPlayerMoveCamera :public CAMERASTATE
{
private:
	LPCHARACTER		m_pFollowCharacter;			//�ǔ�����L�����N�^
	DXQuaternion	m_camera_pose;				//�J���������N�H�[�^�j�I��
	const float		m_rotate_speed;				//�v���C���[�ɂ��Ă����X�s�[�h
	const float		m_local_pos_move_speed;		//���[�J���ʒu�̕�ԃX�s�[�h
	const Vector3	m_finish_rotate_position;	//��]�����鏉�����W
	const Vector3	m_finish_rotate_target;		//��]������^�[�Q�b�g�ʒu
	Vector3			m_rotate_position;			//���݂̃��[�J���ʒu�̍��W
	Vector3			m_rotate_target;			//���݂̃��[�J���ʒu�̃^�[�Q�b�g
public:
	UsualPlayerMoveCamera(
		LPCAMERA	pCamera,				//�J�����ւ̃|�C���^
		LPCHARACTER	pFollowCharacter,		//�ǔ�����L�����N�^
		float		m_local_pos_move_speed,	//���[�J���ʒu�̕�ԃX�s�[�h
		float		rotate_speed,			//�v���C���[�ɂ��Ă����X�s�[�h
		CRVector3	local_pos,				//�J�����̃��[�J���ʒu
		CRVector3	local_target			//�J�����̃��[�J���^�[�Q�b�g
		);
	~UsualPlayerMoveCamera();
private:
	bool isEquQuaternion(const DXQuaternion& q1, const DXQuaternion& q2);
	DXQuaternion GetTargetQuaternion(LPCHARACTER pFollowCharacter);
public:
	void Enter(_client_type_ptr p);			//	������
	void Execute(_client_type_ptr p);	//	���s
	void Exit(_client_type_ptr p);			//	�I��
};


//****************************************************************
//	���X�|�[�����J����
//****************************************************************
class RespornCamera :public CAMERASTATE
{
private:
	LPCAMERASTATE		pNextState;
	const int			max_time;
	int					now_time;
public:
	RespornCamera(int time, LPCAMERASTATE pNext);
	~RespornCamera();
public:
	void Enter(_client_type_ptr p);			//	������
	void Execute(_client_type_ptr p);	//	���s
	void Exit(_client_type_ptr p);			//	�I��
};

//****************************************************************
//	�X�^�[�g���J����
//****************************************************************
class StartCamera :public CAMERASTATE
{
private:
	typedef struct 
	{
		Vector3 pos, target;
		int		move_frame;

	}PositionData;

	Vector3			m_pre_pos, m_pre_target;
	PositionData*	m_pMove_data;
	int				m_MaxData;
	int				m_NowIndex;
	int				m_frame_count;
	bool*			m_pIsFinish;
public:
	StartCamera(const char* posFile, bool* pIsFinish);
	~StartCamera();
private:
	bool LoadData(const char* posFile);
public:
	void Enter(_client_type_ptr p);			//	������
	void Execute(_client_type_ptr p);	//	���s
	void Exit(_client_type_ptr p);			//	�I��
};

//****************************************************************
//	���X�|�[���I�����J����
//****************************************************************
class RespornSelectCamera :public CAMERASTATE
{
private:
	const Vector3 m_TargetPos, m_TargetTarget;
	const Vector3 m_PrePos, m_PreTarget;
	const RATIO m_Speed;
	bool& isMove;
	RATIO m_T;
public:
	RespornSelectCamera(
		CRVector3	TargetPos,
		CRVector3	TargetTarget,
		CRVector3	PrePos,
		CRVector3	PreTarget,
		float		speed,
		bool&		isMove
		);
	~RespornSelectCamera();
public:
	void Enter(_client_type_ptr p);			//	������
	void Execute(_client_type_ptr p);	//	���s
	void Exit(_client_type_ptr p);			//	�I��
};

#endif