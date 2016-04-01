#include	"..\All.h"

Action_Planner*	NPC_Factory::Create_WeakActionPlanner(NPC_Brain* ret_Brain)
{
	Action_Planner* ret = new Action_Planner(ret_Brain, "DATA\\NPC\\GoalPlanning_GraphCost_Strong.txt");
	ret->m_midlle_Goallist.reserve(goal_tag::NumGoal);
	ret->m_midlle_Goallist.clear();
	ret->m_midlle_Goallist.push_back(new Goal_WeaponShot_AttackEnemy
		(ret_Brain, goalcost_calculater::Test, new Behavior_Director_WeaponShot_AttackEnemy()));
	ret->m_midlle_Goallist.push_back(new Goal_ChargeWeapon
		(ret_Brain, goalcost_calculater::Test, new Behavior_Director_ChargeWeapon()));
	ret->m_midlle_Goallist.push_back(new Goal_Hidden
		(ret_Brain, goalcost_calculater::Test, new Behavior_Director_Hidden()));
	return ret;
}

Action_Planner*	NPC_Factory::Create_NormalActionPlanner(NPC_Brain* ret_Brain)
{
	Action_Planner* ret = new Action_Planner(ret_Brain, "DATA\\NPC\\GoalPlanning_GraphCost_Strong.txt");
	ret->m_midlle_Goallist.reserve(goal_tag::NumGoal);
	ret->m_midlle_Goallist.clear();
	ret->m_midlle_Goallist.push_back(new Goal_WeaponShot_AttackEnemy
		(ret_Brain, goalcost_calculater::Test, new Behavior_Director_WeaponShot_AttackEnemy()));
	ret->m_midlle_Goallist.push_back(new Goal_NearAttackEnemy
		(ret_Brain, goalcost_calculater::NearAttack, new Behavior_Director_NearAttackEnemy()));
	ret->m_midlle_Goallist.push_back(new Goal_ChargeWeapon
		(ret_Brain, goalcost_calculater::Test, new Behavior_Director_ChargeWeapon()));
	ret->m_midlle_Goallist.push_back(new Goal_Hidden
		(ret_Brain, goalcost_calculater::Test, new Behavior_Director_Hidden()));
	return ret;
}


Action_Planner*	NPC_Factory::Create_StrongActionPlanner(NPC_Brain* ret_Brain)
{
	Action_Planner* ret = new Action_Planner(ret_Brain,"DATA\\NPC\\GoalPlanning_GraphCost_Strong.txt");
	ret->m_midlle_Goallist.reserve(goal_tag::NumGoal);
	ret->m_midlle_Goallist.clear();
	ret->m_midlle_Goallist.push_back(new Goal_WeaponShot_AttackEnemy
		(ret_Brain, goalcost_calculater::Test, new Behavior_Director_WeaponShot_AttackEnemy()));
	ret->m_midlle_Goallist.push_back(new Goal_NearAttackEnemy
		(ret_Brain, goalcost_calculater::NearAttack, new Behavior_Director_NearAttackEnemy()));
	ret->m_midlle_Goallist.push_back(new Goal_ChargeWeapon
		(ret_Brain, goalcost_calculater::Test, new Behavior_Director_ChargeWeapon()));
	ret->m_midlle_Goallist.push_back(new Goal_Hidden
		(ret_Brain, goalcost_calculater::Test, new Behavior_Director_Hidden()));
	ret->m_midlle_Goallist.push_back(new Goal_GrenadeAttack
		(ret_Brain, goalcost_calculater::GrenadeAttack, new Behavior_Director_GrenadeAttack()));
	return ret;
}

Goal_Think*	NPC_Factory::Create_WeakGoalThink(NPC_Brain* ret_Brain)
{
	Goal_Think* ret = new Goal_Think(ret_Brain);

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_AttackEnemy(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::knockdown_enemy)]));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_MedicalFriend(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::medical_friend)], ret_Brain->m_chara_Param.m_friendMessageBias_MedicalFriend));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_FollowFriend(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::follow_friend)]));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_GetAdvertisementPoint(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::get_advertsementpoint)]));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_CorrespondenceDenger(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::correspondecne_denger)]));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_Wander_Field(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::wander_field)]));

	return ret;
}

Goal_Think*	NPC_Factory::Create_NormalGoalThink(NPC_Brain* ret_Brain)
{
	Goal_Think* ret = new Goal_Think(ret_Brain);

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_AttackEnemy(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::knockdown_enemy)]));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_MedicalFriend(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::medical_friend)],ret_Brain->m_chara_Param.m_friendMessageBias_MedicalFriend));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_FollowFriend(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::follow_friend)]));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_GetAdvertisementPoint(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::get_advertsementpoint)]));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_CorrespondenceDenger(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::correspondecne_denger)]));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_Wander_Field(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::wander_field)]));

	return ret;
}


Goal_Think*	NPC_Factory::Create_StrongGoalThink(NPC_Brain* ret_Brain)
{
	Goal_Think* ret = new Goal_Think(ret_Brain);

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_AttackEnemy(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::knockdown_enemy)]));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_MedicalFriend(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::medical_friend)],
		ret_Brain->m_chara_Param.m_friendMessageBias_MedicalFriend));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_FollowFriend(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::follow_friend)]));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_GetAdvertisementPoint(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::get_advertsementpoint)]));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_CorrespondenceDenger(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::correspondecne_denger)]));

	ret->m_goal_Evalutor.push_back(new GoalEvalutor_Wander_Field(
		0.5f, ret_Brain->m_final_Goallist[static_cast<int>(goal_tag::wander_field)]));

	return ret;
}



bool	NPC_Factory::WeakBrain(NPC_Brain* ret_Brain)
{
	MyAssert(ret_Brain->m_chara_Param.Load("DATA\\NPC\\chara_param_weak.txt"), "NPCparam‚ª³í‚É“Ç‚Ýž‚ß‚Ü‚¹‚ñ‚Å‚µ‚½");
	ret_Brain->m_planner = Create_WeakActionPlanner(ret_Brain);
	ret_Brain->m_think = Create_WeakGoalThink(ret_Brain);
	ret_Brain->m_defuzzify_param.Load("DATA\\NPC\\fuzzyparam.txt");
	ret_Brain->Intialize_Censor();
	return true;
}

bool	NPC_Factory::NormalBrain(NPC_Brain* ret_Brain)
{
	MyAssert(ret_Brain->m_chara_Param.Load("DATA\\NPC\\chara_param_normal.txt"), "NPCparam‚ª³í‚É“Ç‚Ýž‚ß‚Ü‚¹‚ñ‚Å‚µ‚½");
	ret_Brain->m_planner = Create_NormalActionPlanner(ret_Brain);
	ret_Brain->m_think = Create_NormalGoalThink(ret_Brain);
	ret_Brain->m_defuzzify_param.Load("DATA\\NPC\\fuzzyparam.txt");
	ret_Brain->Intialize_Censor();
	return true;
}

bool	NPC_Factory::StrongBrain(NPC_Brain* ret_Brain)
{

	MyAssert(ret_Brain->m_chara_Param.Load("DATA\\NPC\\chara_param_strong.txt"), "NPCparam‚ª³í‚É“Ç‚Ýž‚ß‚Ü‚¹‚ñ‚Å‚µ‚½");
	ret_Brain->m_planner = Create_StrongActionPlanner(ret_Brain);
	ret_Brain->m_think = Create_StrongGoalThink(ret_Brain);
	ret_Brain->m_defuzzify_param.Load("DATA\\NPC\\fuzzyparam.txt");
	ret_Brain->Intialize_Censor();
	return true;
}

NPC_Brain* NPC_Factory::Create(char* npc_name, const npc_strength& strength, 
													const int npc_num,const int team_type,const ENTITY_ID default_ID)
{
	NPC_Brain* ret = new NPC_Brain(npc_name, npc_num,team_type);

	switch (strength)
	{
	case npc_strength::weak : 
		WeakBrain(ret);
		break;

	case npc_strength::normal :
		NormalBrain(ret);
		break;

	case npc_strength::strong : 
		StrongBrain(ret);
		break;
	}
	MyAssert(npc_commander->Register_NPC(ret, default_ID, team_type), "“o˜^Ž¸”s");
	return ret;
}