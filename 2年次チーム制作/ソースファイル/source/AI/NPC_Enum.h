#ifndef __NPC_ENUM_H__
#define __NPC_ENUM_H__


#include "../EnumCP11.h"

#define NPC_ASSART 0

_ENUM_BEGIN(goal_tag,GOAL_TAG)

	//最終ゴール
	error = -1,
	knockdown_enemy,
	get_advertsementpoint,	//広告ポイント
	follow_friend,
	medical_friend,
	correspondecne_denger,
	wander_field,
	//ここから中間ゴール
	shotweapon_enemy,
	nearattack_enemy,
	charge_weapon,
	hidden,
	grenede_attack,
	NumGoal,
	//ここから特殊ゴール
	avoidancevessel,
	//follow_friend,
	
_ENUM_END(goal_tag, GOAL_TAG)

_ENUM_BEGIN(planningkey,PLANNNINGKEY)
	error = -10,
	last_goal = -3,
	OK = -1,
	enemy_down,
	weapon_charged,			//武器チャージ
	safe_zone,							//安全地帯にいる
	im_fine,								//元気かどうか
_ENUM_END(planningkey, PLANNNINGKEY)

_ENUM_BEGIN( goal_status,GOAL_STATUS)
	error = -1,
	active,			//現在実行中
	completed,	//達成済み
	failed,			//失敗
	inactive,		//待機中	
_ENUM_END(goal_status, GOAL_STATUS)

_ENUM_BEGIN(expectation_act,EXPECTATION_ACT)
	none,					//何もしない（mapobject）
	i_dont_know,
	get_publicity_point,	//広告ポイントをとろうとしている
	attack_me,			//自分を攻撃しようとしている
	move_field,		//フィールドを移動している（上記の予測に当てはまらないときはとりあえずこれ）
	dieing,				//死にかけている
	die,					//死亡している
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
