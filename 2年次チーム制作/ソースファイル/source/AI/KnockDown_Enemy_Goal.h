#ifndef __KnockDownEnemy_H__
#define __KnockDownEnemy_H__

/**
*@note 近接攻撃,武器発射ゴールへと派生させるためのからゴールとして機能させる
*/
class Goal_KnockDownEnemy : public Goal
{
private :
public :
	Goal_KnockDownEnemy(NPC_Brain* holder);
	~Goal_KnockDownEnemy();
public:
	planningkey		Effect(const Symbol& symbol);
	planningkey		Prerequisite(const Symbol& symbol);

	bool					Active(Character* character);
	goal_status		Process(Character* character);
	void					Completed(Character* character);
	void					Failed(Character* character);
	
};


#endif
