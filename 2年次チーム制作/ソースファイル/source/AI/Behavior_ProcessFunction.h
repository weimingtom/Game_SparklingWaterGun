#ifndef __BEHAVIOR_PROCESSFUNCTION_H__
#define __BEHAVIOR_PROCESSFUNCTION_H__

class NPC_Brain;
class Character;

namespace behavior_processfunc
{
	namespace move_edge
	{
		goal_status Arrival_GoalPoint(NPC_Brain* brain, Character* character);
		goal_status	Range_of_Target(NPC_Brain* brain, Character* character);
		goal_status	Follow_Target(NPC_Brain* brain, Character* character);
	};

	namespace move_point
	{
		goal_status Arrival_GoalPoint(NPC_Brain* brain, Character* character);
		goal_status	Range_of_Target(NPC_Brain* brain, Character* character);
		goal_status	Follow_Target(NPC_Brain* brain, Character* character);
	};

	namespace shotweapon
	{
		goal_status AttackCharacter(NPC_Brain* brain, Character* character);
		goal_status Start_Up_Object(NPC_Brain* brain, Character* character);
		goal_status	Medical_Character(NPC_Brain* brain, Character* character);
	};
	goal_status	Throw_Grenede(NPC_Brain* brain, Character* character);
	
	goal_status	RollingAttack(NPC_Brain* brain, Character* character);
	
};

/**
*@brief Behaviorのステータスチェック関数を格納する関数ポインタ
*@insert namespace behavior_processfunc
*/
typedef goal_status (*B_ProcessFunc)(NPC_Brain*, Character*);

#endif
