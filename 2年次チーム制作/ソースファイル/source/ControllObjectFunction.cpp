#include "ControllObjectFunction.h"
#include "LuaLoadParam.h"
#include "ScoreCalcManager.h"
#include "GameSystem\GameIDManager.h"
#include "GameSound\GameSound.h"
#include "CODPlaySound.h"

#define TOCONTROLL(ptr) (*(ControllObjFunc*)ptr)


void ControllObjFunc::InitControllObjectParam(LPCONTROLLOBJECTPARAM pOut)
{
	pOut->pos = Vector3(0, 0, 0);
	pOut->blur_time = 0;
	pOut->blur_power = 0;
	pOut->doexplode_blur = false;
	pOut->explode_pos = Vector2(0, 0);
	pOut->do_water_drop = false;
}

CRVector3 ControllObjFunc::GetPos(LPCONTROLLOBJCT obj)
{
	return TOCONTROLL(obj).m_base_param.pos;
}


LPCAMERA ControllObjFunc::GetCamera(LPCONTROLLOBJCT obj)
{
	return &TOCONTROLL(obj).m_camera;
}

LPPLAYERMATCHDATA ControllObjFunc::GetMatchData(LPCONTROLLOBJCT obj)
{
	return TOCONTROLL(obj).m_pMyMatchData;
}

int ControllObjFunc::GetTeamType(LPCONTROLLOBJCT obj)
{
	return TOCONTROLL(obj).m_base_param.team_type;
}

TeamData* ControllObjFunc::GetTeamData(LPCONTROLLOBJCT obj)
{
	return TOCONTROLL(obj).m_pTeamData;
}

const ControllObjectParam& ControllObjFunc::GetParam(LPCONTROLLOBJCT obj)
{
	return TOCONTROLL(obj).m_base_param;
}

void ControllObjFunc::SetMatchDataPtr(LPCONTROLLOBJCT obj, LPPLAYERMATCHDATA data)
{
	MyAssert(data, "nullのマッチデータポインタがセットされようとしました");
	TOCONTROLL(obj).m_pMyMatchData = data;
}

void ControllObjFunc::SetBlur(
	LPCONTROLLOBJCT obj, float time, RATIO power, CRVector2 pos)
{
	TOCONTROLL(obj).m_base_param.doexplode_blur = true;
	TOCONTROLL(obj).m_base_param.explode_pos = pos;
	TOCONTROLL(obj).m_base_param.blur_time = time;
	TOCONTROLL(obj).m_base_param.blur_power = power;
}

void ControllObjFunc::ResetBlurParam(LPCONTROLLOBJCT obj)
{
	TOCONTROLL(obj).m_base_param.doexplode_blur = false;
}

void ControllObjFunc::SetWaterDropParam(LPCONTROLLOBJCT obj, bool flg)
{
	TOCONTROLL(obj).m_base_param.do_water_drop = flg;
}

void ControllObjFunc::AddKillScore(LPCONTROLLOBJCT addObj)
{
	GameMessage msg;
	AddScoreMsgData score;

	score.plData = GetMatchData(addObj);
	score.type = AddScoreMsgData::_ENEMY_KILL;

	msg.from = addObj->GetId();
	msg.to = game_id::id_score_calc_manager;
	msg.type = msg_t::add_score_msg;
	msg.exinfo = &score;

	IDMANAGER.Dispatch(&msg);
}

void ControllObjFunc::AddDeathScore(LPCONTROLLOBJCT addObj)
{
	GameMessage msg;
	AddScoreMsgData score;

	score.plData = GetMatchData(addObj);
	score.type = AddScoreMsgData::_DEATH;

	msg.from = addObj->GetId();
	msg.to = game_id::id_score_calc_manager;
	msg.type = msg_t::add_score_msg;
	msg.exinfo = &score;

	IDMANAGER.Dispatch(&msg);
}

void ControllObjFunc::AddChangeShareObjScore(LPCONTROLLOBJCT addObj)
{
	GameMessage msg;
	AddScoreMsgData score;

	score.plData = GetMatchData(addObj);
	score.type = AddScoreMsgData::_CHANGE_SHARE_OBJ;

	msg.from = addObj->GetId();
	msg.to = game_id::id_score_calc_manager;
	msg.type = msg_t::add_score_msg;
	msg.exinfo = &score;

	IDMANAGER.Dispatch(&msg);
}

void ControllObjFunc::PlayDamageHitSound(LPCONTROLLOBJCT to)
{
	if (TOCONTROLL(to).m_pMyMatchData->player_type != player_type::PLAYER)return;

	SOUNDPLAYER.PlaySE(
		sound_t::damage_hit,
		1
		);
}

void ControllObjFunc::SetShockVecCamera(LPCONTROLLOBJCT obj, CRVector2 shock_vec, RADIAN power)
{
	D3DXVECTOR3 axis(0, 0, 0);
	Vector2 vec = Vector2Normalize(shock_vec);
	DXQuaternion qua;

	axis.z = -vec.x;
	axis.x = vec.y;

	D3DXQuaternionRotationAxis(&qua, &axis, power);

	TOCONTROLL(obj).m_camera.SetShockAngle(qua);
}