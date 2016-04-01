#ifndef __COLA_WATER_H__
#define __COLA_WATER_H__

#include "ColaBullet.h"
#include "ppSpatialPartitionning.h"
#include "DamageHit.h"

//*****************************************************************
//	�R�[���e�ۃN���X�̃w�b�_
//*****************************************************************

//�R�[���e�ۂ̊�{�I�ȃp�����[�^
struct ColaWaterParam
{
	Vector3			pos, move, power;
	Vector3			m_goal_position;//���e�\�z�n�_
	Vector3			m_goal_normal;
	int				m_live_timer;	//�������ԃJ�E���g
	float			m_scale;
	COLOR			color;
	ENTITY_ID		HitObjectId;
};

//�R�[���e�ۃN���X
class ColaWater :public BulletBase
{
private:
	static int			ApperTagCount;
	static int			UpdateTag;
	static const int	UpdateFrame = 10;
	static LVERTEX		lVertex[4];
private:
	EffectClassAnimationNMove*	m_pEffect;
	ColaWaterParam				m_param;		//�p�����[�^�[�\����
	bool						m_DeleteFlag;	//�������邩�ǂ����̃t���O
	const int					m_Updatetag;	//�X�V����
	const int					m_live_time;	//��������
	PolygonEffect::INDEX		m_effect_index;
public:
	DamageObject				m_damage_obj;	//�_���[�W�N���X
public:
	ColaWater(
		const ColaWaterParam&	param,
		int						liveTime,
		LPIEXMESH				pMesh);
	~ColaWater();
private:
	bool HitCheck();
	void GoalPosUpdate();
public:
	static void AddUpdateTag();
	static void SetCameraVector();
	void GetMoveAndPos(Vector3& move, Vector3& pos);
	bool isDelete();
	void Update();
	void Render();
};



#endif