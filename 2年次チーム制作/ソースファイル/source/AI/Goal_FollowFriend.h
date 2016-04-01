#ifndef __FOLLOW_FRIEND__
#define __FOLLOW_FRIEND__

class Goal_FollowFriend : public Goal
{
	static const int		base_FollowTime = 1000;
	static const int		adjust_Frame = 120;
private :
	int		m_follow_Time;	//–¡•û‚É•t‚¢‚Ä‚¢‚­ŽžŠÔ(Frame)

public:
	Goal_FollowFriend(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director);
	~Goal_FollowFriend()
	{
		SAFE_DELETE(m_behavior_Director);
	}
public:
	planningkey			Effect(const Symbol& symbol);
	planningkey			Prerequisite(const Symbol& symbol);

	bool						Active(Character* character);
	goal_status			Process(Character* character);
	void						Completed(Character* character);
	void						Failed(Character* character);
	bool						HandleMessage(AI_MESSAGE* msg);
};

class Behavior_Director_FollowFriend : public Behavior_DirectorBase
{
private:
	goal_status Activate(NPC_Brain*brain, Character* character);
	
public:
	Behavior_Director_FollowFriend()
	{}
	goal_status	Direction(NPC_Brain* brain, Character* character, bool	init = false);

};

#endif
