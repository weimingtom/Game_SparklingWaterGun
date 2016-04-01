#ifndef __AIM_POINT_H__
#define __AIM_POINT_H__

#include "iextreme.h"
#include "ManagerBase.h"
#include "GameSystem\GameBase.h"

enum AIMTARGET_ID
{
	ERR_AIMTARGET_ID = -1
};

//********************************************************
//		�_���ڕW�̏��
//********************************************************
struct AimTargetInfo
{
	Vector3			cli_pos;	//�ڕW�ʒu(���[���h��ԏ�)
	Vector2			scr_pos;	//�ڕW�ʒu(�X�N���[����)
	Vector3			cli_move;	//�P�t���[���Ԃ̈ړ���
	DWORD			color;		//�G�C���̉~�̐F
	int				team_type;	//�`�[���ԍ�(�`�[���ł͂Ȃ��ꍇ��-1)
	float			weight;		//�D��x
	float			size;		//�G�C���̉~�̑傫��
	AIMTARGET_ID	id;			//�G�C���^�[�Q�b�gID
	ENTITY_ID		entity_id;	//ID(error�̏ꍇ����)
};

//********************************************************
//		�_���Ă���҂̏��
//********************************************************
struct AimOwnerInfo
{
	LPCAMERA	pCamera;				//�_���Ă���҂̃J����
	int			team_type;				//�`�[���^�C�v
	Vector3		position;				//�ʒu
	Vector3		view_vec;				//�����x�N�g��
	float		circle_size;			//�F������͈�
	float		range;					//0~1
	float		camera_z;				//�J������Ԃł�Z�l
	ENTITY_ID	id;
};

typedef const AimOwnerInfo&		CRAimOwnerInfo;

//********************************************************
//		�G�C���^�[�Q�b�g�x�[�X�N���X
//********************************************************
class AimTarget
{
protected:
	const AIMTARGET_ID myId;
public:
	AimTarget();
	virtual ~AimTarget();
	virtual bool isTarget(AimTargetInfo &out, CRAimOwnerInfo info) = 0;	//���𓾂� (�^�[�Q�b�g�łȂ��ꍇ��false��Ԃ�)
};

typedef AimTarget* LPAIMTARGET;

//********************************************************
//		�G�C���̉~�̃N���X
//********************************************************
class AimPoint
{
private:
	static const float max_scale;
	LPIEX2DOBJ		m_pTexture;		//�~�e�N�X�`��
	Vector2			m_position2d;	//���W
	Vector3			m_position;
	COLOR			m_color;		//�F
	AimTargetInfo	m_point[2];		//�ړ����
	float			m_next_weight;	//���̃^�[�Q�b�g�̏d��
	float			m_scale;		//���݂̑傫��(���a)
	RATIO			m_hit_reticle;	//�q�b�g���e�B�N���̃A���t�@
	float			m_movearea_size;//�ړ��\�͈�
	float			m_speed;		//�ڕW�Ɉړ������ԃX�s�[�h
	float			m_default_size;	//�ʏ펞�̑傫��(���̑傫���ɏ��X�ɂ��ǂ�)
	float			m_scale_speed;	//���̑傫���ɖ߂�X�s�[�h
	bool			m_change_target;//�T�C�g���ɖڕW����̈ȏア���ꍇ�Ɏ����I�ɖڕW��ς��邩�ǂ����̃t���O
public:
	AimPoint(
		LPIEX2DOBJ	texture,
		float		movearea_size,
		float		default_size,
		float		scale_speed,
		float		move_speed
		);
	~AimPoint();
private:
	//�ړ��ڕW�̏��𓾂�
	void GetGoPosition(
		AimTargetInfo &out,
		CRAimOwnerInfo owner);

	//�������X�V
	void					ReactionUpdate(float target);
	Vector2					GetInCircleRandPos(RATE seed);							//�T�[�N�����̏ꏊ�������_���ɕԂ�
public:
	void					SetChangeTarget(bool changeFlg);						//�T�C�g���ɖڕW����̈ȏア���ꍇ�Ɏ����I�ɖڕW��ς��邩�ǂ���
	const AimTargetInfo&	GetTarget()const{ return m_point[1]; }
	void					GetReactionTargetPos(CRVector3 arm_pos,Vector3 &out);
	CRVector2				GetPosition();											//�T�[�N�����S�̉�ʍ��W�𓾂�
	void					SetReaction(float power);								//�T�[�N����傫������
	void					Update(CRAimOwnerInfo owner);							//�X�V
	void					Render();												//�`��
};

//********************************************************
//		�^�[�Q�b�g���Ǘ�
//********************************************************
static const int AIM_TARGET_MAX = 50;

class AimTargetManager:
	public		ManagerBase<AimTarget, AIM_TARGET_MAX>,
	protected	GameBaseEntity
{
public:
	AimTargetManager();
	~AimTargetManager();
public:
	void GetTarget(AimTargetInfo &out, CRAimOwnerInfo info);
	bool HandleMessage(LPGameMessage msg);
};




#endif