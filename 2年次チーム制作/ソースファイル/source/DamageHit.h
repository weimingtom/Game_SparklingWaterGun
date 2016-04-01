#ifndef __DAMAGE_HIT_H__
#define __DAMAGE_HIT_H__

#include "iextreme.h"
#include "ppSpatialPartitionning.h"
#include "GameSystem\GameBase.h"

class DamageObjectManager;
//*******************************************************
//	�_���[�W�N���X
//*******************************************************

//�_���[�W���ʃt���O
typedef DWORD DAMAGEFLAGS;

//�r�b�g�ɂ��^�C�v����
namespace damage_type_bits
{
	enum
	{
		//�N�ւ̃_���[�W��
		all_character_cure =	(1 << 30),	//�G������킸��
		team_cure_damage =		(1 << 29),	//�������񕜂�����_���[�W
		all_character_damage =	(1 << 28),	//�G������킸�_���[�W
		enemy_damage =			(1 << 27),	//�G�ւ̃_���[�W

		//�I�v�V����
		except_myself =			(1 << 26),	//�����͏���
		apper_crushed_mesh =	(1 << 25),

		//�o���G�[�V����
		to_dying_damage =		(1 << 24),	//�m����Ԃ�(���ɂȂ��Ă���ꍇ�͂��̂܂�)
		blow_off_damage =		(1<<23),	//�������
	};
}

#define IS_TYPE_DAMAGE(flags,damage_type) ((flags)&(damage_type))

//�_���[�W�p�����[�^
struct DamageParam
{
	ENTITY_ID		parent;		//������ID
	DAMAGEFLAGS		type;		//�_���[�W���ʃt���O
	float			val;		//�l
	int				hit_count;	//����������
	Vector3			vec, pos;	//�����ƌ���
	COLOR			color;
};

//�_���[�W�N���X
class DamageObject
{
private:
	DamageObjectManager*	pMgr;
	lp_sp_object			sp_obj;
	Vector3					pos;
	float					size;
public:
	DamageParam				param;
public:
	DamageObject(DamageObjectManager*	pMgr);
	~DamageObject();
public:
	void Apper();	//�_���[�W�����L���ɂ���
	void Exit();	//�_���[�W����𖳌��ɂ���
public:
	bool isEnable()const;
	CRVector3 GetPos()const;
	float GetSize()const;
	void SetPos(CRVector3 pos);	
	void SetSize(float s);
};

//*******************************************************
//	�_���[�W�}�l�[�W��
//*******************************************************
typedef std::vector<DamageObject*> DAMAGEVECTOR;

class DamageObjectManager:public GameBaseEntity
{
private:
	lp_sp_field m_pField;
public:
	DamageObjectManager();
	~DamageObjectManager();
public:
	void RenderDebugParticle();
	lp_sp_field	GetField()const;
	DAMAGEVECTOR GetInDamage(CRVector3 pos, float size);	//�͈͂���`�ɂƂ�̂Œ���
	bool HandleMessage(LPGameMessage msg);
};

//�_���[�W���ʕ֗��֐�
namespace damage_check
{
	bool isCureDamage(LPCONTROLLOBJCT me, const DamageParam& param);
	bool isEnemyDamage(LPCONTROLLOBJCT me, const DamageParam& param);
}

#endif