#include "All.h"


#define TOCONTROLL(ptr) (*(CharacterMoveFunctions*)(ptr))


static void SendStateChangeMsg(LPCHARACTER p, CHARACTER_STATE_TYPE type)
{
	GameMessage msg;
	msg.to = p->GetId();
	msg.exinfo = &type;
	msg.type = msg_t::chr_table_state_change;
	p->HandleMessage(&msg);
}

bool CharacterMoveFunctions::UpCheck(LPCHARACTER C)
{
	Vector3 out, pos(TOCONTROLL(C).m_base_param.pos), vec(0, 1, 0);
	float dist = 1000000000;

	if (MAP_MANAGER.RayPick(&out, &pos, &vec, &dist) &&
		out.y > TOCONTROLL(C).m_base_param.pos.y + 0.1f&&
		out.y < TOCONTROLL(C).m_base_param.pos.y + 0.9f)
	{
		if (TOCONTROLL(C).m_Param.m_move.y>0)
		{
			TOCONTROLL(C).m_base_param.pos.y = out.y - 0.9f;
		}
		return true;
	}
	return false;
}

bool CharacterMoveFunctions::UpdateAllMoveFunc(LPCHARACTER C, float max_speed_val, RATE down_speed_val,float glavity)
{
	//重力関係
	ChrMove::AddMove(C, Vector3(0, glavity, 0));

	//床とのあたり判定チェック
	bool isHitFloor;
	FUNCTIONSPEEDCHECK2(ChrMove::FloorCheck(C), isHitFloor);


	//ＸＺ軸の最大移動速度を設定
	ChrMove::SetMaxXZSpeed(C, max_speed_val);

	//位置を更新
	ChrMove::PositionUpdate(C);

	//速度を減速させる
	if (isHitFloor)ChrMove::DownSpeed(C, down_speed_val);

	FUNCTIONSPEEDCHECK(
		//壁とのあたり判定チェック
		ChrMove::WallCheck(C));
	
	return isHitFloor;
}

void CharacterMoveFunctions::UpdateAimTargetInfo(
	LPCHARACTER C,
	LPTEAMVIEWDATA viewData)
{
	AimTargetInfo target;

	if (!TOCONTROLL(C).m_pUserInterface->GetTarget(target) ||
		target.team_type == TOCONTROLL(C).m_pMyMatchData->team_type ||
		target.team_type == -1)
		return;

	TeamViewData::DataParam d;

	d.alpha = 1;
	d.color = target.color;
	d.id = target.entity_id;
	d.time = 180;

	viewData->SetData(d);
}

bool CharacterMoveFunctions::UpdateParachuteMoveFunc(
	LPCHARACTER C,
	float max_speed_val,
	float down_speed)
{
	//重力関係
	ChrMove::AddMove(C, Vector3(0, GetConstChrParam(C)->glavity, 0));

	if (TOCONTROLL(C).m_Param.m_move.y < -down_speed)
	{
		TOCONTROLL(C).m_Param.m_move.y = -down_speed;
	}

	//床とのあたり判定チェック
	bool isHitFloor;
	FUNCTIONSPEEDCHECK2(ChrMove::FloorCheck(C), isHitFloor);

	//ＸＺ軸の最大移動速度を設定
	ChrMove::SetMaxXZSpeed(C, max_speed_val);

	//位置を更新
	ChrMove::PositionUpdate(C);

	FUNCTIONSPEEDCHECK(
		//壁とのあたり判定チェック
		ChrMove::WallCheck(C));
	
	return true;

}

void CharacterMoveFunctions::AddMove(LPCHARACTER C, CRVector3 move)		
{
	//指定した方向に移動量を加算する
	TOCONTROLL(C).m_Param.m_move += move;
}


void CharacterMoveFunctions::PositionUpdate(LPCHARACTER C)
{
	//座標に移動量を加算する
	TOCONTROLL(C).m_base_param.pos += TOCONTROLL(C).m_Param.m_move;
}



void CharacterMoveFunctions::DownSpeed(LPCHARACTER C, RATE val)
{
	//移動量をvalの割合だけ減少させる
	Vector3& m = TOCONTROLL(C).m_Param.m_move;
	m.x *= (1 - val);
	m.z *= (1 - val);
}
void CharacterMoveFunctions::SetMaxXZSpeed(LPCHARACTER C, float max_val)
{
	//XZ平面の最大移動量をmax_valに丸める
	Vector3& m = TOCONTROLL(C).m_Param.m_move;
	float l = sqrtf(m.x*m.x + m.z*m.z);

	if (l > max_val)
	{
		m.x = max_val*(m.x / l);
		m.z = max_val*(m.z / l);
	}
}

bool CharacterMoveFunctions::FloorCheck(LPCHARACTER C)	
//床とのあたり判定チェック
{
	Vector3 out;
	LPCHARACTERCONSTPARAMETERS cParam = GetConstChrParam(C);
	const float ride_height = cParam->ride_height, adjust_floor = cParam->adjust_floor;
	CharacterMoveFunctions& chr = TOCONTROLL(C);
	LPMapObjectInterface pRideObj;
	bool hit_up;
	//乗っているオブジェクトを更新
	LPMapObjectInterface pPreRideObj = (LPMapObjectInterface)IDMANAGER.GetPointer(chr.m_Param.m_RideObjectId);
	
	if (pPreRideObj)
	{
		pPreRideObj->ReleaseChild(C);
	}

	if ((hit_up = UpCheck(C)) && chr.m_Param.m_move.y > 0)
	{
		chr.m_Param.m_move.y = 0;
	}
	
	//地面との判定
	if ((pRideObj =
		collision::HitCheckFloor(
		&out,
		ControllObjFunc::GetPos(C),
		//ride_height,
			max(-chr.m_Param.m_move.y + ride_height + 0.2f, ride_height), 
			adjust_floor,
			cParam->check_wall_size*0.7f,
			0
			)) 
		!= nullptr)
	{
		pRideObj->AddChild(C);
		chr.m_Param.m_RideObjectId = pRideObj->GetId();

		//衝撃を与える
		if (!pPreRideObj ||
			(pPreRideObj != pRideObj))
		{
			GameMessage msg;
			msg.from = C->GetId();
			msg.to = pRideObj->GetId();
			msg.type = msg_t::ride_obj;
			msg.exinfo = C;
			pRideObj->HandleMessage(&msg);
		}

		//地面の場所に
		chr.m_base_param.pos.y = out.y;

		if (hit_up)
		{
			GameMessage msg;
			CHARACTER_STATE_TYPE state = chr_move_state::die_mode;
			msg.exinfo = &state;
			msg.to = msg.from = C->GetId();
			msg.type = msg_t::chr_table_state_change;
			C->HandleMessage(&msg);

			ChrFunc::ApperEmptyCan(ControllObjFunc::GetPos(C), 0.22f, ControllObjFunc::GetTeamType(C));
		}

		//ここの処理はまだ悩み中
		if ((chr.m_Param.m_move.y)<0.03f)
			chr.m_Param.m_move.y = 0;

		chr.m_frame_info.isHitFloor = true;

		return true;
	}

	chr.m_Param.m_RideObjectId = game_id::id_error;

	if (pPreRideObj)
	{
		chr.m_Param.m_move += chr.m_Param.m_parent_move;
		VECTOR3TOZERO(chr.m_Param.m_parent_move);
	}

	chr.m_frame_info.isHitFloor = false;

	return false;
}

bool CharacterMoveFunctions::CeilingCheck(LPCHARACTER C)
{
	//CharacterMoveFunctions& chr = TOCONTROLL(C);
	//Vector3 out, pos, vec;
	//float dist = 10000;

	//pos = chr.m_base_param.pos;


	//MAP_MANAGER.RayPick(&out,

	return false;
}

bool CharacterMoveFunctions::WallCheckGetNormal(
	LPCHARACTER C,
	Vector3& out_normal)
{
	Vector3 out, m;
	LPCHARACTERCONSTPARAMETERS cParam = GetConstChrParam(C);
	const float character_size = cParam->check_wall_size;
	CharacterMoveFunctions& chr = TOCONTROLL(C);
	Vector3 pos = chr.m_base_param.pos;
	pos.y += cParam->check_wall_height;

	if (Vector3XZLength(chr.m_Param.m_move) < 0.001f)
		m = VECTOR3AXISZ;
	else
		m = chr.m_Param.m_move;

	if (collision::HitCheckWallGetNormal(&out, pos, m, character_size, out_normal))
	{
		chr.m_base_param.pos.x = out.x;
		chr.m_base_param.pos.z = out.z;

		chr.m_frame_info.isHitWall = true;

		return true;
	}
	chr.m_frame_info.isHitWall = false;

	return false;
}

void CharacterMoveFunctions::Bound(LPCHARACTER C,CRVector3 normal, RATIO r)
{
	Vector3& m = TOCONTROLL(C).m_Param.m_move;
	m = Vector3Reflect(m, normal, r);
}

bool CharacterMoveFunctions::Hover(LPCHARACTER C, RATIO power)
{
	Vector3 move(0, power * 2, 0);
	const RATIO charge = ChrFunc::GetChargePower(C);

	if (charge <= 0.01)return false;

	move.y *= min(charge, 0.5f);
	move.y *= 0.025f;

	if (TOCONTROLL(C).m_Param.m_move.y > 0.05f)
		move.y *= 0.5f;

	if (TOCONTROLL(C).m_Param.m_move.y < 0.0f)
		TOCONTROLL(C).m_Param.m_move.y *= 0.5f;

	ChrMove::AddMove(C, move);
	ChrMove::SetMaxXZSpeed(C, GetConstChrParam(C)->jump_move_min_val);

	ChrMove::AddShuffleGauge(C, -GetConstChrParam(C)->cola_gauge_fire_down_speed * 2);

	COLORf color(0xA0abaaa8);
	Vector3 pos = ControllObjFunc::GetPos(C);
	Vector3 m(0, -0.2f, 0);
	pos.y += 0.1f;

	color.a = 1;
	color.ClampValue();
	DWORD col = color.toDWORD();

	for (int i = 0; i < 2; ++i)
	{
		POLYGONEFFECT.SetEffect(
			pos + Vector3Rand()*0.1f,
			m,
			VECTOR3ZERO,
			0.20f,
			EffectTex::_DUST,
			col,
			RS_ADD,
			17
			);
	}

	if (rand() % 2 == 0)
	{
		POLYGONEFFECT.SetEffect(
			pos + Vector3Rand()*0.1f,
			m + Vector3Rand()*0.001f,
			VECTOR3ZERO,
			fRAND(0.02f, 0.07f),
			EffectTex::_SOUR,
			CODMgr::GetTeamColaWaterColor(ControllObjFunc::GetTeamType(C)),
			RS_COPY
			);
	}

	ControllObjFunc::GetCamera(C)->SetShock(10.f, 5);


	return true;
}

bool CharacterMoveFunctions::WallCheck(LPCHARACTER C)
//壁とのあたり判定チェック
{
	Vector3 out, m;
	LPCHARACTERCONSTPARAMETERS cParam = GetConstChrParam(C);
	const float character_size = cParam->check_wall_size;
	CharacterMoveFunctions& chr = TOCONTROLL(C);
	Vector3 pos = chr.m_base_param.pos;
	pos.y += cParam->check_wall_height;
	float to_wall_dot;

	if (Vector3XZLength(chr.m_Param.m_move) < 0.001f)
		m = VECTOR3AXISZ;
	else
		m = chr.m_Param.m_move;

	if (collision::HitCheckWallR2(&out, pos, m, character_size, &to_wall_dot))
	{
		to_wall_dot = 1 + to_wall_dot*0.1f;
		to_wall_dot = min(to_wall_dot, 1);

		chr.m_base_param.pos.x = out.x;
		chr.m_base_param.pos.z = out.z;

		//移動力の補正
		chr.m_Param.m_move.x *= to_wall_dot;
		chr.m_Param.m_move.z *= to_wall_dot;

		chr.m_frame_info.isHitWall = true;

		return true;
	}
	chr.m_frame_info.isHitWall = false;

	return false;
}

void CharacterMoveFunctions::AngleSet(LPCHARACTER C, CRVector3 view_vec)
{
	//指定した方向に向かせる(一瞬で)
	TOCONTROLL(C).m_Param.m_view_vec = view_vec;
	TOCONTROLL(C).m_Param.m_view_vec.Normalize();
	CheckMaxViewUpAndDown(C, TOCONTROLL(C).m_Param.m_view_vec);
}


void CharacterMoveFunctions::AngleControll(LPCHARACTER C, CRVector3 view_pos, RADIAN speed)
{
	//指定した場所に向かってspeedの速度で向く
	Vector3 v1 = TOCONTROLL(C).m_Param.m_view_vec, v2 = view_pos - C->GetBodyCenter(), axis;

	float angle = Vector3Radian(v1, v2);
	angle = min(angle, speed);

	Vector3Cross(axis, v1, v2);

	if (Vector3isZero(axis) || Vector3isZero(v1))return;

	TOCONTROLL(C).m_Param.m_view_vec = Vector3RotateAxis(Vector3Normalize(axis), angle, Vector3Normalize(v1));

	CheckMaxViewUpAndDown(C, TOCONTROLL(C).m_Param.m_view_vec);
}

void CharacterMoveFunctions::AngleControllAxisY(LPCHARACTER C, CRVector3 view_pos, RADIAN speed)
{
	//指定した場所に向かってspeedの速度で向く
	Vector3 v1 = TOCONTROLL(C).m_Param.m_view_vec, v2 = view_pos - C->GetBodyCenter(), axis;

	v1.y = 0; v2.y = 0;

	float angle = Vector3Radian(v1, v2);
	angle = min(angle, speed);

	Vector3Cross(axis, v1, v2);

	if (Vector3isZero(axis) || Vector3isZero(v1))return;

	TOCONTROLL(C).m_Param.m_view_vec = Vector3RotateAxis(Vector3Normalize(axis), angle, Vector3Normalize(v1));

	CheckMaxViewUpAndDown(C, TOCONTROLL(C).m_Param.m_view_vec);
}

void CharacterMoveFunctions::CheckMaxViewUpAndDown(LPCHARACTER C, Vector3& v)
{
	Vector3& view_vec = v;
	Vector3 axis;
	Vector3Cross(axis, CHARACTER_UPVEC, view_vec);
	axis.Normalize();

	//補正
	LPCHARACTERCONSTPARAMETERS pParam = GetConstChrParam(C);
	RADIAN rad = Vector3Radian(view_vec, CHARACTER_UPVEC);

	if (rad< pParam->rotate_max_up)
	{
		view_vec = Vector3RotateAxis(axis, pParam->rotate_max_up - rad, view_vec);
	}

	rad = PI - rad;

	if (rad < pParam->rotate_max_down)
	{
		view_vec = Vector3RotateAxis(axis, -(pParam->rotate_max_down - rad), view_vec);
	}
}

void CharacterMoveFunctions::Vanish(LPCHARACTER C, CRVector3 move)
{
	GameMessage gmsg;
	CharacterStateMessage cmsg;

	gmsg.from = gmsg.to= C->GetId();
	gmsg.type = msg_t::to_chr_state;
	gmsg.exinfo = &cmsg;

	cmsg.type = chr_state_msg_type::send_vanish_vec;
	cmsg.v1 = move;

	SendStateChangeMsg(C, chr_move_state::vanish);
	C->HandleMessage(&gmsg);
}

DAMAGEFLAGS CharacterMoveFunctions::UsualDamageCheck(LPCHARACTER C)
{

	CharacterMoveFunctions& chr = TOCONTROLL(C);
	DamageObjectManager* dmmgr = (DamageObjectManager*)IDMANAGER.GetPointer(game_id::id_damage_manager);
	DAMAGEVECTOR damage_array = dmmgr->GetInDamage(chr.m_base_param.pos, 3);
	
	int i = (int)damage_array.size();

	if (i<=0)return (DAMAGEFLAGS)0;
	--i;

	DAMAGEFLAGS ret = 0x0;
	DamageObject* target;
	CharacterUI* ui = ChrFunc::GetUI(C);
	PositionEffect::AnimeEffectParam pparam;
	LPCONTROLLOBJCT pObj;

	for (DamageObject** first = &damage_array.at(0);i >= 0;--i)
	{
		target = first[i];
		if (!ChrFunc::HitCheck(C, target->GetPos(), target->GetSize()))continue;
		
		target->param.hit_count++;

		pparam.color = target->param.color;

		if (IS_CONTROLLOBJ_ID(target->param.parent) &&
			(pObj = (LPCONTROLLOBJCT)IDMANAGER.GetPointer(target->param.parent)))
		{
		//	ControllObjFunc::PlayDamageHitSound(pObj);
		}

		if (damage_check::isCureDamage(C, target->param))
		{
			SetDamageEffect(C, damage_type_bits::team_cure_damage, target->param);

			ret |= damage_type_bits::team_cure_damage;
			ChrFunc::SetCureValue(C, target->param.val);
			pparam.max_time = pparam.time = 120;
			pparam.pos.x = -target->param.vec.x;
			pparam.pos.y = -target->param.vec.z;
			pparam.pos = Vector2Normalize(pparam.pos);
			pparam.angle = fRAND(-PI, PI);
			pparam.size.x = (float)(80 + rand() % 20);
			pparam.size.y = (float)(80 + rand() % 20);

			ui->SetDamageEffect(pparam, false);

			continue;
		}
		if (damage_check::isEnemyDamage(C, target->param))
		{
			ret |= damage_type_bits::enemy_damage;
			if (target->param.val <= 0.0f)continue;

			SetDamageEffect(C, damage_type_bits::enemy_damage, target->param);

			ChrFunc::SetDamageValue(C, target->param.val);

			GameMessage msg;
			ChrFunc::CreateToBrainDamageMsg(C, &msg, &target->param);
			C->HandleMessage(&msg);

			pparam.max_time = pparam.time = 120;
			pparam.pos.x = -target->param.vec.x;
			pparam.pos.y = -target->param.vec.z;
			pparam.pos = Vector2Normalize(pparam.pos);
			pparam.angle = fRAND(-PI, PI);
			pparam.size.x = (float)(80 + rand() % 20);
			pparam.size.y = (float)(80 + rand() % 20);

			ui->SetDamageEffect(pparam);

		//	ControllObjFunc::GetCamera(C)->SetShock(0.05f, 3);

			chr.m_Param.m_last_damage_parent = target->param.parent;
			chr.m_Param.m_last_damage_count = 0;

			if (IS_TYPE_DAMAGE(target->param.type,
				damage_type_bits::blow_off_damage))
			{
				Vector3 v = Vector3Normalize(C->GetBodyCenter() - target->GetPos());
				v.y = max(v.y, 0.3f);
				v.Normalize();
				v *= 0.26f;
				
				Vanish(
					C,
					v
					);

				return ret;
			}
			if (IS_TYPE_DAMAGE(target->param.type,
				damage_type_bits::apper_crushed_mesh))
			{
				ChrFunc::ApperEmptyCan(ControllObjFunc::GetPos(C),0.22f,ControllObjFunc::GetTeamType(C));
			}

			continue;
		}
		target->param.hit_count--;
	}

	return ret;
}

void CharacterMoveFunctions::SetDamageEffect(LPCHARACTER C, DAMAGEFLAGS dFlags,const DamageParam& dParam)
{

	if (IS_TYPE_DAMAGE(dFlags, damage_type_bits::enemy_damage)||
		IS_TYPE_DAMAGE(dFlags, damage_type_bits::team_cure_damage))
	{
		Vector3 effect_pos = dParam.vec;

		effect_pos.Normalize();
		effect_pos *= -0.5f;
		effect_pos += dParam.pos;


		POLYGONEFFECT.SetEffect(
			effect_pos,
			VECTOR3ZERO,
			VECTOR3ZERO,
			0.4f,
			EffectTex::_COLA_HIT,
			dParam.color,
			RS_COPY
			);

		POLYGONEFFECT.SetEffect(
			effect_pos,
			VECTOR3ZERO,
			VECTOR3ZERO,
			1.2f,
			EffectTex::_SPLASH,
			dParam.color,
			RS_COPY
			);
	}
}

CHARACTER_STATE_TYPE CharacterMoveFunctions::DamageStateUpdate(LPCHARACTER C,DAMAGEFLAGS flgs)
{
	//if (ChrFunc::GetStateType(C) == chr_move_state::vanish)
	//{
	//	return chr_move_state::vanish;
	//}

	if (ChrFunc::isDead(C))
	{
		SendStateChangeMsg(C, chr_move_state::die_mode);
		return chr_move_state::die_mode;
	}

	if (ChrFunc::isDying(C))
	{
		SendStateChangeMsg(C, chr_move_state::dying_mode);
		return chr_move_state::dying_mode;
	}

	return chr_move_state::error_mode;
}


void CharacterMoveFunctions::Shuffle(LPCHARACTER C, bool flg)
{
	TOCONTROLL(C).m_pUserInterface->SetShuffleFlg(flg);
	TOCONTROLL(C).m_pLockOnClass->Shuffle(flg);
}

void CharacterMoveFunctions::LockOn(LPCHARACTER C, bool flg)			//ロックオンするかどうか
{
	TOCONTROLL(C).m_pLockOnClass->LockOn(flg);
}

void CharacterMoveFunctions::LockOnRLChange(LPCHARACTER C)
{
	TOCONTROLL(C).m_pLockOnClass->ChangeRL();
}

void CharacterMoveFunctions::AddShuffleGauge(LPCHARACTER C,RATIO power)
{
	TOCONTROLL(C).m_pUserInterface->AddColaGauge(power);
}

void CharacterMoveFunctions::SetExplositonBlur(CRVector3 pos, RATIO power)
{
	class predicate
	{
	public:
		bool operator ()(LPCONTROLLOBJCT obj){
			return true;
		}
	};
	CONTROLLOBJLIST cList = CONTROLLOBJMGR.GetObjectList(predicate());
	LPCONTROLLOBJCT obj; 
	Vector3			scr_tmp,v;
	Vector2			scr_pos;
	const float		exp_len = GetConstChrParam(nullptr)->explode_blur_len;
	const float		shock_len = 30.0f;
	LPCAMERA		cam;
	float			p,l,t,z;
	

	for (CONTROLLOBJLIST::iterator it = cList.begin(), end = cList.end();
		it != end;
		++it
		)
	{
		obj = *it;
		cam = ControllObjFunc::GetCamera(obj);

		v = cam->GetPos() - ControllObjFunc::GetPos(obj);
		l = v.Length();

		//if (l < shock_len)
		//{
		//	cam->SetShock(
		//		0.03f*(1-(l / shock_len)),
		//		30
		//		);
		//}


		if (cam->ClientToScreen(scr_tmp, pos) == false ||
			scr_tmp.z > exp_len)
		{
			continue;
		}

		//if (scr_tmp.z < 7 &&
		//	Vector3XYLength(scr_tmp) < 0.7f
		//	)
		//{
		//	ControllObjFunc::SetWaterDropParam(obj, true);
		//}

		z = scr_tmp.z / exp_len;
		z *= z;

		p = (1 - (z))*power;
		if (p > 1)p = 1;
		t = p * 60;

		scr_pos.x = scr_tmp.x*0.5f + 0.5f;
		scr_pos.y = scr_tmp.y*0.5f + 0.5f;

		ControllObjFunc::SetBlur(obj, t, p, scr_pos);
	}

}

void CharacterMoveFunctions::SetExplositon(CRVector3 pos, RATIO power,  LPCHARACTER C)
{
	DamageParam param;
	param.color = CODMgr::GetTeamColaWaterColor(ControllObjFunc::GetMatchData(C)->team_type);
	param.hit_count = 0;
	param.parent = C->GetId();
	param.pos = pos;
	param.type = damage_type_bits::enemy_damage | damage_type_bits::team_cure_damage;
	param.val = 0.0f;

	VECTOR3TOZERO(param.vec);

	SetExplositon(pos, power, param, 2);
}


void CharacterMoveFunctions::SetExplositon(CRVector3 pos, RATIO power, const DamageParam& param, float damage_size)
{

	const int explode_cola_particle_num = (GAMEMATCHINFO.GetMatchData().player_num > 2) ? (16) : (40);
	const int explode_cola_num = (GAMEMATCHINFO.GetMatchData().player_num > 2) ? (3) : (20);


	const int smoke_num = 5;
	const float smoke_size = 1.5f*(max(power, 0.3f));
	Vector3 move;
	const Vector3 glavity(0, -0.003f, 0);
	ColaWater* cw;

	for (int i = 0; i < explode_cola_particle_num; i++)
	{
		move.x = fRAND(-0.1f, 0.1f);
		move.z = fRAND(-0.1f, 0.1f);

		move.y = fRAND(-0.1f, 0.55f);

		POLYGONEFFECT.SetEffect(
			pos,
			move,
			glavity,
			fRAND(0.01f, 0.25f),
			EffectTex::_SOUR,
			param.color,
			RS_COPY,
			80 + rand() % 60
			);
	}

	BulletManager* blt = (BulletManager*)IDMANAGER.GetPointer(game_id::id_bullet_manager);

	for (int i = 0; i < explode_cola_num; i++)
	{
		move.x = fRAND(-0.2f, 0.2f);
		move.z = fRAND(-0.2f, 0.2f);

		move.y = fRAND(-0.2f, 0.02f);

		ColaGun::ParamColaFactory(
			&cw,
			pos,
			param.color,
			move,
			0.1f,
			param.parent
			);

		blt->AddBullet(cw);
	}

	VECTOR3TOZERO(move);
	move.y = 0.01f;

	DamageObject* dmg = new DamageObject((DamageObjectManager*)IDMANAGER.GetPointer(game_id::id_damage_manager));
	
	dmg->param = param;

	POLYGONEFFECT.SetDamageEffect(
		pos,
		move,
		VECTOR3ZERO,
		3.5f,
		EffectTex::_SMOKE,
		3,
		5,
		dmg
		);
	

	for (int i = 0; i < smoke_num; ++i)
	{
		move = pos;

		move.x += fRAND(-0.25f, 0.25f);
		move.y += fRAND(-0.25f, 0.25f);
		move.z += fRAND(-0.25f, 0.25f);

		POLYGONEFFECT.SetEffect(
			move,
			VECTOR3ZERO,
			VECTOR3ZERO,
			1.5f,
			EffectTex::_SMOKE,
			0xFFFFFFFF,
			RS_ADD
			);
	}

	SetExplositonBlur(pos, min(power,1));

	SOUNDPLAYER.PlaySE(
		sound_t::explode,
		SOUNDPLAYER.GetSoundVolume(pos, sound_t::explode)
		);

	MapObjectManager::MAPOBJ_ARRAY objs;
	MAP_MANAGER.Get_TypeObject(objs, map_obj::_crane_steel_frame);

	Vector3 force;
	float force_len;

	for (MapObjectManager::MAPOBJ_ARRAY::iterator it = objs.begin();
		it != objs.end();
		++it)
	{
		
		CraneSteelFrame* pIt = (CraneSteelFrame*)*it;
		force = pIt->Get_Param().pos;
		force -= pos;

		force_len = force.Length();

		if (force_len > pIt->AddPowerSize)continue;

		force_len = 1 - (force_len / pIt->AddPowerSize);
		force_len *= 0.75f;

		force_len = max(force_len, 0.5f);

		force.Normalize();
		force *= force_len;

		pIt->AddForce(force);
	}
}


void CharacterMoveFunctions::SetFriendMessegeIcon(LPCHARACTER C, FriendMessageIcon::MsgType type)
{
	FriendMessageIcon* icon = new FriendMessageIcon(
		C->GetId(),
		GAMEMATCHINFO.GetTextureMgr().GetTexturePtr(TextureManager::FRIEND_MSG_ICON),
		{ (int)type * 128, 0, 128, 128 }, 
		C->GetBodyCenter(),
		Vector2(100, 100)
		);
	icon->SetTeamType(ControllObjFunc::GetTeamType(C));

	GameMessage msg;

	msg.to = game_id::id_screen_icon_manager;
	msg.from = game_id::id_error;
	msg.type = msg_t::send_screen_icon_msg;
	msg.exinfo = icon;

	MyAssert(IDMANAGER.Dispatch(&msg), "シェアオブジェクトアイコンの作成メッセージ送信に失敗しました");

}