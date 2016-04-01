#include	"All.h"

float	GoalEvalutor_AttackEnemy::Calcuate_Score(NPC_Brain* brain, Character* character)
{
	static int	testscore = 200;
	static int game_Frame = 60;	//フレームレート
	//ターゲットは一番近い敵にする
	float dist;
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	int insight_Num;
	MemoryFunc::Get_MostNearPos(brain->memory.memory, my_Pos, true, 1, dist,insight_Num);
	//視界内に敵がいない場合はこのゴールは適応しない
	if (insight_Num <= 0)
		return .0f;

	//射程内に入るまでの時間をかなりおおざっぱに計測
	//dist = sqrtf(dist);
	float in_RangeDist = dist - brain->chara_Param.weapon_Range;
	in_RangeDist = sqrtf(in_RangeDist);
	Vector3 my_Velocity = CharacterUtilityFunctions::GetParam(character).m_move;
	float my_MovePower = my_Velocity.Length();

	float	in_RangeTime = (in_RangeDist / my_MovePower) / (float)game_Frame;
	//今の弾を前弾命中させた時の時間を計測
	//とりあえずテストで
	float	down_Time = 120.0f / (float)game_Frame;
	//スコアレートに変換
	float	total_Time = in_RangeTime + down_Time;
	float score_Rate = (float)testscore / total_Time;

	//リスク計算
	float	risk = 0.5f;
	float ret = score_Rate * ((1.0f + insight_Num) * m_Bias) * (1.0f - risk);
	return ret;
}

Goal*	GoalEvalutor_AttackEnemy::Set_Goal(NPC_Brain* brain)
{
	return brain->final_Goallist[static_cast<int>(goal_tag::weapon_attack)];
}

float	GoalEvalutor_MedicalFriend::Calcuate_Score(NPC_Brain* brain, Character* character)
{
	//回復を必要としている味方の人数を計測
	//距離と人数からスコアを割り出すのがベスト？
	//num_Player* bias * dist

	return .0f;
}

Goal*	GoalEvalutor_MedicalFriend::Set_Goal(NPC_Brain* brain)
{
	return brain->final_Goallist[static_cast<int>(goal_tag::medical_friend)];
}

float	GoalEvalutor_FlugPoint::Calcuate_Score(NPC_Brain* brain, Character* character)
{
	static int flg_Score = 500;
	static Vector3 goal_Pos(.0f, .0f, .0f);
	
	//とりあえず直線距離でおおざっぱに判定する
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	std::vector<Field_Node*> work_vector;
	field_Info->Search_StaticObject(npc_predicate::F_Flug, &work_vector);
	float dist = FLT_MAX;
	Vector3 flg_Pos;
	for (auto it : work_vector)
	{
		float distsq = (it->pos - my_Pos).LengthSq();
		if (distsq < dist)
		{
			dist = distsq;
			flg_Pos = it->pos;
		}
	}
	dist = sqrtf(dist);
	//時間計測
	Vector3 my_Velocity = CharacterUtilityFunctions::GetParam(character).m_move;
	float my_MovePower = my_Velocity.Length();
	float get_PointTime = (dist * 2) / my_MovePower;	//往復するから*2
	float	point_Rate = flg_Score / get_PointTime;

	//risk計算
	//とりあえずテスト
	float	risk = .5f;

	float ret = point_Rate * m_Bias * (1.0f - risk);
	return ret;;
}

Goal*	GoalEvalutor_FlugPoint::Set_Goal(NPC_Brain* brain)
{
	return brain->final_Goallist[static_cast<int>(goal_tag::get_flugpoint)];
}

float	GoalEvalutor_Scouting::Calcuate_Score(NPC_Brain* brain, Character* character)
{
	return .0f;
}

Goal*	GoalEvalutor_Scouting::Set_Goal(NPC_Brain* brain)
{
	return brain->final_Goallist[static_cast<int>(goal_tag::scouting)];
}
