#include	"..\All.h"

#pragma region FinalGoal

//敵を倒す
planningkey Goal_KnockDownEnemy::Effect(const Symbol& symbol)
{
	return planningkey::last_goal;
}

planningkey Goal_KnockDownEnemy::Prerequisite(const Symbol& symbol)
{

	return planningkey::enemy_down;
}


//広告ポイントをゲットする
planningkey Goal_GetAdvertisementPoint::Effect(const Symbol& symbol)
{
	return planningkey::last_goal;
}

planningkey Goal_GetAdvertisementPoint::Prerequisite(const Symbol& symbol)
{
	if (symbol.m_weapon_Charge != -1)
		return planningkey::weapon_charged;

	return planningkey::OK;
}

//偵察行動
planningkey Goal_Scouting::Effect(const Symbol& symbol)
{
	return planningkey::last_goal;
}

planningkey Goal_Scouting::Prerequisite(const Symbol& symbol)
{
	if (symbol.m_im_Fine)
		return planningkey::OK;
	else
		return planningkey::im_fine;
}

//回復する
planningkey Goal_MedicalFriend::Effect(const Symbol& symbol)
{
	return planningkey::last_goal;
}

planningkey Goal_MedicalFriend::Prerequisite(const Symbol& symbol)
{
	if (symbol.m_down_Friend != -1)
	{
		if (symbol.m_weapon_Charge <= 0)
			return planningkey::OK;
		else
			return planningkey::weapon_charged;
	}

	return planningkey::error;
}

//画面外の敵に対応する（特殊ゴール）
planningkey Goal_CorrespondenceDenger::Effect(const Symbol& symbol)
{
	return planningkey::last_goal;
}

planningkey Goal_CorrespondenceDenger::Prerequisite(const Symbol& symbol)
{
	return planningkey::OK;
}


planningkey Goal_WanderField::Effect(const Symbol& symbol)
{
	return planningkey::last_goal;
}

planningkey Goal_WanderField::Prerequisite(const Symbol& symbol)
{
	if (symbol.m_im_Fine)
		return planningkey::OK;
	else
		return planningkey::im_fine;
}

planningkey Goal_FollowFriend::Effect(const Symbol& symbol)
{
	return planningkey::last_goal;
}

planningkey Goal_FollowFriend::Prerequisite(const Symbol& symbol)
{
	return planningkey::OK;
}


#pragma endregion

#pragma region MidlleGoal

//射撃で攻撃
planningkey Goal_WeaponShot_AttackEnemy::Effect(const Symbol& symbol)
{
	return planningkey::enemy_down;
}

planningkey Goal_WeaponShot_AttackEnemy::Prerequisite(const Symbol& symbol)
{

	return (symbol.m_weapon_Charge <= 0) ? planningkey::OK : planningkey::weapon_charged;
}

//武器で殴る
planningkey Goal_NearAttackEnemy::Effect(const Symbol& symbol)
{
	return planningkey::enemy_down;
}

planningkey Goal_NearAttackEnemy::Prerequisite(const Symbol& symbol)
{

	return planningkey::OK;
}

//グレネード
planningkey Goal_GrenadeAttack::Effect(const Symbol& symbol)
{
	if (symbol.m_weapon_Charge == -1)
		return planningkey::enemy_down;
	return planningkey::error;
}

planningkey Goal_GrenadeAttack::Prerequisite(const Symbol& symbol)
{
	return planningkey::OK;
}

//武器チャージ
planningkey Goal_ChargeWeapon::Effect(const Symbol& symbol)
{
	return planningkey::weapon_charged;
}
planningkey Goal_ChargeWeapon::Prerequisite(const Symbol& symbol)
{
	//要改良(危険察知)
	if (symbol.m_is_Danger != game_id::id_error)
		return planningkey::safe_zone;

	return planningkey::OK;

}

//隠れる
planningkey Goal_Hidden::Effect(const Symbol& symbol)
{
	return planningkey::safe_zone;
}

planningkey Goal_Hidden::Prerequisite(const Symbol& symbol)
{
	return planningkey::OK;
}

#pragma endregion
