#ifndef __KnockDownEnemy_H__
#define __KnockDownEnemy_H__

/**
*@note ‹ßÚUŒ‚,•Ší”­ËƒS[ƒ‹‚Ö‚Æ”h¶‚³‚¹‚é‚½‚ß‚Ì‚©‚çƒS[ƒ‹‚Æ‚µ‚Ä‹@”\‚³‚¹‚é
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
