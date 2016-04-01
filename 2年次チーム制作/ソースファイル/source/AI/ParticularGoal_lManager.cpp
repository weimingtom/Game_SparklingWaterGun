#include	"..\All.h"

ParticularGoal_Manager::ParticularGoal_Manager( NPC_Brain* holder) : 
m_holder(holder), m_target_ID(ENTITY_ID::id_error)
{
	Initialize_Goal();
}

ParticularGoal_Manager::~ParticularGoal_Manager()
{
	for (auto it : m_particularGoal_vector)
		SAFE_DELETE(it);
	
	m_particularGoal_vector.clear();
}

void	ParticularGoal_Manager::Initialize_Goal()
{
	m_particularGoal_vector.reserve((int)partculargoal_type::num_partculargoal);

	m_particularGoal_vector.push_back(new Goal_AvoidanceVessel(m_holder, new Behavior_Director_AvoidanceVessel()));
	m_particularGoal_vector.push_back(new Goal_Airborne(m_holder, new Behavior_Director_Goal_Airborne));
}

bool	ParticularGoal_Manager::Insert_Goal(Character* character, const partculargoal_type& insert_goal)
{
	if (m_holder->Insert_Goal(character, m_particularGoal_vector[(int)(insert_goal)]))
	{
		Active();
		return true;
	}
	return false;
}
