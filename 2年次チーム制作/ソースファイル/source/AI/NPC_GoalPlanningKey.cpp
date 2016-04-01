#include	"..\All.h"

#pragma region FinalGoal

//�G��|��
planningkey Goal_KnockDownEnemy::Effect(const Symbol& symbol)
{
	return planningkey::last_goal;
}

planningkey Goal_KnockDownEnemy::Prerequisite(const Symbol& symbol)
{

	return planningkey::enemy_down;
}


//�L���|�C���g���Q�b�g����
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

//��@�s��
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

//�񕜂���
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

//��ʊO�̓G�ɑΉ�����i����S�[���j
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

//�ˌ��ōU��
planningkey Goal_WeaponShot_AttackEnemy::Effect(const Symbol& symbol)
{
	return planningkey::enemy_down;
}

planningkey Goal_WeaponShot_AttackEnemy::Prerequisite(const Symbol& symbol)
{

	return (symbol.m_weapon_Charge <= 0) ? planningkey::OK : planningkey::weapon_charged;
}

//����ŉ���
planningkey Goal_NearAttackEnemy::Effect(const Symbol& symbol)
{
	return planningkey::enemy_down;
}

planningkey Goal_NearAttackEnemy::Prerequisite(const Symbol& symbol)
{

	return planningkey::OK;
}

//�O���l�[�h
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

//����`���[�W
planningkey Goal_ChargeWeapon::Effect(const Symbol& symbol)
{
	return planningkey::weapon_charged;
}
planningkey Goal_ChargeWeapon::Prerequisite(const Symbol& symbol)
{
	//�v����(�댯�@�m)
	if (symbol.m_is_Danger != game_id::id_error)
		return planningkey::safe_zone;

	return planningkey::OK;

}

//�B���
planningkey Goal_Hidden::Effect(const Symbol& symbol)
{
	return planningkey::safe_zone;
}

planningkey Goal_Hidden::Prerequisite(const Symbol& symbol)
{
	return planningkey::OK;
}

#pragma endregion
