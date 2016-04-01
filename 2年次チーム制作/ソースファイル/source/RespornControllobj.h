#ifndef __RESPORN_CONTROLL_OBJ_H__
#define __RESPORN_CONTROLL_OBJ_H__

#include "ControllObject.h"
#include "CODBOForwardDecl.h"

//***************************************************************
//	リスポーン用コントロールオブジェクト
//***************************************************************

class RespornControllobj :public ControllObject
{
private:
	enum RESPORN_STATE
	{
		_DIE_CAMERA,
		_RESPORN_WAIT
	};
	RESPORN_STATE			m_State;
	iex2DObj				m_DamageTexture;
	LPCHARACTER				m_pNextCharacter;
	int						m_Count;
	int						m_CameraFreezeCount;
	bool					m_isCameraMove;
	ENTITY_ID				m_zoom_charater;
	ENTITY_ID				m_NowSelectVending;
	Vector3					m_first_pos;
	RATIO					m_t;
	RATIO					m_Alpha;
public:
	RespornControllobj(
		LPCAMERA				pPreCamera,
		ENTITY_ID				myId,
		LPCHARACTER				pNextCharacter,
		int						team_type,
		TeamData*				tData,
		ENTITY_ID				zoom_charater
		);
	~RespornControllobj();
private:
	bool ChangeResporn();
	void DieCameraUpdate(TeamData& teamdata);
	bool RespornSelect(TeamData& teamdata);
	void RespornVending(TeamData& teamdata,LPMapObjectInterface ptr);
	void RespornParachute(TeamData& teamdata);
	bool CameraMove(TeamData& teamdata);
	void Resporn(TeamData& teamdata);
	void SetKillMeEnemyView(LPCONTROLLOBJCT pObj, TeamData& teamdata);
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