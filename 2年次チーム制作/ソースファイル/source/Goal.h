#ifndef __GOAL_H__
#define __GOAL_H__
#include	"NPC_Enum.h"

class NPC_Brain;
class Character;
class Symbol;
class CharacterMoveStateBase;
class AI_BehaviorBase;

//ゴールコスト計算用関数ポインタ

typedef float(*Goal_Evaluator)(NPC_Brain*, float cost);

class Goal
{
	friend class Action_Planner;
	friend class NPC_Brain;	//behavior観るよう
protected:
	goal_status	status;													//ゴールの状態
	Goal_Evaluator	evalutor;											//ゴールコスト評価関数
	goal_tag			tag;														//ゴールの識別番号

protected:
	AI_BehaviorBase**					behavior_List;			//Processの振る舞い（ステート列)
	const int										num_Behavior;			//振る舞い数
	int												current_Behavior;	//現在の振る舞い番号
	int												timer;							//ゴールの制限時間
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
	char*											goal_Name;	//ゴールの名前
	//**********************************************************
	//	プランニング用関数
	//**********************************************************
	virtual PLANNINGKEY		Effect(const Symbol& symbol) = 0;
	virtual PLANNINGKEY		Prerequisite(const Symbol& symbol) = 0;
public:
	//**********************************************************
	//	エフェクター
	//**********************************************************
	virtual void						Active(NPC_Brain* brain, Character* character) = 0;
	virtual goal_status		Process(NPC_Brain* brain, Character* character) = 0;
	virtual void						Completed(NPC_Brain*brain, Character* character) = 0;
	virtual void						Failed(NPC_Brain*brain, Character* character) = 0;
public :
	virtual	bool						Path_Search(NPC_Brain* brain, Character* character,const bool& research, Vector3& goal_Pos) = 0;
};

#endif
