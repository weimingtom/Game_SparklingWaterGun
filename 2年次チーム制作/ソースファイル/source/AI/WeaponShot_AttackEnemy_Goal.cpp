#include	"..\All.h"

#pragma region Goal

Goal_WeaponShot_AttackEnemy::Goal_WeaponShot_AttackEnemy
(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director) :
Goal(brain, "ShotWeaponEnemy",evo,goal_tag::shotweapon_enemy,use_director)
{
	m_behavior_Director->Set_Parent(this);
}

bool	Goal_WeaponShot_AttackEnemy::Active(Character* character)
{
	//ターゲットを設定する
	Accesor_Brain::Evalutor(m_holder)->Decision_TargetEnemy(character);
	
	//最終ゴールのターゲットはnullに
	Accesor_Brain::Final_GoalEvalutor(m_holder)->Reset_Target();
#if ASSERT
	MyAssert(IS_CONTROLLOBJ_ID(m_holder->Get_CurrentTarget()), "%sのターゲットのタイプがおかしいです", m_holder->character_Name);
#endif
	if (Accesor_Brain::Current_Target(m_holder) == game_id::id_error)
		return false;
	if (m_behavior_Director->Direction(m_holder, character, true) == goal_status::failed)
		return false;

	status = goal_status::active;
	return true;

}

bool	Goal_WeaponShot_AttackEnemy::Target_IsActive()
{
	Character* target = dynamic_cast<Character*>(IDMANAGER.GetPointer(Accesor_Brain::Current_Target(m_holder)));
	if (!target)
		return false;
	return true;
}

goal_status	Goal_WeaponShot_AttackEnemy::Process(Character* character)
{
	//ターゲットが決まらない場合は失敗(これいらないっぽい)
	if (Accesor_Brain::Current_Target(m_holder) == game_id::id_error)
	{
		MemoryFunction::Remove_PlannningMemory(m_holder, Accesor_Brain::Current_Target(m_holder));
		return goal_status::failed;
	}
	//ターゲットエラーチェック
	if (!Target_IsActive())
		return goal_status::completed;

	//ターゲットのきおくをLostした場合
	float reliability;
	Character_Record* target_Record = m_holder->
																Get_CharacterMemory(Accesor_Brain::Current_Target(m_holder), &reliability);
	if (!target_Record)
	{
		return goal_status::failed;
	}
	goal_status behavior_status = Behavior(character);
	if (behavior_status == goal_status::failed)
	{
		//一時記憶から消去
		if (target_Record->m_update)
			MemoryFunction::Remove_PlannningMemory(m_holder, Accesor_Brain::Current_Target(m_holder));
		else
			//記憶が更新されない場合は死んでいる可能性があるので登録
			m_holder->GetMemory().m_goal_Result->Register(Accesor_Brain::Current_Target(m_holder),false);
	}

	//近接攻撃の射程に入っていた場合は失敗としてプランニングし直す
	if (Accesor_Brain::Evalutor(m_holder)->Target_is_NearRange(m_holder, character))
	{
		//一時記憶から消去
		MemoryFunction::Remove_PlannningMemory(m_holder, Accesor_Brain::Current_Target(m_holder));
		return goal_status::failed;
	}
	return behavior_status;
}

void	Goal_WeaponShot_AttackEnemy::Completed(Character* character)
{
	//記憶にターゲットが死亡したことを知らせる
	ENTITY_ID target = Accesor_Brain::Current_Target(m_holder);
	ai_message->Send_MemoryMessage(m_holder, ai_messagetype::chracter_Die,&target);
	Accesor_Brain::Target_Clear(m_holder);
	status = goal_status::completed;
	End(character);
}

void	Goal_WeaponShot_AttackEnemy::Failed(Character* character)
{
	Accesor_Brain::Target_Clear(m_holder);
	status = goal_status::inactive;
	End(character);
}

bool	Goal_WeaponShot_AttackEnemy::HandleMessage(AI_MESSAGE* msg)
{
	switch (msg->type)
	{
	case ai_messagetype::evalute_currentGoalPos :
		//return Evalute_CurrentGoalPos(static_cast<R_Targetwrapping*>(msg->extra_Info));
	default :
		break;
	}
	return false;
}

#pragma endregion 

#pragma region Director

goal_status Behavior_Director_WeaponShot_AttackEnemy::Activate(NPC_Brain* brain, Character* character)
{
	//距離判定
	float target_dist;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist))
	{
		//ShotWeapon_Circle
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_circle, m_holder,
			behavior_processfunc::shotweapon::AttackCharacter);
		//TOCONTROLL(object)->Change_Behavior(b_num::shot_weapon, character);
		return goal_status::active;
	}
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	Vector3 target_Pos;

	//ターゲットの視界判定
	if (Accesor_Brain::Evalutor(brain)->Target_InSight())
	{
		float reliability;
		const Character_Record* target_Record = brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain),
			&reliability);
		if (!target_Record)
		{
			MyAssert(target_Record, "ターゲットの記憶がない、やばい");
			return goal_status::failed;
		}
		target_Pos = target_Record->m_sensed_Pos;
	}
	else//視界外の場合は位置を予想する
	{
		float reliability;
		if (!Accesor_Brain::Evalutor(brain)->Expectation_TargetPos_Character(brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain), &reliability), &target_Pos, reliability))
		{
#if ASSERT
			MyAssert(false, "attackEnemy::activeでターゲットの記憶が消失しています");
#endif
			return goal_status::failed;
		}
	}
	if (navigation_system->Duplication_Check(ControllObjFunc::GetPos(character), target_Pos))
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_point, m_holder,
			behavior_processfunc::move_point::Range_of_Target);
		ai_message->Send_BehaviorMessage(brain, character, ai_messagetype::set_currentGoalPos, &target_Pos);
		return goal_status::active;
	}
	Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_edge, m_holder,
		behavior_processfunc::move_edge::Range_of_Target);
	Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(), target_Pos,
		Accesor_Brain::Param(brain).m_interval_characterTarget_SearchPath);

	return goal_status::active;
}

goal_status Behavior_Director_WeaponShot_AttackEnemy::MoveEdge(NPC_Brain* brain, Character* character)
{
	float target_dist;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist) &&
		Accesor_Brain::Evalutor(brain)->Target_InSight())
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_circle, m_holder,
			behavior_processfunc::shotweapon::AttackCharacter);
		//TOCONTROLL(object)->Change_Behavior(b_num::shot_weapon, character);
		return goal_status::active;
	}
	return goal_status::failed;
}

goal_status Behavior_Director_WeaponShot_AttackEnemy::MovePoint(NPC_Brain* brain, Character* character)
{
	float target_dist;
	if (Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist) &&
		Accesor_Brain::Evalutor(brain)->Target_InSight())
	{
		Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::shot_weapon_circle, m_holder,
			behavior_processfunc::shotweapon::AttackCharacter);
		//TOCONTROLL(object)->Change_Behavior(b_num::shot_weapon, character);
		return goal_status::active;
	}
	return goal_status::failed;
}

goal_status Behavior_Director_WeaponShot_AttackEnemy::ShotWeapon(NPC_Brain* brain, Character* character)
{
	//Behaviorが成功したのか失敗したのかを取得
	goal_status beahvior_Status = Accesor_Brain::BehaviorManager(brain)->Get_BehaviorStatus();
	if (beahvior_Status == goal_status::completed)
		return beahvior_Status;

	if (beahvior_Status != goal_status::failed)
		MyAssert(false, "failedとcompleted以外でここにくるのはおかしい");

	Vector3 my_Pos = ControllObjFunc::GetPos(character);

	//ターゲットを追うかやめるかどうかを決定する
	//視界から外れて失敗した場合
	if (!Accesor_Brain::Evalutor(brain)->Target_InSight())
	{
		float target_dist;
		Vector3 target_Pos;
		if (!Accesor_Brain::Evalutor(brain)->Target_at_LoseSight(character, &target_Pos, &target_dist))
		{
			return goal_status::failed;
		}
		else
		{
			//target_posとmyposのNode_indexが同じ場合はMoveEdge
			if (navigation_system->Duplication_Check(my_Pos, target_Pos))
			{
				Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_point,
					m_holder, behavior_processfunc::move_point::Range_of_Target);
				ai_message->Send_BehaviorMessage(brain, character, ai_messagetype::set_currentGoalPos,
					&target_Pos);
			}
			else
			{
				Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_edge,
					m_holder, behavior_processfunc::move_edge::Range_of_Target);
				Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(), target_Pos,
					Accesor_Brain::Param(brain).m_interval_characterTarget_SearchPath);
			}
			return goal_status::active;
		}
	}
	//射程から外れて失敗した場合
	float target_dist;
	if (!Accesor_Brain::Evalutor(brain)->Range_of_Traget(character, &target_dist))
	{
		int current_charge = (int)((float)ChrFunc::GetUI(character)->GetColaGaugePower() * 100);
		target_dist = sqrtf(target_dist);
		float desiraliblity = Accesor_Brain::FuzzyTool(brain)->Calculate_Sight_Target_Character(target_dist, (float)current_charge);
		if (desiraliblity > 60.0f)
		{
			float reliability;
			Character_Record* record = brain->Get_CharacterMemory(Accesor_Brain::Current_Target(brain), &reliability);
			MyAssert(record, "ここで落ちるのはやばい");
			if (navigation_system->Duplication_Check(my_Pos, record->m_sensed_Pos))
			{
				//同じのノードの場合は失敗、近接攻撃になるようにしたい
				return goal_status::failed;
			}
			else
			{
				Accesor_Brain::BehaviorManager(brain)->Change_Behavior(character, behavior_tag::move_edge,
					m_holder, behavior_processfunc::move_edge::Range_of_Target);
				Accesor_Brain::PathPlanner(brain)->Activate(character, m_holder->Get_Tag(), record->m_sensed_Pos,
					Accesor_Brain::Param(brain).m_interval_characterTarget_SearchPath);
				return goal_status::active;
			}
	
		}
		else
			return goal_status::failed;
	}
	MyAssert(false, "ここまではこないはずなのに");
	return goal_status::failed;
}

goal_status Behavior_Director_WeaponShot_AttackEnemy::Direction(NPC_Brain* brain,
	Character* character, bool init)
{
	if (init)
		return Activate(brain, character);

	goal_status ret = goal_status::error;

	switch (Accesor_Brain::BehaviorManager(brain)->Get_Current_Tag())
	{
	case behavior_tag::move_edge:
		ret = MoveEdge(brain, character);
		break;

	case	behavior_tag::move_point:
		ret = MovePoint(brain, character);
		break;

	case behavior_tag::shot_weapon_circle:
		ret = ShotWeapon(brain, character);
		break;
	}
	return ret;
}

#pragma endregion

