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
	bool				m_active;	//CurrentBehavior��MoveEdge�̂Ƃ��̂�true�ƂȂ�t���O
	bool				m_request;	//�o�H�T����\�������ǂ���
	int				m_search_Interval;	//�Č������s��Ȃ��ꍇ��-1
	int				m_progress_Frame;
	Vector3		m_current_GoalPos;
	goal_tag		m_parent_goal;	//�o�H�T�����s���Ă���S�[��
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
