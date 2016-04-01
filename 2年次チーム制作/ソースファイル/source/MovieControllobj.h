#ifndef __MOVIE_CONTROLL_OBJ_H__
#define __MOVIE_CONTROLL_OBJ_H__

#include "ControllObject.h"
#include "CODBOForwardDecl.h"

//***************************************************************
//	ムービー用コントロールオブジェクト
//***************************************************************


class MovieControllobj :public ControllObject
{
public:
	MovieControllobj(
		ENTITY_ID				myId,
		int						team_type,
		TeamData*				tData
		);
	~MovieControllobj();
private:

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