#ifndef __PATH_PLANNER_H__
#define __PATH_PLANNER_H__

class NPC_Brain;
class Character;
struct PathSearchObject;

class PathPlanner
{
	static const int	m_not_Research = -1;
	static const int	m_search_errorFrame = 20;
private :
	bool				m_active;	//CurrentBehaviorがMoveEdgeのときのみtrueとなるフラグ
	bool				m_request;	//経路探索を申請中かどうか
	int				m_search_Interval;	//再検索を行わない場合は-1
	int				m_progress_Frame;
	Vector3		m_current_GoalPos;
	goal_tag		m_parent_goal;	//経路探索を行っているゴール
	NPC_Brain* m_holder;

private:
	bool	Evalute_Current_GoalPos(Character* character);

public :
	PathPlanner(NPC_Brain*	holder);
	~PathPlanner();

public: 
	bool	Activate(Character* character,const goal_tag& goal, const Vector3& goal_pos, int search_interval = -1);
	void	Exit();
	bool	Search(PathSearchObject*	search_Object);
	bool	Request_PathSearch(Character* character);
	void	Update(Character* character);

	bool	HandleMessage(AI_MESSAGE*	msg);

};

#endif
