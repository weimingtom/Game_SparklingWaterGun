#ifndef __SHARE_RATIO_OBJECT_H__
#define __SHARE_RATIO_OBJECT_H__

#include "ShareRatioObjectAim.h"
#include "MapObject.h"
#include "../DamageHit.h"
#include "MapObjectParam.h"
#include "../ShareObjIcon.h"
#include "MapObjectFactory.h"

//*************************************************************
// �V�F�A�I�u�W�F�N�g��{�N���X
//*************************************************************

class ShareRatioObjectBase 
	:public MapObjectInterface
{
protected:
	ShareObjIcon*              m_pShareObjIcon;  // �A�C�R��
	ShareRatioParam          m_ShareRatio;        // �V�F�A�p�����[�^
	ShareRatioObjectAim    aim_target_;           // �G�C���^�[�Q�b�g
	float                              collision_size_;        // ����̃T�C�Y
	Vector3                         damage_pos_;        // ����̒��S���W
public:
	ShareRatioObjectBase(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	virtual~ShareRatioObjectBase();
private:
	void UpdateShareValueCure();
	bool CalcHitDamage(DamageObject* p);
	void UpdateColor();
	// �_���[�W����
	DAMAGEFLAGS HitCheck();
	static bool SetChangeSourEffect(CRVector3 pos, ENTITY_ID parentId);
protected:
	static void SetChangeShareIcon(CRVector3 pos, ENTITY_ID parentId);
	void SetChangeTeamEffect(
		bool set_water_pool = true,
		bool set_smoke = true
		);
	virtual void ChangeTeam(DamageObject* p) = 0;
	virtual void Hit(DamageObject* p) = 0;
public:
	static RATIO GetSeeThroughAlpha();
	// �X�V
	virtual void Update()override;
	virtual Vector3 Get_MoveParameter()const{ return VECTOR3ZERO; };
	// �Q�b�^
	CRVector3 Get_DamagePos()const{ return damage_pos_; }
	const ShareRatioParam& Get_ShareParam()const;

};

//*************************************************************
// �|�X�^�[�N���X
//*************************************************************

class PosterShareRatioObject 
	:public ShareRatioObjectBase
{
public:
	PosterShareRatioObject(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);
	~PosterShareRatioObject();
public:
	void Hit(DamageObject* p)override;

	// �`�[���ύX
	void ChangeTeam(DamageObject* p)override;

	// �e�̉e�����󂯂�
	void ParentMove(
		LPCMATRIX pParentWorldMat,
		LPCMATRIX pParentWorldMatInv,
		LPCMATRIX pParentMoveMat,
		LPCMATRIX AffineMat);

	// �X�V�E�`��
	void Update()override;
	void Render()override;
	void Render(char* shaderName)override;
	void Render_ShadowMap()override;

	//RayPick
	virtual bool RayPick(
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
// �|�X�^�[�쐬�N���X
//*************************************************************
class PosterFactory
	:public MapObjectFractory
{
public:
	bool CreateNewObject(const ObjectData &data)override;
};

#endif // __SHARE_RATIO_OBJECT_H__

// EOF