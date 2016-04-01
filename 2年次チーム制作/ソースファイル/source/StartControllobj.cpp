#include "StartControllobj.h"
#include "CharacterUtilityFunction.h"
#include "ControllObjectFunction.h"
#include "ControllObjectManager.h"
#include "ControllerManager.h"
#include "GameSystem\GameIDManager.h"
#include "map\MapObject.h"
#include "CameraMove.h"
#include <typeinfo>
#include "ScoreCalcManager.h"
#include "GameMatchInfomation.h"
#include "TimeRender.h"

StartControllobj::StartControllobj(
	ENTITY_ID				myId,
	LPCHARACTER				pNextCharacter,
	int						team_type,
	TeamData*				tData) :
	ControllObject(myId, team_type, tData),
	m_pNextCharacter(pNextCharacter),
	m_Count(0),
	m_StartAngle(0),
	m_StartPos(0,0,0)
{
	m_isFinish = false;
	m_camera.SetNewState(new StartCamera("DATA\\ScriptFile\\StartCamera.txt", &m_isFinish));
	m_base_param.pos = Vector3(0, 0, 0);
	m_camera.Update();
}


StartControllobj::~StartControllobj()
{
	if (m_pNextCharacter)
	{
		delete m_pNextCharacter;
	}
}

void StartControllobj::Controll()
{

}

bool StartControllobj::GetRespornPoint(LPMapObjectInterface* ppOut)
{
	class Predicate
	{
		TEAM_TYPE type;
	public:
		Predicate(TEAM_TYPE type) :type(type){}
		bool operator()(LPCONTROLLOBJCT obj)
		{
			return ControllObjFunc::GetTeamType(obj) == type&&
				typeid(StartControllobj) == typeid(*obj);
		}
	};
	MapObjectManager::MAPOBJ_ARRAY obj;
	
	MAP_MANAGER.Get_TypeObject(
		obj,
		static_cast<MAP_OBJ_TYPE>(map_obj::_resporn_point_begin + m_pMyMatchData->team_type));

	int size = (int)obj.size();
	if (size <= 0)return false;

	CONTROLLOBJLIST obj_list = CONTROLLOBJMGR.GetObjectList(Predicate(m_pMyMatchData->team_type));
	int num = 0;

	for (auto it = obj_list.begin();
		it != obj_list.end();
		++it, ++num)
	{
		if ((*it)->GetId() == GetId())break;
	}

	if (num >= size)return false;

	*ppOut = obj[num];

	return true;
}

void StartControllobj::Resporn(TeamData& teamdata)
{

	CHARACTER_STATE_TYPE type = chr_move_state::apeer_vending_machine;
	GameMessage msg;

	ChrFunc::ApperVendingMachine(m_pNextCharacter, m_StartPos, m_StartAngle);

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

void StartControllobj::Update(TeamData& teamdata)
{
	m_camera.Update();

	if (m_isFinish || KEY(KEY_ENTER, 0) == 3)
	{
		((ScoreCalcManager*)IDMANAGER.GetPointer(game_id::id_score_calc_manager))->StartCountStart();

		LPMapObjectInterface pMapobj;

		MyAssert(GetRespornPoint(&pMapobj),
			"リスポーンポイントが見つかりませんでした。");

		const OBJECTPARAM& param = pMapobj->Get_Param();

		m_StartPos = param.pos;
		m_StartAngle = param.angle.y;

		m_camera.SetNewState(new FreezeCamera());

		m_camera.SetPos(
			param.pos + Vector3(sinf(m_StartAngle + PI_d4 / 2), 0, cosf(m_StartAngle + PI_d4 / 2)) * 6 + Vector3(0, 3, 0));
		m_camera.SetTarget(
			param.pos + Vector3(0, 2, 0));
	}

	if (GAMEMATCHINFO.isGameStart())
	{
		Resporn(teamdata);
	}
}

void StartControllobj::RenderUI(ControllObject& viewowner, TeamData& teamdata)
{
	const RECTI& rc = GAMEMATCHINFO.GetScreenSize();

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


bool StartControllobj::HandleMessage(LPGameMessage msg)
{
	return false;
}