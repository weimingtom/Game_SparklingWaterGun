#include "RespornControllobj.h"
#include "CharacterUtilityFunction.h"
#include "ControllObjectFunction.h"
#include "ControllObjectManager.h"
#include "ControllerManager.h"
#include "GameSystem\GameIDManager.h"
#include "CharacterMoveState.h"
#include "map\MapObject.h"
#include "GameMatchInfomation.h"
#include "TimeRender.h"

RespornControllobj::RespornControllobj(
	LPCAMERA				pPreCamera,
	ENTITY_ID				myId,
	LPCHARACTER				pNextCharacter,
	int						team_type,
	TeamData*				tData,
	ENTITY_ID				zoom_charater) :
	ControllObject(myId, team_type, tData),
m_pNextCharacter(pNextCharacter),
m_State(_DIE_CAMERA),
m_Count(0),
m_zoom_charater(zoom_charater),
m_t(0),
m_Alpha(1),
m_NowSelectVending(game_id::id_error),
m_DamageTexture("DATA\\TEXTURE\\UI\\Gradation.png"),
m_isCameraMove(true),
m_CameraFreezeCount(0)
{
	ChrFunc::SetAutoAimPointEnable(m_pNextCharacter, false);
	m_base_param.pos = Vector3(0,0,0);

	m_camera.SetPos(pPreCamera->GetPos());
	m_camera.SetTarget(pPreCamera->GetTarget());
	m_first_pos = pPreCamera->GetTarget();
}


RespornControllobj::~RespornControllobj()
{
	if (m_pNextCharacter)
	{
		delete m_pNextCharacter;
	}
}

void RespornControllobj::Controll()
{
	if (controller::GetTRG(controller::button::maru, m_pMyMatchData->button_set.controller_num) &&
		m_State == _RESPORN_WAIT&&
		m_Count < 300)
	{
		m_Count = 300;
	}

	return;

	if (controller::GetTRG(controller::button::batu, m_pMyMatchData->button_set.controller_num) &&
		m_State == _RESPORN_WAIT&&
		m_Count < 300)
	{
		ChangeResporn();
	}
}

bool RespornControllobj::CameraMove(TeamData& teamdata)
{
	LPCONTROLLOBJCT pObj;

	if (!IS_CONTROLLOBJ_ID(m_zoom_charater) ||
		!(pObj = (LPCONTROLLOBJCT)IDMANAGER.GetPointer(m_zoom_charater)))
	{
		return false;
	}

	m_camera.SetTarget(
		fLERP(m_first_pos, pObj->GetBodyCenter(),m_t)
		);

	SetKillMeEnemyView(pObj, teamdata);

	return true;
}

void RespornControllobj::Resporn(TeamData& teamdata)
{

#if PARACHUTE_RESPORN==TRUE

	RespornParachute(teamdata);

#else
	LPMapObjectInterface pMapObj;

	if(IS_MAPOBJ_ID(m_NowSelectVending) &&
		(pMapObj = (LPMapObjectInterface)IDMANAGER.GetPointer(m_NowSelectVending)) &&
		pMapObj->Get_Type() == (MAP_OBJ_TYPE)((int)map_obj::_resporn_point_begin + m_pMyMatchData->team_type)
		)
	{
		RespornVending(teamdata, pMapObj);
	}
	else
	{
		RespornParachute(teamdata);
	}
#endif
}

void RespornControllobj::SetKillMeEnemyView(
	LPCONTROLLOBJCT pObj,
	TeamData& teamdata)
{
	TeamViewData::DataParam param;

	param.alpha = 1;
	param.color = CODMgr::GetTeamColor(ControllObjFunc::GetMatchData(pObj)->team_type);
	param.id = m_zoom_charater;
	param.time = 40;

	teamdata.m_Viewdata.SetData(param);

}

bool RespornControllobj::ChangeResporn()
{
	MapObjectManager::MAPOBJ_ARRAY obj;
	LPMapObjectInterface pRand;
	LPMapObjectInterface pNow;

	MAP_MANAGER.Get_TypeObject(
		obj,
		static_cast<MAP_OBJ_TYPE>(map_obj::_resporn_point_begin + m_pMyMatchData->team_type));

	int size = (int)obj.size();
	if (size <= 1)return false;

	if (IS_MAPOBJ_ID(m_NowSelectVending) &&
		(pNow = (LPMapObjectInterface)IDMANAGER.GetPointer(m_NowSelectVending)) &&
		pNow->Get_Type() == (MAP_OBJ_TYPE)((int)map_obj::_resporn_point_begin + m_pMyMatchData->team_type)
		)
	{
	}
	else
	{
		pNow = nullptr;
	}

	do
	{
		pRand = obj.at(rand() % (int)size);
	} while (pRand == pNow);

	const OBJECTPARAM& param = pRand->Get_Param();
	const float obj_angle = param.angle.y + (rand() % 2 == 0) ? (-PI_d4) : (PI_d4);

	m_camera.SetNewState(
		new RespornSelectCamera(
		param.pos + Vector3(sinf(obj_angle), 0, cosf(obj_angle)) * 6 + Vector3(0, 3, 0),
		param.pos + Vector3(0, 2, 0), 
		m_camera.GetPos(),
		m_camera.GetTarget(),
		0.03f,
		m_isCameraMove
		)
		);

	m_NowSelectVending = pRand->GetId();
	return true;
}

void RespornControllobj::DieCameraUpdate(TeamData& teamdata)
{
	const int wait_time = 260;
	const int zoom_time = 40;
	bool zoom = true;

	if (m_Count<80)
	{
		m_camera.SetPos(m_camera.GetPos() + Vector3(0, 0.0005f, 0));
	}

	if (m_Count > zoom_time)
	{
		zoom = CameraMove(teamdata);

		m_t += 0.02f;
		if (m_t > 1)m_t = 1;
	}

	if (m_Count++ > wait_time ||
		(zoom == false && m_Count>120))
	{
		m_Count = 0;
		m_t = 0;
		m_State = _RESPORN_WAIT;
	}
}

bool RespornControllobj::RespornSelect(TeamData& teamdata)
{
	LPMapObjectInterface pMapObj;

	if (!IS_MAPOBJ_ID(m_NowSelectVending) ||
		(pMapObj = (LPMapObjectInterface)IDMANAGER.GetPointer(m_NowSelectVending)) == nullptr ||
		pMapObj->Get_Type() != (MAP_OBJ_TYPE)((int)map_obj::_resporn_point_begin + m_pMyMatchData->team_type)
		)
	{
		if (!ChrFunc::GetRandomRespornPoint(m_pMyMatchData->team_type, &pMapObj))
		{
			m_camera.SetPos(Vector3(30, 20, 0));
			m_camera.SetTarget(Vector3(0, 10, 10));

			m_NowSelectVending = game_id::id_error;

			return false;
		}

		const OBJECTPARAM& param = pMapObj->Get_Param();
		const float obj_angle = param.angle.y + (rand() % 2 == 0) ? (-PI_d4) : (PI_d4);

		m_camera.SetPos(
			param.pos +
			Vector3(sinf(obj_angle), 0, cosf(obj_angle)) * 6 +
			Vector3(0, 3, 0)
			);

		m_camera.SetTarget(param.pos + Vector3(0, 2, 0));
	}

	m_NowSelectVending = pMapObj->GetId();


	return true;
}


void RespornControllobj::RespornVending(TeamData& teamdata, LPMapObjectInterface ptr)
{
	Vector3 pos;
	float angle;

	CHARACTER_STATE_TYPE type = chr_move_state::apeer_vending_machine;
	GameMessage msg;

	const OBJECTPARAM& param = ptr->Get_Param();

	angle = param.angle.y;
	pos = param.pos;

	ChrFunc::ApperVendingMachine(m_pNextCharacter, pos, angle);
	ChrFunc::SetAutoAimPointEnable(m_pNextCharacter, true);

	((ControllerManager*)IDMANAGER.GetPointer(game_id::id_controller_manager))->SwapControllObject(
		this,
		m_pNextCharacter
		);

	msg.exinfo = &type;
	msg.from = game_id::id_error;
	msg.to = m_pNextCharacter->GetId();
	msg.type = msg_t::chr_table_state_change;

	m_pNextCharacter->HandleMessage(&msg);

	CONTROLLOBJMGR.AddObject(m_pNextCharacter);
	CONTROLLOBJMGR.DeleteObject(GetId());

	LPCAMERA cam = ControllObjFunc::GetCamera(m_pNextCharacter);
	cam->SetPos(m_camera.GetPos());
	cam->SetTarget(m_camera.GetTarget());

	cam->SetNewState(
		new UsualPlayerMoveCamera(
		cam,
		m_pNextCharacter,
		1.0f,
		0.025f,
		Vector3(0.32f, -0.5f, -5.3f),
		Vector3(0.32f, -0.5f, 0))
		);

	m_pNextCharacter->Update(teamdata);
	m_pNextCharacter = nullptr;
}

void RespornControllobj::RespornParachute(TeamData& teamdata)
{
	Vector3 pos;
	float angle;

	GameMessage msg;

	//ない場合は空からパラシュート
	CHARACTER_STATE_TYPE state = chr_move_state::parachute;

	msg.from = GetId();
	msg.to = m_pNextCharacter->GetId();
	msg.type = msg_t::chr_table_state_change;
	msg.exinfo = &state;

	m_pNextCharacter->HandleMessage(&msg);

	pos = Vector3((float)(rand() % 70 - 35), 20, (float)(rand() % 70 - 35));
	angle = 0;
	

	ChrFunc::ApperVendingMachine(m_pNextCharacter, pos, angle);
	ChrFunc::SetAutoAimPointEnable(m_pNextCharacter, true);

	((ControllerManager*)IDMANAGER.GetPointer(game_id::id_controller_manager))->SwapControllObject(
		this,
		m_pNextCharacter
		);

	CONTROLLOBJMGR.AddObject(m_pNextCharacter);
	CONTROLLOBJMGR.DeleteObject(GetId());

	m_pNextCharacter->Update(teamdata);
	m_pNextCharacter = nullptr;
}


void RespornControllobj::Update(TeamData& teamdata)
{
	if (m_CameraFreezeCount < INT_MAX)m_CameraFreezeCount++;

	if (m_isCameraMove)
	{
		m_CameraFreezeCount = 0;
	}

	m_Alpha -= 0.01f;
	m_Alpha = max(m_Alpha, 0);

	if (m_State == _DIE_CAMERA)
	{
		DieCameraUpdate(teamdata);
	}
	else
	{
		m_Count++;

		if (RespornSelect(teamdata) == false)
		{
			if (m_Count < 300)
			{
				m_Count = 300;
			}
		}
		if ((m_Count>40
			|| m_pMyMatchData->player_type == player_type::COMPUTER))
		{
			Resporn(teamdata);
		}
	}

	m_camera.Update();
}

void RespornControllobj::RenderUI(ControllObject& viewowner, TeamData& teamdata)
{
	const RECTI& rc = GAMEMATCHINFO.GetScreenSize();

	m_DamageTexture.Render(
		rc.x,
		rc.y,
		rc.width,
		rc.height,

		0,
		0,
		512,
		512,
		RS_COPY,
		D3DCOLOR_COLORVALUE(1, 0, 0, m_Alpha)
		);

	RECTI screen = GAMEMATCHINFO.GetScreenSize();
	GameMessage		msg;
	TimerRenderMsgInfo info;

	info.tex_rc.width = screen.width / 8;
	info.tex_rc.height = screen.height / 16;
	info.tex_rc.left = screen.width / 64;
	info.tex_rc.top = screen.height / 32;

	info.color = CODMgr::GetTeamColor(m_pMyMatchData->team_type);

	msg.from = GetId();
	msg.to = game_id::id_timer;
	msg.type = msg_t::timer_render_msg;
	msg.exinfo = &info;

	IDMANAGER.Dispatch(
		&msg
		);

	msg.from = GetId();
	msg.to = game_id::id_score_calc_manager;
	msg.type = msg_t::render_score_msg;
	msg.exinfo = &m_pMyMatchData->team_type;

	IDMANAGER.Dispatch(
		&msg
		);
}


bool RespornControllobj::HandleMessage(LPGameMessage msg)
{

	return false;
}