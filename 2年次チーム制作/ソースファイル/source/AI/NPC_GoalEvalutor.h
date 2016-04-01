#ifndef __NPC_GOALEVALUTOR_H__
#define __NPC_GOALEVALUTOR_H__

class NPC_Brain;
class Character;
class Goal;
class Goal_Think;


//�S�̂̕]����
//��莞�ԂɊ��҂����X�R�A�l * Bias * (1.0f - �댯�x) 
//						S						*				B		*		R


//�U���̕]����
//(�G�����j�������ɓ�����X�R�A / �G��|���̂ɂ�����\������)* (Bias) * (1.0 - �댯�x)
//																S						*	B * R																									

//�L���|�C���g�̕]����
//�i�l���ł���ő�Rate�j * (�X�R�Arate )�@/ (����Rate�ɂł���܂ł����鎞��) * Bias * (1.0f - �댯�x)
//(0~2.0)

//��@�̕]����(������������ԑ��)
//�i���̎�ނɂ��Rate	 / �����j* �e���� * bias;


#pragma region GOAL_EVALUTOR

struct Lock
{
	bool	m_flg;						//lock����Ă邩�ǂ���
	int	m_lock_Time;			//lock���ꂽ�t���[��
	const int	m_unlock_Frame;	//���������t���[����

	Lock(const int& unlock_Time) : m_flg(false), m_unlock_Frame(unlock_Time),
		m_lock_Time(0)
	{	}

	bool	Is_Bind(const int current_Frame)
	{
		if (!m_flg) return false;
		
		if ((current_Frame - m_lock_Time) >= m_unlock_Frame)
			m_flg = false;

		return m_flg;
	}
	void	Bind(const int current_Frame)
	{
		m_lock_Time = current_Frame;
		m_flg = true;
	}

};

class FinalGoal_Evalutor
{
	friend class Goal_Think;
protected :
	float				m_Bias;								//�S�[���ɑ΂���o�C�A�X�l
	float				m_message_Bias;				//��������̃��b�Z�[�W�ɑ΂���Bias
	float				m_message_BiasVal;			
	ENTITY_ID	m_target_ID;						//�S�[����I�����ꂽ�Ƃ��ɒ��₳�ꂽ�^�[�Q�b�g
	Goal*			m_evalute_Goal;					//�]������S�[��
	Lock				m_lock;
	
public :
	FinalGoal_Evalutor(const float bias,Goal* const evalute_Goaltag,const float message_BiasVal) : m_Bias(bias), m_target_ID(game_id::id_error),
		m_evalute_Goal(evalute_Goaltag), m_lock(10), m_message_Bias(1.0f), m_message_BiasVal(message_BiasVal)
	{	}
	virtual float	Calculate_Score(NPC_Brain* brain, Character* character) = 0;
	char*	Get_EvaluteGoalName(){ return m_evalute_Goal->goal_Name; }
	inline void	Reset_Target(){ m_target_ID = game_id::id_error; }
	inline const ENTITY_ID&	Get_TargetID()const{ return m_target_ID; }
	inline 	Goal*	Get_EvaluteGoal(){ return m_evalute_Goal; }
	inline void Clear_MessageBias(){ m_message_Bias = 1.0f; }
	inline void	 Set_MessageBias(){ m_message_Bias += m_message_BiasVal; }
};

class GoalEvalutor_AttackEnemy : public FinalGoal_Evalutor
{
	friend class NPC_Brain;
public :
	GoalEvalutor_AttackEnemy(const float bias, Goal* const evalute_Goal) : FinalGoal_Evalutor(bias, evalute_Goal,.0f)
	{	}	
	float	Calculate_Score(NPC_Brain* brain, Character* character);
};

class GoalEvalutor_GetAdvertisementPoint : public FinalGoal_Evalutor
{
	friend class NPC_Brain;
public:
	GoalEvalutor_GetAdvertisementPoint(const float bias, Goal* const evalute_Goal) : FinalGoal_Evalutor(bias, evalute_Goal,.0f)
	{	}
	float	Calculate_Score(NPC_Brain* brain, Character* character);
};

class GoalEvalutor_MedicalFriend : public FinalGoal_Evalutor
{
	friend class NPC_Brain;
public:
	GoalEvalutor_MedicalFriend(const float bias, Goal* const evalute_Goal, const float message_BiasVal) : FinalGoal_Evalutor(bias, evalute_Goal, message_BiasVal)
	{	}
	float	Calculate_Score(NPC_Brain* brain, Character* character);
};

class GoalEvalutor_CorrespondenceDenger : public FinalGoal_Evalutor
{
	friend class NPC_Brain;
public:
	GoalEvalutor_CorrespondenceDenger(const float& bias, Goal* const evalute_Goal) : FinalGoal_Evalutor(bias, evalute_Goal,.0f)
	{	}
	float	Calculate_Score(NPC_Brain* brain, Character* character);
};

class GoalEvalutor_Wander_Field : public FinalGoal_Evalutor
{
	friend class NPC_Brain;
public:
	GoalEvalutor_Wander_Field(const float bias, Goal* const evalute_Goal) : FinalGoal_Evalutor(bias, evalute_Goal,.0f)
	{	}
	float	Calculate_Score(NPC_Brain* brain, Character* character);
};

class GoalEvalutor_FollowFriend : public FinalGoal_Evalutor
{
	friend class NPC_Brain;
public:
	GoalEvalutor_FollowFriend(const float bias, Goal* const evalute_Goal) :
		FinalGoal_Evalutor(bias, evalute_Goal, .0f)
	{	}
	float	Calculate_Score(NPC_Brain* brain, Character* character);
};

#pragma endregion

#pragma region GoalCost_Calculater

namespace goalcost_calculater
{
	float	NearAttack(Goal* connection_source, NPC_Brain* brain,Character* character, const float& default_Cost);
	float GrenadeAttack(Goal* connection_source, NPC_Brain* brain, Character* character, const float& default_Cost);

	inline float	Test(Goal* connection_source, NPC_Brain* brain, Character* character, const float& cost)
	{
		return cost;
	}
	
}

#pragma endregion

class GoalEvalutor_Scouting : public FinalGoal_Evalutor
{
	friend class NPC_Brain;
private :
	float Get_Rate(NPC_Brain* brain,const SOUND_TYPE& type);
public:
	GoalEvalutor_Scouting(const float bias, Goal* const evalute_Goal) : FinalGoal_Evalutor(bias, evalute_Goal,.0f)
	{	}
	float	Calculate_Score(NPC_Brain* brain, Character* character);
};

#endif
