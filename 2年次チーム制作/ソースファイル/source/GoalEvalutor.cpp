#include	"All.h"

float	GoalEvalutor_AttackEnemy::Calcuate_Score(NPC_Brain* brain, Character* character)
{
	static int	testscore = 200;
	static int game_Frame = 60;	//�t���[�����[�g
	//�^�[�Q�b�g�͈�ԋ߂��G�ɂ���
	float dist;
	Vector3 my_Pos = ControllObjFunc::GetPos(character);
	int insight_Num;
	MemoryFunc::Get_MostNearPos(brain->memory.memory, my_Pos, true, 1, dist,insight_Num);
	//���E���ɓG�����Ȃ��ꍇ�͂��̃S�[���͓K�����Ȃ�
	if (insight_Num <= 0)
		return .0f;

	//�˒����ɓ���܂ł̎��Ԃ����Ȃ肨�������ςɌv��
	//dist = sqrtf(dist);
	float in_RangeDist = dist - brain->chara_Param.weapon_Range;
	in_RangeDist = sqrtf(in_RangeDist);
	Vector3 my_Velocity = CharacterUtilityFunctions::GetParam(character).m_move;
	float my_MovePower = my_Velocity.Length();

	float	in_RangeTime = (in_RangeDist / my_MovePower) / (float)game_Frame;
	//���̒e��O�e�������������̎��Ԃ��v��
	//�Ƃ肠�����e�X�g��
	float	down_Time = 120.0f / (float)game_Frame;
	//�X�R�A���[�g�ɕϊ�
	float	total_Time = in_RangeTime + down_Time;
	float score_Rate = (float)testscore / total_Time;

	//���X�N�v�Z
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
	//�񕜂�K�v�Ƃ��Ă��閡���̐l�����v��
	//�����Ɛl������X�R�A������o���̂��x�X�g�H
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
	
	//�Ƃ肠�������������ł��������ςɔ��肷��
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
	//���Ԍv��
	Vector3 my_Velocity = CharacterUtilityFunctions::GetParam(character).m_move;
	float my_MovePower = my_Velocity.Length();
	float get_PointTime = (dist * 2) / my_MovePower;	//�������邩��*2
	float	point_Rate = flg_Score / get_PointTime;

	//risk�v�Z
	//�Ƃ肠�����e�X�g
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
