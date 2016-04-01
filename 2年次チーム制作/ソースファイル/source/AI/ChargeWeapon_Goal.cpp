#include	"..\All.h"

Goal_ChargeWeapon::Goal_ChargeWeapon(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director)
:Goal(brain, "ChargeWeapon", evo, goal_tag::charge_weapon,use_director)
{
	m_behavior_Director->Set_Parent(this);
}

bool	Goal_ChargeWeapon::Path_Search(Character* character, const bool& research, Vector3& goal_Pos)
{
	return false;
}

bool	Goal_ChargeWeapon::Active(Character* character)
{
	goal_status status = m_behavior_Director->Direction(m_holder, character, true);

	if (status == goal_status::failed)
		return false;

	return true;
}

goal_status	Goal_ChargeWeapon::Process( Character* character)
{
	goal_status status = Behavior(character);
	return status;
}

void	Goal_ChargeWeapon::Completed( Character* character)
{
//	End(character);
}

void	Goal_ChargeWeapon::Failed( Character* character)
{
//	End(character);
}

goal_status Behavior_Director_ChargeWeapon::Activate(NPC_Brain* brain, Character* character)
{
	//Œ»Ý‚Ìƒ`ƒƒ[ƒW—¦‚ðŽæ“¾
	
	//int current_Charge = (int)((float)ChrFunc::GetUI(character)->GetColaGaugePower() * 100);
	//int need_Charge = Accesor_Brain::Symbol(brain).m_weapon_Charge;
	//if (current_Charge < need_Charge)
	//{
	Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::charge_weapon,
		m_holder, nullptr);
	
	return goal_status::active;
	//}
	//MyAssert(false, "‚±‚±‚Ü‚Å‚­‚é‚Ì‚Í‚¨‚©‚µ‚¢");
}

goal_status Behavior_Director_ChargeWeapon::Direction(NPC_Brain* brain, Character* character,bool init)
{
	if (init)
		return Activate(brain, character);
	return goal_status::completed;
}
