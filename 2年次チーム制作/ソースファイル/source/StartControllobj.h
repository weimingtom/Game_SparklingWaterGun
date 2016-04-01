#ifndef __START_CONTROLL_OBJ_H__
#define __START_CONTROLL_OBJ_H__

#include "ControllObject.h"
#include "CODBOForwardDecl.h"

//***************************************************************
//	スタート用コントロールオブジェクト
//***************************************************************

class StartControllobj :public ControllObject
{
private:
	LPCHARACTER				m_pNextCharacter;
	int						m_Count;
	bool					m_isFinish;
	Vector3					m_StartPos;
	RADIAN					m_StartAngle;
public:
	StartControllobj(
		ENTITY_ID				myId,
		LPCHARACTER				pNextCharacter,
		int						team_type,
		TeamData*				tData
		);
	~StartControllobj();
private:
	bool GetRespornPoint(LPMapObjectInterface* ppOut);
	void Resporn(TeamData& teamdata);
public:
	Vector3 GetBodyCenter(){ return m_base_param.pos; }
	void Controll();
	void Update(TeamData& teamdata);
	void Render(ControllObject& viewowner, TeamData& teamdata){}
	void Render_ShadowMap(ControllObject& viewowner, TeamData& teamdata){}
	void RenderUI(ControllObject& viewowner, TeamData& teamdata);
	bool HandleMessage(LPGameMessage msg);
};

#endif