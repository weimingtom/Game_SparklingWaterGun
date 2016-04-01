#ifndef __ACCESOR_NPC_BRAIN
#define __ACCESOR_NPC_BRAIN

class NPC_Brain;
class NPCParam;

class Accesor_Brain
{
private :
	friend class NPC_Brain;
	
public :	//ゲッター
	static NPC_Evalutor*					Evalutor(NPC_Brain* access_Brain);
	static ENTITY_ID							Current_Target(NPC_Brain* access_Brain);
	static const NPCParam&				Param(NPC_Brain* access_Brain);
	static const DeFuzzify_Param&	 FuzzyParam(NPC_Brain* access_Brain);
	static const Symbol&					Symbol(NPC_Brain* access_Brain);
	static FinalGoal_Evalutor*			Final_GoalEvalutor(NPC_Brain* access_Brain);
	static const RAP_CENSROR*		Censor(NPC_Brain* access_Brain);
	static const StatusParam&			StatusParam(NPC_Brain* access_Brain);
	static NPC_FuzzyTool*					FuzzyTool(NPC_Brain* access_Brain);
	static Behavior_Manager*			BehaviorManager(NPC_Brain* access_Brain);
	static WorkingMemory*				WorkingMemory(NPC_Brain* access_Brain);
	static PathPlanner*						PathPlanner(NPC_Brain* access_Brain);
	static ParticularGoal_Manager*	ParticularGoal_Manager(NPC_Brain* access_Brain);
	static NPCFriendMessage*			FriendMessage(NPC_Brain* access_Brain);
public :	//セッター
	static void		Set_Current_Target(NPC_Brain* access_Brain, const ENTITY_ID& set_ID);
	static void		Target_Clear(NPC_Brain* access_Brain);
};

#endif
