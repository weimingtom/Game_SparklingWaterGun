#include	"..\All.h"

Goal_KnockDownEnemy::Goal_KnockDownEnemy(NPC_Brain* brain)
: Goal(brain, "KnockDownEnemy", goalcost_calculater::Test, 
	goal_tag::knockdown_enemy, nullptr)
{

}

Goal_KnockDownEnemy::~Goal_KnockDownEnemy()
{

}

bool	Goal_KnockDownEnemy::Active(Character* character)
{
	return true;
}

goal_status	Goal_KnockDownEnemy::Process(Character* character)
{
	return goal_status::completed;
}

void	Goal_KnockDownEnemy::Completed(Character* character)
{

}

void	Goal_KnockDownEnemy::Failed(Character* character)
{

}
