#include	"..\All.h"

Behavior_Manager::Behavior_Manager(NPC_Brain* holder) :
m_current_Behavior(nullptr), m_current_tag(behavior_tag::error),
m_holder(holder)
{
	InitializeContainer();
}

void	Behavior_Manager::InitializeContainer()
{
	m_behavior_Container.reserve(behavior_tag::num_behavior);

	m_behavior_Container.push_back(new Move_Edge());
	m_behavior_Container.push_back(new Move_Point());
	m_behavior_Container.push_back(new Anime_ShotWeapon_Stand(new NPC_Aim_Normal("DATA\\NPC\\AimFuzzy.txt")));
	m_behavior_Container.push_back(new Anime_ShotWeapon_Circle(new NPC_Aim_Normal("DATA\\NPC\\AimFuzzy.txt")));
	m_behavior_Container.push_back(new Anime_RollingAttack());
	m_behavior_Container.push_back(new Anime_ChargeWeapon());
	m_behavior_Container.push_back(new Anime_LookBack());
	m_behavior_Container.push_back(new Anime_ThrowGrenade());
	m_behavior_Container.push_back(new Anime_AirBorne());
}

Behavior_Manager::~Behavior_Manager()
{
	for (auto it : m_behavior_Container)
	{
		SAFE_DELETE(it);
	}
	m_behavior_Container.clear();
}

bool	Behavior_Manager::Change_Behavior(Character* character, const behavior_tag& next_behavior,
																	Goal* parent,B_ProcessFunc use_process_Function)
{
	if (next_behavior >= behavior_tag::num_behavior ||
		next_behavior <= behavior_tag::error)
		MyAssert(false, "想定外のBehaviorにChangeしようとしてる");
	
	if (m_current_Behavior)
		m_current_Behavior->Exit(m_holder, character);
	
	m_current_tag = next_behavior;
	m_current_Behavior = m_behavior_Container[m_current_tag];
	m_current_Behavior->Enter(m_holder, character);
	m_current_Behavior->m_parent = parent;
	m_current_Behavior->m_process_Function = use_process_Function;
	return true;
}

const goal_status Behavior_Manager::Get_BehaviorStatus()
{
	return m_current_Behavior->Get_Status();
}

goal_status	Behavior_Manager::Execute_Current_Behavior(Character* character)
{
	if (m_current_Behavior)
		return m_current_Behavior->Execute(m_holder, character);

	if (m_current_tag >= behavior_tag::num_behavior ||
		m_current_tag <= behavior_tag::error)
		MyAssert(false, "想定外のBehaviorをExecuteしようとしてる");
	MyAssert(m_current_Behavior, "bahaviorがないよ");
	return goal_status::error;
}

bool	Behavior_Manager::Behavior_IsActive()
{
	if (m_current_tag < behavior_tag::num_behavior && m_current_tag > behavior_tag::error)
		return true;
	return false;
}

void	Behavior_Manager::Exit_Current_Behavior(Character* character)
{
	if (m_current_Behavior)
		m_current_Behavior->Exit(m_holder, character);
}

