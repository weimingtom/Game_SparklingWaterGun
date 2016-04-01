#ifndef __NPC_FACTORY_H__
#define __NPC_FACTORY_H__

class NPC_Brain;

enum class npc_strength
{
	weak,		//Žã‚¢
	normal,	//‚Ó‚Â‚¤
	strong,		//‹­‚¢
};

class NPC_Factory
{
private :
	static Action_Planner*	Create_WeakActionPlanner(NPC_Brain* ret_Brain);
	static Action_Planner*	Create_NormalActionPlanner(NPC_Brain* ret_Brain);
	static Action_Planner*	Create_StrongActionPlanner(NPC_Brain* ret_Brain);

	static Goal_Think*	Create_WeakGoalThink(NPC_Brain* ret_Brain);
	static Goal_Think*	Create_NormalGoalThink(NPC_Brain* ret_Brain);
	static Goal_Think*	Create_StrongGoalThink(NPC_Brain* ret_Brain);

private :
	static bool WeakBrain(NPC_Brain* ret_Brain);
	static bool NormalBrain(NPC_Brain* ret_Brain);
	static bool StrongBrain(NPC_Brain* ret_Brain);

public :
	static NPC_Brain* Create(char* npc_name, const npc_strength& strength,
		const int npc_num,const int team_type,const ENTITY_ID default_ID);
	
};


#endif
