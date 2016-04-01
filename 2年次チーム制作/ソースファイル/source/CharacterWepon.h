#ifndef __CHARACTER_WEPON_H__
#define __CHARACTER_WEPON_H__

#include "Character.h"
#include "MeshManager.h"
#include "ColaWater.h"

//#define _DEBUG_WEPON

//*********************************
//�@�L�����N�^�̕���p�����[�^�[
//*********************************
struct WeaponParam
{
	int		rate;				//���ˊԊu
	RATIO	power;				//���݂̃p���[
	float	damage_min;			//�_���[�W
	float	damage_max;			//�_���[�W
	float	range_min;			//�ŏ��˒�����
	float	range_max;			//�ő�˒�����
};

//
////************************************************************
////�@		�L�����N�^�̕���N���X
////************************************************************
//class CharacterWeponBase
//{
//public:
//	enum WEPONSTATE
//	{
//		_WS_FREE = 0, 	//���ˉ\
//		_WS_FIRE,		//���˒�
//		_WS_RELOAD,		//�����[�h��
//		_WS_EMPTY,		//�}�K�W�����c�e�[��
//	};
//protected:
//	LPCHARACTER		m_pOwner;
//	WeaponParam		m_Param;
//public:
//	CharacterWeponBase(
//		LPCHARACTER				pOwner,
//		const WeaponParam&		param);
//	virtual ~CharacterWeponBase(){}
//public:
//	const WeaponParam& GetParam()const;
//	virtual WEPONSTATE GetState() = 0;			//���݂̏�Ԃ𓾂�
//	virtual bool Fire() = 0;					//���݂̃G�C���^�[�Q�b�g�Ɍ����Ĕ���
//	virtual bool Reload() = 0;					//�}�K�W���𖞃^���ɂ���
//	virtual void Update() = 0;					//���t���[���̍X�V
//	virtual void Render(char* shader_n) = 0;	//�r�ɕ`��
//};
//
////************************************************************
////�@�R�[���e�N���X
////************************************************************
//class ColaGun :public CharacterWeponBase
//{
//private:
//	LPMYMESH			m_pMesh;
//	WEPONSTATE			m_state;
//	int					m_timer;
//public:
//	ColaGun(
//		LPCHARACTER				pOwner,
//		const WeaponParam&		param);
//	~ColaGun();
//private:
//	void ChangeState(WEPONSTATE next);
//private:
//	void StateFunc_Free();
//	void StateFunc_Fire();
//	void StateFunc_Reload();
//	void StateFunc_Empty();
//public:
//	WEPONSTATE GetState();
//	bool Fire();
//	void Update();
//	void Render(char* shader_n);
//};
//


class CharacterWeponBase
{
public:
	virtual ~CharacterWeponBase(){}
	virtual bool Fire() = 0;
	virtual void Update() = 0;
	virtual void SetPower(RATIO val) = 0;
	virtual const WeaponParam& GetParam()const = 0;
	virtual void Render(TEAM_TYPE team, bool shuffle) = 0;
};

class ColaGun :public CharacterWeponBase
{
public:
	typedef bool(*COLA_FACTORY_FUNC)(ColaWater**ppOut, LPCHARACTER pOwner, const WeaponParam& wp);
private:
	int						m_count;
	WeaponParam				m_param;
	LPCHARACTER				m_pOwner;
	COLA_FACTORY_FUNC		m_func;
	int						m_shot_effect_count;
	int						m_Sound_timer;
	int						m_NotFire_timer;
public:
	ColaGun(
		const WeaponParam&	p,
		LPCHARACTER			pOwner
		);
	~ColaGun();
public:
	static void InitWeponParam(WeaponParam& out);
	static bool UsualColaFactory(ColaWater**ppOut, LPCHARACTER pOwner, const WeaponParam& wp);
	static void ParamColaFactory(
		ColaWater**ppOut,
		CRVector3 pos,
		DWORD color,
		CRVector3 move,
		float dmg,
		ENTITY_ID id,
		DAMAGEFLAGS dFlg = damage_type_bits::team_cure_damage | damage_type_bits::except_myself | damage_type_bits::enemy_damage);
	static RATIO GetWeponChargePower(LPCHARACTER pOwner,float toEnemyLength);	//�����̋������΂��ɂ͂ǂꂾ����charge���K�v����Ԃ�
public:
	bool Fire();
	void Update();
	void SetPower(RATIO val);
	void SetColawaterFactory(COLA_FACTORY_FUNC func);
	const WeaponParam& GetParam()const;
	void Render(TEAM_TYPE team, bool shuffle);
};

#endif