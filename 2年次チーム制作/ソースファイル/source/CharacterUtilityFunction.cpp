#include "All.h"

// atan2f(x,z)

#define TOCONTROLL(ptr) ((CharacterUtilityFunctions*)(ptr))



void CharacterUtilityFunctions::InitCharacterParam(
	LPCHRPARAM pOut)
{
	//全項目を初期化
	D3DXMatrixIdentity(&pOut->m_trans_mat);
	pOut->m_move.x = pOut->m_move.y = pOut->m_move.z = 0;
	pOut->m_view_vec.x = pOut->m_view_vec.y = 0;
	pOut->m_view_vec.z = 1;
	pOut->m_RideObjectId = game_id::id_error;
	pOut->m_parent_move.x = pOut->m_parent_move.y = pOut->m_parent_move.z = 0;
	pOut->m_LifePoint = GetConstChrParam(nullptr)->max_hp;
	pOut->m_Count = 0;
	pOut->m_last_damage_parent = game_id::id_error;
	pOut->m_last_damage_count = 0;
	pOut->UpValue.ChargeSpeed = 0;
	pOut->UpValue.MoveSpeed= 0;
	pOut->UpValue.PowerUpAlpha = 0;
}

void CharacterUtilityFunctions::CreateTransMatrix(
	Matrix& mat, CRVector3 cfront, CRVector3 pos)
{
	Vector3 right, up(0, 1, 0), front(cfront);
	Vector3Cross(right, up, front);
	Vector3Cross(up, front, right);

	
	D3DXMatrixIdentity(&mat);

	front.Normalize();
	right.Normalize();
	up.Normalize();

	mat._11 = right.x;
	mat._12 = right.y;
	mat._13 = right.z;

	mat._21 = up.x;
	mat._22 = up.y;
	mat._23 = up.z;

	mat._31 = front.x;
	mat._32 = front.y;
	mat._33 = front.z;

	mat._41 = pos.x;
	mat._42 = pos.y;
	mat._43 = pos.z;
}

CHARACTER_STATE_TYPE CharacterUtilityFunctions::GetStateType(LPCharacter C)
{
	return TOCONTROLL(C)->m_State_type;
}

const CHRPARAM&	CharacterUtilityFunctions::GetParam(LPCharacter C)
{
	return TOCONTROLL(C)->m_Param;
}


CRVector3 CharacterUtilityFunctions::GetFront(LPCharacter C)
{
	//前方向ベクトルを得る
	return TOCONTROLL(C)->m_Param.m_view_vec;
}

Vector3	CharacterUtilityFunctions::GetRight(LPCharacter C)
{
	//右方向ベクトルを得る
	Vector3 right;
	Vector3Cross(right, CHARACTER_UPVEC, TOCONTROLL(C)->m_Param.m_view_vec);
	return right;
}


float	CharacterUtilityFunctions::GetAngle(LPCharacter C)		
{
	//向いているY軸の角度を得る(正規化済み)
	CRVector3 v = TOCONTROLL(C)->m_Param.m_view_vec;
	return atan2f(v.x,v.z);
}

const LPCAMERA CharacterUtilityFunctions::GetCamera(LPCharacter C)
{
	return ControllObjFunc::GetCamera(C);
}

LPCHARACTERMODELBASE CharacterUtilityFunctions::GetModelPtr(LPCharacter C)		//キャラクタのモデルポインタを得る
{
	return TOCONTROLL(C)->m_pCharacterModel;
}

LPCHARACTERWEPONBASE CharacterUtilityFunctions::GetWepon(LPCharacter C)
{
	return TOCONTROLL(C)->m_pWepon;
}

LPCHARACTER_UI CharacterUtilityFunctions::GetUI(LPCharacter C)
{
	return TOCONTROLL(C)->m_pUserInterface;
}

RATIO CharacterUtilityFunctions::GetChargePower(LPCharacter C)
{
	return TOCONTROLL(C)->m_pUserInterface->GetColaGaugePower();
}

void CharacterUtilityFunctions::GetBonePos(LPCHARACTER C, CHARACTER_BONE_TYPE bone_type, Vector3& out)
{
	LPCHARACTERMODELBASE pModel = ChrFunc::GetModelPtr(C);
	const Matrix& m = pModel->GetBoneMatrix(bone_type);

	out.x = m._41;
	out.y = m._42;
	out.z = m._43;

	out = Vector3MulMatrix(out, pModel->GetMatrix());
}

void CharacterUtilityFunctions::GetBoneLocalPos(LPCHARACTER C, CHARACTER_BONE_TYPE bone_type, Vector3& out)
{
	const Matrix& m = ChrFunc::GetModelPtr(C)->GetBoneMatrix(bone_type);

	out.x = m._41;
	out.y = m._42;
	out.z = m._43;
}

void CharacterUtilityFunctions::GetTransMatrix(LPCharacter C, Matrix& mat,CRVector3 scale)
{
	const CHRPARAM& p = ChrFunc::GetParam(C);

	CRVector3 pos = ControllObjFunc::GetPos(C);
	Vector3 front(p.m_view_vec), right(ChrFunc::GetRight(C)), up;
	front.y = 0;

	Vector3Cross(up, front, right);

	D3DXMatrixIdentity(&mat);

	front.Normalize();
	right.Normalize();
	up.Normalize();

	VECTOR3MULSCALAR(right, right, scale.x);
	VECTOR3MULSCALAR(up, up, scale.y);
	VECTOR3MULSCALAR(front, front, scale.z);

	mat._11 = right.x;
	mat._12 = right.y;
	mat._13 = right.z;

	mat._21 = up.x;
	mat._22 = up.y;
	mat._23 = up.z;

	mat._31 = front.x;
	mat._32 = front.y;
	mat._33 = front.z;

	mat._41 = pos.x;
	mat._42 = pos.y;
	mat._43 = pos.z;
}

//ダメージ関連
void CharacterUtilityFunctions::SetCureValue(LPCharacter C, float val)		//回復
{
	const float maxHP = GetConstChrParam(C)->max_hp;

	TOCONTROLL(C)->m_Param.m_LifePoint += val;

	if (TOCONTROLL(C)->m_Param.m_LifePoint > maxHP)
		TOCONTROLL(C)->m_Param.m_LifePoint = maxHP;
}

void CharacterUtilityFunctions::SetDamageValue(LPCharacter C, float val)	//ダメージ
{
	TOCONTROLL(C)->m_Param.m_LifePoint -= val;

	if (TOCONTROLL(C)->m_Param.m_LifePoint < 0)
		TOCONTROLL(C)->m_Param.m_LifePoint = 0;
}

bool CharacterUtilityFunctions::isDead(LPCharacter C)	//死んでいるかどうか
{
	return TOCONTROLL(C)->m_Param.m_LifePoint<=FLT_EPSILON;
}


bool CharacterUtilityFunctions::isDying(LPCharacter C)
{
	return TOCONTROLL(C)->m_Param.m_LifePoint <= GetConstChrParam(C)->dying_hp;
}

bool CharacterUtilityFunctions::HitCheck(LPCharacter C, CRVector3 hitball, float hitballsize)
{
	Vector3 tmp;
	float len, size = GetConstChrParam(C)->damage_hit_width_size + hitballsize;
	Vector3 center = C->GetBodyCenter();

	VECTOR3POINTTOPOINT(len, center, hitball, tmp);

	if (len > size * 3)return false;
	if (len < size)return true;

	VECTOR3POINTTOPOINT(len, TOCONTROLL(C)->m_base_param.pos, hitball, tmp);
	
	//VECTOR3POINTTOPOINT(len, TOCONTROLL(C)->m_base_param.pos, hitball, tmp);
	return (len < size);
}

void CharacterUtilityFunctions::UpdateDamageLifePoint(LPCharacter C)
{
	int& damage_timer = TOCONTROLL(C)->m_Param.m_last_damage_count;

	if (damage_timer < INT_MAX)++damage_timer;

	if (damage_timer>120)
	{
		TOCONTROLL(C)->m_Param.m_LifePoint = min(
			TOCONTROLL(C)->m_Param.m_LifePoint + 2.0f,
			GetConstChrParam(C)->max_hp);
	}

}

bool CharacterUtilityFunctions::GetRandomRespornPoint(TEAM_TYPE team, LPMapObjectInterface* ppOut)
{
	MapObjectManager::MAPOBJ_ARRAY obj;

	MAP_MANAGER.Get_TypeObject(
		obj,
		static_cast<MAP_OBJ_TYPE>(map_obj::_resporn_point_begin + team));

	int size = (int)obj.size();
	if (size <= 0)return false;

	*ppOut = obj[rand() % (int)size];

	return true;
}

void CharacterUtilityFunctions::SetStartCharacter(LPCHARACTER C, LPCONTROLLER cnt)
{
	ENTITY_ID id;
	const int team_type = ControllObjFunc::GetTeamType(C);

	MyAssert(IDMANAGER.GetEmptyControllObjID(&id), "IDに空きがありません");

#if 1
	StartControllobj* stObj = new StartControllobj(
		id,
		C,
		ControllObjFunc::GetTeamType(C),
		ControllObjFunc::GetTeamData(C)
		);
#else

	RespornControllobj* stObj = new RespornControllobj(
		ControllObjFunc::GetCamera(C),
		id,
		C,
		team_type,
		ControllObjFunc::GetTeamData(C),
		game_id::id_error
		);

#endif

	cnt->SetControllObj(stObj);
	CONTROLLOBJMGR.AddObject(stObj);

}

void CharacterUtilityFunctions::ApperVendingMachine(LPCHARACTER C, CRVector3 pos, float angle)
{
	TOCONTROLL(C)->m_Param.m_view_vec = Vector3(sinf(angle), 0, cosf(angle));
	TOCONTROLL(C)->m_base_param.pos = pos;	
}

LPCONTROLLOBJCT CharacterUtilityFunctions::RespornCharacter(LPCHARACTER C)
{
	const int team_type = ControllObjFunc::GetTeamType(C);
	CharacterUI* chr_ui = new CharacterUI;

	ENTITY_ID id;
	MyAssert(IDMANAGER.GetEmptyControllObjID(&id), "IDに空きがありません");

	TOCONTROLL(C)->m_pCharacterModel->SetMotion(chr_motion_type::STAND, chr_motion_parts::ALL_PARTS, 0, 1, true);
	TOCONTROLL(C)->m_pCharacterModel->SetMotion(chr_motion_type::STAND, chr_motion_parts::ALL_PARTS, 1, 0, true);

	LPCHARACTER pCharacter = new Character(
		id,
		TOCONTROLL(C)->m_pBrain,					//脳みそをプレイヤのものに
		TOCONTROLL(C)->m_pCharacterModel,
		chr_ui,
		team_type,
		ControllObjFunc::GetTeamData(C)
		);

	ControllObjFunc::SetMatchDataPtr(pCharacter, TOCONTROLL(C)->m_pMyMatchData);
	TOCONTROLL(C)->m_pBrain = nullptr;
	TOCONTROLL(C)->m_pCharacterModel = nullptr;

	chr_ui->SetOwner(pCharacter);

	//カメラステートをセット
	LPCAMERA cam = ControllObjFunc::GetCamera(pCharacter);

	cam->SetNewState(
		CameraFunction::GetCharacterUsualModeCamera(
		pCharacter,
		cam,
		CameraFunction::_RIGHT));

	MyAssert(IDMANAGER.GetEmptyControllObjID(&id), "IDに空きがありません");

	RespornControllobj* resporn = new RespornControllobj(
		ControllObjFunc::GetCamera(C),
		id,
		pCharacter,
		team_type,
		ControllObjFunc::GetTeamData(C),
		TOCONTROLL(C)->m_Param.m_last_damage_parent
		);

	CONTROLLOBJMGR.AddObject(resporn);

	((ControllerManager*)IDMANAGER.GetPointer(game_id::id_controller_manager))->SwapControllObject(
		C,
		resporn
		);

	chr_ui->SetColaGaugeColor(
		(CODMgr::GetTeamColaWaterColor(ControllObjFunc::GetMatchData(resporn)->team_type) & 0x00FFFFFF) | 0x50000000);

	DeleteCharacter(C);

	return resporn;
}


void CharacterUtilityFunctions::DeleteCharacter(LPCHARACTER C)
//キャラクタを抹消する
{
	CONTROLLOBJMGR.DeleteObject(C->GetId());
}

void CharacterUtilityFunctions::SetAutoAimPointEnable(LPCHARACTER C, bool flg)
{
	TOCONTROLL(C)->m_AimTarget.SetEnable(flg);
}

bool CharacterUtilityFunctions::HitCheckOtherControllObj(LPCHARACTER C)
{
	class pred
	{
		LPCONTROLLOBJCT me;
	public:
		pred(LPCONTROLLOBJCT me) :me(me){}
		bool operator ()(LPCONTROLLOBJCT obj){return me != obj;}
	};

	bool ret = false;
	LPCONTROLLOBJCT cit;
	Vector3& pos = TOCONTROLL(C)->m_base_param.pos, *itpos, v;
	CONTROLLOBJLIST clist = CONTROLLOBJMGR.GetObjectList(pred(C));
	
	float len;
	const float size = GetConstChrParam(nullptr)->check_wall_size * 2;

	for (CONTROLLOBJLIST::iterator it = clist.begin();
		it != clist.end();
		++it)
	{
		cit = *it;
		itpos = &TOCONTROLL(cit)->m_base_param.pos;

		if (fabsf(itpos->y - pos.y) > 1)continue;

		VECTOR3MINUS(v,*itpos, pos);
		v.y = 0;
		len = VECTOR3LENGTH(v);

		if (len > size)
		{
			continue;
		}
		ret = true;

		if (len <= 0.0f)
		{
			itpos->x += 0.001f;
			continue;
		}

		len = size - len;
		len /= 2.0f;

		v.Normalize();
		VECTOR3MULSCALAR(v, v, len);

		VECTOR3PLUS(*itpos, *itpos, v);
		v = -v;

		VECTOR3PLUS(pos, pos, v);
	}

	return ret;
}

bool CharacterUtilityFunctions::isShuffle(LPCharacter C)
{
	return TOCONTROLL(C)->m_pUserInterface->isShuffle();
}

bool CharacterUtilityFunctions::isZoom(LPCharacter C)
{
	return TOCONTROLL(C)->m_pLockOnClass->isZoom();
}

bool CharacterUtilityFunctions::WeponFire(LPCHARACTER C)
{
	return TOCONTROLL(C)->m_pWepon->Fire();
}

float CharacterUtilityFunctions::GetMaxChargeWeaponRange(LPCHARACTER C)
{
	return TOCONTROLL(C)->m_pWepon->GetParam().range_max * 120;
}

float CharacterUtilityFunctions::GetMinChargeWeaponRange(LPCHARACTER C)
{
	return TOCONTROLL(C)->m_pWepon->GetParam().range_min * 120;
}

float CharacterUtilityFunctions::GetNowChargeRange(LPCHARACTER C)
{
	
	return fLERP(
		TOCONTROLL(C)->m_pWepon->GetParam().range_min,
		TOCONTROLL(C)->m_pWepon->GetParam().range_max,
		TOCONTROLL(C)->m_pUserInterface->GetColaGaugePower()
		) * 120;

}

void CharacterUtilityFunctions::SetGaugeMove(LPCHARACTER C, CRVector2 pos, CRVector2 size)
{
	TOCONTROLL(C)->m_pUserInterface->SetGaugeMove(pos, size);
}

void CharacterUtilityFunctions::ResetGauge(LPCHARACTER C)
{
	TOCONTROLL(C)->m_pUserInterface->ResetGauge();
}

bool CharacterUtilityFunctions::isFrend(LPCHARACTER me, LPCHARACTER you)
{
	return (TOCONTROLL(me)->m_pMyMatchData->team_type ==
		TOCONTROLL(you)->m_pMyMatchData->team_type);
}

void CharacterUtilityFunctions::SetMotion(LPCHARACTER me, CHR_MOTION_PARTS parts, CHR_MOTION_TYPE motion)
{
	TOCONTROLL(me)->m_pCharacterModel->SetMotion(
		motion,
		parts,
		0,
		1,
		false
		);

	TOCONTROLL(me)->m_pCharacterModel->SetSpeed(
		parts,
		0,
		1
		);

	TOCONTROLL(me)->m_pCharacterModel->SetMotion(
		chr_motion_type::STAND,
		parts,
		1,
		0,
		false
		);

	TOCONTROLL(me)->m_pCharacterModel->SetSpeed(
		parts,
		1,
		1
		);
}

void CharacterUtilityFunctions::ResetMotion(LPCHARACTER me, CHR_MOTION_PARTS parts)
{
	TOCONTROLL(me)->m_pCharacterModel->SetMotion(
		chr_motion_type::STAND,
		parts,
		0,
		1,
		true
		);

	TOCONTROLL(me)->m_pCharacterModel->SetSpeed(
		parts,
		0,
		1
		);

	TOCONTROLL(me)->m_pCharacterModel->SetMotion(
		chr_motion_type::STAND,
		parts,
		1,
		0,
		true
		);

	TOCONTROLL(me)->m_pCharacterModel->SetSpeed(
		parts,
		1,
		1
		);
}

void CharacterUtilityFunctions::SetMotionSpeed(LPCHARACTER me, CHR_MOTION_PARTS parts, float speed)
{
	TOCONTROLL(me)->m_pCharacterModel->SetSpeed(
		parts,
		0,
		speed
		);

	TOCONTROLL(me)->m_pCharacterModel->SetSpeed(
		parts,
		1,
		speed
		);
}

bool CharacterUtilityFunctions::SetWalkingMotion(LPCHARACTER me, CRVector2 move_v, RATIO speed)
{
	CHR_MOTION_TYPE	m1, m2;
	float angle = GetAngle(me);

	Vector2 v(
		move_v.x*cosf(angle) + move_v.y*-sinf(angle),
		move_v.x*sinf(angle) + move_v.y*cosf(angle)
		);

	//一定以下ならたちモーション
	const float stand_value = 0.3f;

	if (speed < stand_value)
	{
		float wight[2];
		int change_motion_line = 0;
		RATIO walk_weight = speed / stand_value;

		TOCONTROLL(me)->m_pCharacterModel->GetWeight(
			chr_motion_parts::LOWER_BODY,
			wight
			);

		
		if (wight[0] > wight[1])
			change_motion_line = 1;

		TOCONTROLL(me)->m_pCharacterModel->SetMotion(
			chr_motion_type::STAND,
			chr_motion_parts::LOWER_BODY,
			change_motion_line,
			1 - walk_weight,
			false
		);

		TOCONTROLL(me)->m_pCharacterModel->SetWeight(
			chr_motion_parts::LOWER_BODY,
			(change_motion_line + 1) % 2 ,
			walk_weight
			);
		return false;
	}

	
	if (v.x > 0)
		m1 = chr_motion_type::MOVE_RIGHT;
	else
		m1 = chr_motion_type::MOVE_LEFT;

	if (v.y<0)
	{
		if (m1 == chr_motion_type::MOVE_RIGHT)
			m1 = chr_motion_type::MOVE_LEFT;
		else
			m1 = chr_motion_type::MOVE_RIGHT;
	}


	if (v.y > 0)
		m2 = chr_motion_type::MOVE_FRONT;
	else
		m2 = chr_motion_type::MOVE_BACK; 

	v.x = ABS(v.x);
	v.y = ABS(v.y);

	float t = v.x + v.y;

	if (t != 0.0f)
	{
		v.x /= t;
		v.y /= t;
	}


	TOCONTROLL(me)->m_pCharacterModel->SetMotion(
		m1,
		chr_motion_parts::LOWER_BODY,
		0,
		v.x,
		true
		);

	TOCONTROLL(me)->m_pCharacterModel->SetSpeed(
		chr_motion_parts::LOWER_BODY,
		0,
		speed
		);

	TOCONTROLL(me)->m_pCharacterModel->SetMotion(
		m2,
		chr_motion_parts::LOWER_BODY,
		1,
		v.y,
		false
		);

	TOCONTROLL(me)->m_pCharacterModel->SetSpeed(
		chr_motion_parts::LOWER_BODY,
		1,
		speed
		);


	float local_frame = TOCONTROLL(me)->m_pCharacterModel->GetMotionFrame(chr_motion_parts::LOWER_BODY, 1);
	
	TOCONTROLL(me)->m_pCharacterModel->SetMotionFrame(
		chr_motion_parts::LOWER_BODY,
		0,
		local_frame
		);

	return true;
}

void CharacterUtilityFunctions::SetOutLineColorHP(LPCHARACTER me)
{
	COLORf& color = TOCONTROLL(me)->m_Param.m_OutLineColor;
	const float maxHP = GetConstChrParam(me)->max_hp / 2;
	RATIO t = TOCONTROLL(me)->m_Param.m_LifePoint / maxHP;

	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 0.1f;

	if (t < 1)
	{
		color.r = (sinf(TOCONTROLL(me)->m_Param.m_Count*2)*0.5f + 0.5f)*(1 - t);
	//	color.a += color.r*0.1f;
	}
}

//cur=(cur+(max-1))%max;

void CharacterUtilityFunctions::CreateToBrainDamageMsg(
	LPCHARACTER	C, LPGameMessage msg, DamageParam* pDamage)
{
	msg->from = C->GetId();
	msg->to = C->GetId();
	msg->exinfo = pDamage;
	msg->type = msg_t::damage_hit;
}

void CharacterUtilityFunctions::SendMoveSpeedUpMsg(ENTITY_ID to)
{
	GameMessage msg;
	float value = GetConstChrParam(nullptr)->character_move_speed_up_value;

	msg.exinfo = &value;
	msg.from = game_id::id_error;
	msg.to = to;
	msg.type = msg_t::up_character_move_speed;

	IDMANAGER.Dispatch(&msg);
}


void CharacterUtilityFunctions::SendChargeSpeedUpMsg(ENTITY_ID to)
{
	GameMessage msg;
	float value = GetConstChrParam(nullptr)->character_charge_speed_up_value;

	msg.exinfo = &value;
	msg.from = game_id::id_error;
	msg.to = to;
	msg.type = msg_t::up_character_charge_speed;

	IDMANAGER.Dispatch(&msg);
}

void CharacterUtilityFunctions::SetPlusAlpha(LPCHARACTER me, RATIO a)
{
	TOCONTROLL(me)->m_Param.UpValue.PowerUpAlpha = a;
}


void CharacterUtilityFunctions::SetPowerUpEffect(LPCHARACTER me, MSG_TYPE type)
{
	SetPlusAlpha(me, 1);

	GameMessage msg;
	TextureRect texrc;
	Vector2 pos;

	texrc.left = 0;
	texrc.top = 0;
	texrc.width = 200;
	texrc.height = 128;

	switch (type)
	{
	case msg_t::up_character_charge_speed:
		pos.x = 1100;
		pos.y = 600;
		texrc.top = 128;
		break;
	case msg_t::up_character_move_speed:
		pos.x = 700;
		pos.y = 650;
		texrc.top = 0;
		break;
	default:
		MyAssert(false, "CharacterUtilityFunctions::SetPowerUpEffectに対応していないメッセージタイプが送られました。type=%d", (int)type);
		break;
	}

	PowerUpIcon* icon = new PowerUpIcon(
		GAMEMATCHINFO.GetTextureMgr().GetTexturePtr(TextureManager::POWER_UP),
		texrc,
		me->GetId(),
		pos,
		Vector2(0, -1.5f),
		Vector2(0,0),
		Vector2(100, 60),
		60
		);

	msg.to = game_id::id_screen_icon_manager;
	msg.from = game_id::id_error;
	msg.type = msg_t::send_screen_icon_msg;
	msg.exinfo = icon;

	IDMANAGER.Dispatch(&msg);
}

void CharacterUtilityFunctions::UpdateUpValueAlpha(LPCHARACTER me)
{
	float& alpha = TOCONTROLL(me)->m_Param.UpValue.PowerUpAlpha;
	const float speed = 0.0125f;

	if (alpha > 0.5f)
		alpha -= speed * 2;
	else
		alpha -= speed;

	alpha = max(0, alpha);
}

bool CharacterUtilityFunctions::PlayFootSound(LPCHARACTER me,CHARACTER_BONE_TYPE& out_touch_foot)
{
	const int sound_frame[] = { 8, 16 };

	const float f =
		TOCONTROLL(me)->m_pCharacterModel->GetMotionFrame(
		chr_motion_parts::LOWER_BODY,
		1);
	

	for (int i = 0; i < ARRAYSIZE(sound_frame); ++i)
	{
		if (f >= sound_frame[i] && f < sound_frame[i] + 1)
		{
			SOUNDPLAYER.PlaySE(
				sound_t::foot_sound,
				SOUNDPLAYER.GetSoundVolume(
					TOCONTROLL(me)->m_base_param.pos,
					sound_t::foot_sound)
				);

			out_touch_foot = (i == 0) ? (chr_bone_type::RIGHT_LEG_3) : (chr_bone_type::LEFT_LEG_3);

			return true;
		}
	}
	return false;
}


void CharacterUtilityFunctions::SendFriendIconMsgToBrain(LPCHARACTER me, FriendMessageIcon::MsgType type)
{
	class pred
	{
		LPCONTROLLOBJCT me;
		TEAM_TYPE t;
	public:
		pred(TEAM_TYPE t, LPCONTROLLOBJCT me) :t(t), me(me){}
		bool operator ()(LPCONTROLLOBJCT obj)
		{
			return ControllObjFunc::GetTeamType(obj) == t && (me != obj);
		}
	};
	GameMessage msg;
	pred pr(ControllObjFunc::GetTeamType(me), me);
	FriendIconMsgInfo info;
	CONTROLLOBJLIST cList = CONTROLLOBJMGR.GetObjectList(pr);
	//LPCONTROLLOBJCT i;

	info.sender = me->GetId();
	info.to = game_id::id_error;
	info.type = type;

	msg.from = me->GetId();
	msg.type = msg_t::apper_friend_msg_icon;
	msg.exinfo = &info;
	npc_commander->HandleMessage(ControllObjFunc::GetMatchData(me)->team_type, &msg);


	//GameMessage msg;
	//pred pr(ControllObjFunc::GetTeamType(me), me);
	//FriendIconMsgInfo info;
	//CONTROLLOBJLIST cList = CONTROLLOBJMGR.GetObjectList(pr);
	//LPCONTROLLOBJCT i;

	//for (CONTROLLOBJLIST::iterator it = cList.begin();
	//	it != cList.end();
	//	++it)
	//{
	//	i = *it;

	//	info.sender = me;
	//	info.type = type;
	//	info.to = i->GetId();

	//	msg.to = i->GetId();
	//	msg.exinfo = &info;
	//	msg.from = me->GetId();
	//	msg.type = msg_t::apper_friend_msg_icon;

	//	i->HandleMessage(&msg);
	//}
}


void CharacterUtilityFunctions::RenderEye(LPCHARACTER me)
{
	Vector3 right_eye, left_eye, right, up, front;

	LPCHARACTERMODELBASE pModel = TOCONTROLL(me)->m_pCharacterModel;
	
	const Matrix& world_mat = pModel->GetMatrix();

	//目のボーン位置がおかしいので応急処置
#if 0
	const Matrix& eye_r = pModel->GetBoneMatrix(chr_bone_type::EYE_RIGHT);
	const Matrix& eye_l = pModel->GetBoneMatrix(chr_bone_type::EYE_LEFT);

	//それぞれの目の位置を計算
	right_eye.x = eye_r._41;
	right_eye.y = eye_r._42;
	right_eye.z = eye_r._43;

	left_eye.x = eye_l._41;
	left_eye.y = eye_l._42;
	left_eye.z = eye_l._43;

	right_eye = Vector3MulMatrix(right_eye, world_mat);
	left_eye = Vector3MulMatrix(left_eye, world_mat);

#else
	Matrix head = pModel->GetBoneMatrix((CHARACTER_BONE_TYPE)13);
	head *= world_mat;


	//位置

	right_eye.x = head._41;
	right_eye.y = head._42;
	right_eye.z = head._43;

	left_eye = right_eye;

	up.x = -head._11;
	up.y = -head._12;
	up.z = -head._13;

	front.x = head._21;
	front.y = head._22;
	front.z = head._23;

	right.x = -head._31;
	right.y = -head._32;
	right.z = -head._33;


#endif

	right.Normalize();
	up.Normalize();
	front.Normalize();

	//両目を左右にずらす
	left_eye -= right*0.125f;
	right_eye += right*0.125f;

	left_eye += front*0.25f;
	right_eye += front*0.25f;

	left_eye += up*0.05f;
	right_eye += up*0.05f;

	//目を描画
	TOCONTROLL(me)->m_Eye.Render(
		right_eye,
		left_eye,
		front,
		right,
		up
		);
}

void CharacterUtilityFunctions::ApperEmptyCan(
	CRVector3 pos, float size, TEAM_TYPE type)
{

	GameMessage msg;

	EmptyCan* can = new EmptyCan(
		GAMEMATCHINFO.GetMeshMgr().GetMeshPtr((MeshManager::MESH_ID)GameMatchInfomation::_EMPTY_CAN),
		GAMEMATCHINFO.GetTextureMgr().GetTexturePtr((TextureManager::TEXTURE_ID)(TextureManager::CHR_TEXTURE_SODA + type)),
		120,
		size,
		pos
		);

	msg.exinfo = can;
	msg.from = game_id::id_error;
	msg.to = game_id::id_bullet_manager;
	msg.type = msg_t::add_bullet;

	MyAssert(IDMANAGER.Dispatch(&msg), "id_bullet_managerに空き缶メッシュクラスの送信を送信するときにエラーが発生しました");
	

}