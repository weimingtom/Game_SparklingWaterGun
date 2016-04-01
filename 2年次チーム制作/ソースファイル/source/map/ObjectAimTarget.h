#ifndef __OBJECT_AIM_TARGET_H__
#define __OBJECT_AIM_TARGET_H__

#include "AimPoint.h"

//***********************************************************
//	�X�e�[�W��̃I�u�W�F�N�g�����G�C���^�[�Q�b�g�N���X
//***********************************************************

class ObjectAimTarget: public AimTarget
{
public:
	typedef bool(*isCanAimCharacter)(CRAimOwnerInfo info);	//�L�����N�^�ɂ���đ_�����ǂ����𔻒f����֐�
private:
	Vector3				position;	//�G�C�������ꏊ
	isCanAimCharacter	exp;		//�L�����N�^�ɂ���đ_�����ǂ����𔻒f����֐�
public:
	ObjectAimTarget();
	~ObjectAimTarget();
private:
	bool isTarget(AimTargetInfo &out, CRAimOwnerInfo info);	//���𓾂� (�^�[�Q�b�g�łȂ��ꍇ��false��Ԃ�)
public:
	void SetPos(CRVector3 pos);
	void SetExp(isCanAimCharacter exp);	//�L�����N�^�ɂ���đ_�����ǂ����𔻒f����֐����Z�b�g(�f�t�H���g�ł͂��ׂẴL�����N�^���_����)
};

#endif