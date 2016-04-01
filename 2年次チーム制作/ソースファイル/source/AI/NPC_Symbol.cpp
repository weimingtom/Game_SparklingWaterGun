#include	"..\All.h"

void	Symbol::Initialize()
{
	m_enemy_InSight = false;
	m_enemy_range_OfTarget = false;
	m_importance_Sound = false;
	m_weapon_Charge = false;
	m_near_Friend = false;
	m_win_Now = false;
	m_pinch_Friend = game_id::id_error;
	m_down_Friend = game_id::id_error;
	m_is_Danger = game_id::id_error;
	m_enemy_nearRange_OfTarget = ENTITY_ID::id_error;
	m_aimRing_Target = ENTITY_ID::id_error;
	m_im_Fine = true;
	m_attack_Me = false;
	m_attack_OutSight = game_id::id_error;
}

void	Symbol::Update_Target_Enemy(NPC_Brain* brain, Character* character)
{
	//GoalAttackEnemy‚É•]‰¿‚³‚ê‚½“G‚ª‚¢‚½ê‡‚»‚Ìî•ñ‚ğŠi”[‚·‚é
	
	ENTITY_ID target_ID = Accesor_Brain::Final_GoalEvalutor(brain)->Get_TargetID();
	if (target_ID != ENTITY_ID::id_error)
	{
		float reliability;
		Character_Record* target_m = brain->Get_CharacterMemory(target_ID, &reliability);
		if (!target_m)
		{
#if ASSERT
			MyAssert(false, "Symbol::Update_Target_Enemy‚É‚¨‚¢‚Äƒ^[ƒQƒbƒg‚Ì‚«‚¨‚­‚ª‘¶İ‚µ‚Ü‚¹‚ñ", (int)target_ID);
#endif
			m_enemy_InSight = -1.0f;
			m_enemy_range_OfTarget = -1.0f;
			return;
		}
		Vector3 my_Pos = ControllObjFunc::GetPos(character);
		float dist = (target_m->m_sensed_Pos - my_Pos).Length();
		m_enemy_InSight = (target_m->m_visibilily) ? dist : -1.0f;
		float current_WeaponRange = ChrFunc::GetNowChargeRange(character);
		m_enemy_range_OfTarget = (current_WeaponRange >= dist) ? dist : -1.0f;

		return;
	}
	//Šî–{“I‚É‹ŠE‚É“G‚ª‚¢‚é‚©A‚Ü‚½‚ÍË’ö“à‚É‚¢‚é‚©‚Å”»’f‚·‚é
	std::map<ENTITY_ID, Character_Record*>* map_Ptr = &brain->GetMemory().m_character_Record;
	int current_Frame = GAMEMATCHINFO.GetMatchTime();
	float temp_Dist = FLT_MAX;
	float temp_Range = FLT_MAX;
	bool	insight_Flg = false;
	for (auto it = map_Ptr->begin(); it != map_Ptr->end(); it++)
	{
		//g‚¦‚È‚¢‹L‰¯‚Í”ò‚Î‚·
		float reliability;
		
		if (!it->second->Can_Use(current_Frame, Accesor_Brain::Param(brain).record_character_ReliabilityTime, &reliability))
			continue;
		//‹ŠEŠO‚Í”ò‚Î‚·
		if (!it->second->m_visibilily)
			continue;

		insight_Flg = true;
		//‹——£”»’è
		Vector3 my_Pos = ControllObjFunc::GetPos(character);
		float dist = (my_Pos - it->second->m_sensed_Pos).Length();
		if (temp_Dist > dist)
		{
			m_enemy_InSight = dist;
			temp_Dist = dist;
		}
		float current_WeaponRange = ChrFunc::GetNowChargeRange(character);
		if (dist > current_WeaponRange)
			continue;
		if (temp_Range > dist)
		{
			m_enemy_range_OfTarget = dist;
			temp_Range = dist;
		}
		break;
	}
	//‹ŠE“à‚É“G‚ª‚¢‚È‚¢ê‡
	if (!insight_Flg)
	{
		m_enemy_InSight = -1.0f;
		m_enemy_range_OfTarget = -1.0f;
	}
}

void	Symbol::Update_Importance_Sound(NPC_Brain* brain, Character* character)
{

}

void	Symbol::Update_Weapon_Charge(NPC_Brain* brain, Character* character)
{
	static int max_weaponcharge = 100;
	//Œ»İ‚Ìcharge—¦
	int current_Chagerate = (int)((float)ChrFunc::GetUI(character)->GetColaGaugePower() * 100);
	//UŒ‚–Ú•W‚Æ‚µ‚Ä•]‰¿‚³‚ê‚Ä‚¢‚é“G‚ª‚¢‚é‚©‚ğŠm”F
	ENTITY_ID target = Accesor_Brain::Final_GoalEvalutor(brain)->Get_TargetID();
	if (target == game_id::id_error)
	{
		//ˆê’èˆÈ‰º‚È‚ç“K“–‚É’™‚ß‚é
		if (current_Chagerate < Accesor_Brain::Param(brain).feel_few_WeaponCharge)
		{
			m_weapon_Charge = Rand::Equal_DistanceNum(Accesor_Brain::Param(brain).feel_few_WeaponCharge - current_Chagerate,
														max_weaponcharge) + 30;
			m_weapon_Charge = min(100, m_weapon_Charge);
			return;
		}
		m_weapon_Charge = -1;
		return;
	}
	float reliability;
	Vector3 target_Pos;
	if (IS_CONTROLLOBJ_ID(target))
	{
		Character_Record* target_M = brain->Get_CharacterMemory(target, &reliability);
		if (!target_M)
		{
			m_weapon_Charge = -1;
			return;
		}
		target_Pos = target_M->m_sensed_Pos;
		//BodyCentor•ª‚‚­
		target_Pos.y += 0.7f;
	}
	else
	{
		MapObject_Record* target_M = brain->Get_MapObjectMemory(target, &reliability);
		if (!target_M)
		{
			m_weapon_Charge = -1;
			return;
		}
		target_Pos = target_M->m_sensed_Pos;
	}
	if (current_Chagerate <= 0)
	{
		m_weapon_Charge = (Accesor_Brain::Param(brain).feel_few_WeaponCharge - current_Chagerate) + 30;
		m_weapon_Charge = min(100, m_weapon_Charge);
		return;
	}

	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	Vector3 vec = target_Pos - my_Pos;
	float xz_dist = sqrtf(vec.x * vec.x + vec.z* vec.z);
	float move_power = ChrFunc::GetNowChargeRange(character) / 120.0f;
	float time = xz_dist / move_power;
	vec.Normalize();
	vec *= move_power;
	float gravity = 0.001f;

	//‹——£‚É‘Î‚µ‚Ä‚Ì—‰º“_‚ğ—\‘ª‚·‚é
	float y_pos = my_Pos.y + vec.y * time + (0.5f * -gravity *time* time);
	if (y_pos + 1.0f >= target_Pos.y)
	{
		m_weapon_Charge = -1;
	}
	else
		m_weapon_Charge = min(100, current_Chagerate + 30);
	
	//float need_ChargeRate = ColaGun::GetWeponChargePower(character, dist);
	//if (current_Chagerate <= need_ChargeRate)
	//{
	//	//­‚µ‚¾‚¯ãæ‚¹‚·‚é
	//	need_ChargeRate = (Min(need_ChargeRate + 0.1f, 1.0f)*100.0f);
	//	m_weapon_Charge = (int)need_ChargeRate;

	//	return;
	//}
	////‚Æ‚è‚ ‚¦‚¸‚Ìˆ—
	//if (m_weapon_Charge <= brain->Get_Param().feel_few_WeaponCharge)
	//{
	//	m_weapon_Charge = brain->Get_Param().feel_few_WeaponCharge;
	//	return;
	//}
	//m_weapon_Charge = -1;
}

void	Symbol::Update_Friend(NPC_Brain* brain, Character* character)
{

}

void	Symbol::Update_IsDanger(NPC_Brain* brain, Character* character)
{
	int current_Frame = GAMEMATCHINFO.GetMatchTime();
	//‰æ–ÊŠO‹L‰¯‚ÌQÆ
	const MEMORY* mem = &brain->GetMemory();

	if (!mem->m_threat_Outsight.empty())
	{
		m_is_Danger = mem->m_threat_Outsight.front().m_id;
		return;
	}

	int num_Crisis = 0;
	//‚Æ‚è‚ ‚¦‚¸
	static const int player_Num = 10;
	Character_Record* crisis_Record[player_Num] = { nullptr };
	for (auto cur_Memory = mem->m_character_Record.begin();
		cur_Memory != mem->m_character_Record.end();
		++cur_Memory)
	{
		//–¡•û‚Í”ò‚Î‚·
		//if (cur_Memory.second->team_No ==)
		//continue;
		//©•ª‚ğUŒ‚‚µ‚Ä‚¢‚é“G‚ª‚¢‚½ê‡‚Ítrue
		int	memory_leliability_Time = Accesor_Brain::Param(brain).record_character_ReliabilityTime;
		if (cur_Memory->second->m_action.m_tag == expectation_act::attack_me)
		{
			for (int i = 0; i < player_Num; i++)
			{
				float reliability;
				if (!crisis_Record[i]/* && cur_Memory->second->Can_Use(current_Frame, memory_leliability_Time, &reliability)*/)
				{
					if (!cur_Memory->second->Can_Use(current_Frame, memory_leliability_Time, &reliability))
					{
#if NPC_ASSART
						MyAssert(false, "‚±‚ê‚Í‚â‚Î‚¢");
#endif
						continue;
					}
					crisis_Record[i] = cur_Memory->second;
					num_Crisis++;
				}
			}
			
		}
	}
	//ˆê”Ô‹ºˆĞ‚Æ‚È‚Á‚Ä‚¢‚é“G‚ÌEINTIY_ID‚ğ•Ô‹p‚·‚é
	if (num_Crisis == 0)
	{
		m_is_Danger = ENTITY_ID::id_error;
		return;
	}
	if (num_Crisis == 1)
	{
		m_is_Danger = crisis_Record[0]->m_id;
		return;
	}

	float dist = FLT_MAX;
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	//‚Æ‚è‚ ‚¦‚¸ˆê”Ô‹ß‚¢“G‚ğ‘ÎÛ‚Æ‚·‚é
	for (int i = 0; i < num_Crisis; i++)
	{
		float d_sq = (my_Pos - crisis_Record[i]->m_sensed_Pos).LengthSq();
		if (d_sq < dist)
		{
			m_is_Danger = crisis_Record[i]->m_id;
			dist = d_sq;
		}
	}
}

void	Symbol::Update_Helth(NPC_Brain* brain, Character* character)
{
	m_im_Fine = true;
}

bool	Symbol::Update(NPC_Brain* brain, Character* character)
{
	Update_Target_Enemy(brain, character);
	Update_NearRangeTarget(brain, character);
	Update_Importance_Sound(brain, character);
	Update_Weapon_Charge(brain, character);
	Update_Friend(brain, character);
	Update_Helth(brain, character);
	Update_OutSight(brain, character);
	return true;
}

void	Symbol::Update_OutSight(NPC_Brain* brain, Character* character)
{
	m_attack_OutSight = false;
	for (auto it = brain->GetMemory().m_character_Record.begin();
		it != brain->GetMemory().m_character_Record.end(); it++)
	{
		if ((it)->second->m_action.m_tag == expectation_act::attack_me &&
			!(it)->second->m_visibilily)
		{
			m_attack_OutSight = true;
			return;
		}
	}
}

void	Symbol::Update_NearRangeTarget(NPC_Brain* brain, Character* character)
{
	int	current_Frame = GAMEMATCHINFO.GetMatchTime();
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	float most_min_dist = -1.0f;
	TEAM_TYPE my_team = ControllObjFunc::GetMatchData(character)->team_type;
	int reliability_time = Accesor_Brain::Param(brain).record_character_ReliabilityTime;
	float feel_NearDist = Accesor_Brain::Param(brain).m_feel_NearRangeDist;
	for (auto& cur_mem : brain->GetMemory().m_character_Record)
	{
		float reliabiliry;
		if (!cur_mem.second->Can_Use(current_Frame, reliability_time, &reliabiliry))
			continue;
		if (reliabiliry <= 0.6f)
			continue;
		if (cur_mem.second->m_status == arive_status::dead)
			continue;
		if (cur_mem.second->m_team_Type == my_team)
			continue;
		if (!cur_mem.second->m_line_of_Fire)
			continue;

		float dist_sq = (cur_mem.second->m_sensed_Pos - my_Pos).LengthSq();
		if (dist_sq > feel_NearDist)
			continue;

		//‰‚ß‚Ä‚ÌƒP[ƒX
		if (most_min_dist < 0)
		{
			most_min_dist = dist_sq;
			m_enemy_nearRange_OfTarget = cur_mem.second->m_id;
			continue;
		}
		//‹——£”äŠr
		if (dist_sq < most_min_dist)
		{
			most_min_dist = dist_sq;
			m_enemy_nearRange_OfTarget = cur_mem.second->m_id;
		}
	}

}

void	Symbol::Update_AimRingTarget(NPC_Brain* brain, Character* character)
{
	//Aimî•ñæ“¾
	AimTargetInfo info;
	ChrFunc::GetUI(character)->GetTarget(info);
	m_aimRing_Target = info.entity_id;
	
}