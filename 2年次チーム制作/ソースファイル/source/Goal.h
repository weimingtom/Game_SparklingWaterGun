#ifndef __GOAL_H__
#define __GOAL_H__
#include	"NPC_Enum.h"

class NPC_Brain;
class Character;
class Symbol;
class CharacterMoveStateBase;
class AI_BehaviorBase;

//�S�[���R�X�g�v�Z�p�֐��|�C���^

typedef float(*Goal_Evaluator)(NPC_Brain*, float cost);

class Goal
{
	friend class Action_Planner;
	friend class NPC_Brain;	//behavior�ς�悤
protected:
	goal_status	status;													//�S�[���̏��
	Goal_Evaluator	evalutor;											//�S�[���R�X�g�]���֐�
	goal_tag			tag;														//�S�[���̎��ʔԍ�

protected:
	AI_BehaviorBase**					behavior_List;			//Process�̐U�镑���i�X�e�[�g��)
	const int										num_Behavior;			//�U�镑����
	int												current_Behavior;	//���݂̐U�镑���ԍ�
	int												timer;							//�S�[���̐�������
protected:
	goal_status							Behavior(NPC_Brain* brain, Character* character);
	bool											Put_CurrentBehavior(NPC_Brain* brain, Character* character);
	bool											Path_Research(NPC_Brain* brain, Character* character,  Vector3 & goal_Pos);
public:
	Goal(char* goalname, Goal_Evaluator evo, const goal_tag& tag, const int& num_Behavior) :
		goal_Name(goalname), status(goal_status::inactive), evalutor(evo),
		num_Behavior(num_Behavior), timer(0), tag(tag){}
	virtual ~Goal(){}

public:
	char*											goal_Name;	//�S�[���̖��O
	//**********************************************************
	//	�v�����j���O�p�֐�
	//**********************************************************
	virtual PLANNINGKEY		Effect(const Symbol& symbol) = 0;
	virtual PLANNINGKEY		Prerequisite(const Symbol& symbol) = 0;
public:
	//**********************************************************
	//	�G�t�F�N�^�[
	//**********************************************************
	virtual void						Active(NPC_Brain* brain, Character* character) = 0;
	virtual goal_status		Process(NPC_Brain* brain, Character* character) = 0;
	virtual void						Completed(NPC_Brain*brain, Character* character) = 0;
	virtual void						Failed(NPC_Brain*brain, Character* character) = 0;
public :
	virtual	bool						Path_Search(NPC_Brain* brain, Character* character,const bool& research, Vector3& goal_Pos) = 0;
};

#endif
