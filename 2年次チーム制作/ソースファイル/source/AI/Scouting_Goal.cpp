#include	"..\All.h"

Goal_Scouting::Goal_Scouting(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director)
: Goal(brain, "Scouting", evo, goal_tag::error,use_director)
{
	//behavior_List[0] = new Anime_LookBack("Anime_LookBack", this);
	//behavior_List[1] = new Move_Edge(-1, true, "MOVE_EDGE", this,behavior_processfunc::move_edge::Arrival_GoalPoint);

}

bool	Goal_Scouting::Active(Character* character)
{
	return true;
}

goal_status	Goal_Scouting::Process( Character* character)
{
	goal_status status = Behavior(character);
	return status;
}

void	Goal_Scouting::Completed( Character* character)
{
	//End(character);
}

void	Goal_Scouting::Failed(Character* character)
{
	//End(character);
}

goal_status	Behavior_Director_Scouting::Direction(NPC_Brain* brain, Character* character, bool init)
{
	return goal_status::completed;
}