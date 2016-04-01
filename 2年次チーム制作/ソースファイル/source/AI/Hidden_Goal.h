#ifndef __GoalHidden_H__
#define __GoalHidden_H__

class Goal_Hidden : public Goal
{
private :
	ENTITY_ID	m_escape_Target;			//逃げる対象
	Vector3		m_current_EscapePos;	//逃げる座標
private :
	bool	Evalute_Current_GoalPos();
	
	//goal_status	Directon_Behavior(Character* character);

public:
	Goal_Hidden(NPC_Brain* brain, GoalCost_Calculater evo, Behavior_DirectorBase* use_director);
	~Goal_Hidden()
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

	inline ENTITY_ID	Get_EscapeTarget(){ return m_escape_Target; }
	//とりあえず
	void	Set_EscapePos(const Vector3& pos){ m_current_EscapePos = pos; }

};

class Behavior_Director_Hidden : public Behavior_DirectorBase
{
private:
	goal_status Activate(NPC_Brain*brain, Character* character);
	/**
	*@brief	character_recordまたはthreat_Outsightから逃げるベクトルを決定する
	*@return 成功で引数にベクトル　失敗でfalse
	*/
	bool		Direction_Escape_Vec(NPC_Brain* brain,Vector3* out
													,const ENTITY_ID& escape_Target);
	Field_Node*	Direction_Esacpe_Point(NPC_Brain* brain, Character* character
													, const ENTITY_ID& escape_Target);
public:
	Behavior_Director_Hidden() 
	{}
	goal_status	Direction(NPC_Brain* brain, Character* character,bool	init = false);
};


#endif
