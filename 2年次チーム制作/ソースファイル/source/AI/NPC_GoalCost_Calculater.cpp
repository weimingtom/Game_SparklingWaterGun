#include	"..\All.h"

float goalcost_calculater::NearAttack(Goal* connection_source, 
	NPC_Brain* brain,Character* character, const float& default_Cost)
{

	if (Accesor_Brain::Symbol(brain).m_enemy_nearRange_OfTarget != 
														ENTITY_ID::id_error)
		return default_Cost;

	return 100.0f;
}

float goalcost_calculater::GrenadeAttack(Goal* connection_source,
	NPC_Brain* brain, Character* character, const float& default_Cost)
{
	int num = Rand::Equal_DistanceNum(0, 99);
	if (num <= Accesor_Brain::Param(brain).m_probability_grenede_attack && 
		Accesor_Brain::Final_GoalEvalutor(brain)->Get_TargetID() ==
		Accesor_Brain::Symbol(brain).m_aimRing_Target)
		return 2.0f;
	
	return default_Cost;
}
