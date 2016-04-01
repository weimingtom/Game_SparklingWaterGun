#ifndef __NPC_ENUM_H__
#define __NPC_ENUM_H__


#include "../EnumCP11.h"

#define NPC_ASSART 0

_ENUM_BEGIN(goal_tag,GOAL_TAG)

	//�ŏI�S�[��
	error = -1,
	knockdown_enemy,
	get_advertsementpoint,	//�L���|�C���g
	follow_friend,
	medical_friend,
	correspondecne_denger,
	wander_field,
	//�������璆�ԃS�[��
	shotweapon_enemy,
	nearattack_enemy,
	charge_weapon,
	hidden,
	grenede_attack,
	NumGoal,
	//�����������S�[��
	avoidancevessel,
	//follow_friend,
	
_ENUM_END(goal_tag, GOAL_TAG)

_ENUM_BEGIN(planningkey,PLANNNINGKEY)
	error = -10,
	last_goal = -3,
	OK = -1,
	enemy_down,
	weapon_charged,			//����`���[�W
	safe_zone,							//���S�n�тɂ���
	im_fine,								//���C���ǂ���
_ENUM_END(planningkey, PLANNNINGKEY)

_ENUM_BEGIN( goal_status,GOAL_STATUS)
	error = -1,
	active,			//���ݎ��s��
	completed,	//�B���ς�
	failed,			//���s
	inactive,		//�ҋ@��	
_ENUM_END(goal_status, GOAL_STATUS)

_ENUM_BEGIN(expectation_act,EXPECTATION_ACT)
	none,					//�������Ȃ��imapobject�j
	i_dont_know,
	get_publicity_point,	//�L���|�C���g���Ƃ낤�Ƃ��Ă���
	attack_me,			//�������U�����悤�Ƃ��Ă���
	move_field,		//�t�B�[���h���ړ����Ă���i��L�̗\���ɓ��Ă͂܂�Ȃ��Ƃ��͂Ƃ肠��������j
	dieing,				//���ɂ����Ă���
	die,					//���S���Ă���
_ENUM_END(expectation_act, EXPECTATION_ACT)

enum class mapobjcet_type
{
	error = -1,
	denger_object,
	poster,
	num_type
};

enum class partculargoal_type
{
	avoidancevessel,
	airborne,
	num_partculargoal,
};

#endif
