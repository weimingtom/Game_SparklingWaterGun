#include "All.h"


CHRFRAMEINFO::CHRFRAMEINFO():
pre_pos(0,0,0),
frame_move(0,0,0)
{
	Reset();
}

void CHRFRAMEINFO::Reset()
{
	isHitWall = false;
	isHitFloor = false;
}

//初期化
Character::Character(
	ENTITY_ID					id,		//このキャラクタの一意な番号
	LPCHARACTER_BRAINBASE		m_pBrain,	//このキャラクタを動かす脳みそクラス
	LPCHARACTERMODELBASE		pCharacterModel,	//このキャラクタのモデルクラス
	LPCHARACTER_UI				pUserInterface,
	int							team_type,
	TeamData*					tData
	):
	ControllObject(id, team_type, tData),
m_pCharacterModel(pCharacterModel),
m_pBrain(m_pBrain),
m_pStateTable(nullptr),
m_State_type(chr_move_state::error_mode),
m_pUserInterface(pUserInterface),
m_Eye(0.09f)
{
	ChrFunc::InitCharacterParam(&m_Param);
	m_Param.m_LifePoint = GetConstChrParam(this)->max_hp; 

	m_pLockOnClass = new CharacterLockOn(this);

	m_pStateTable = new CharacterMoveStateTable(this);
	m_pStateTable->SetState(chr_move_state::usual_mode);

	m_AimTarget.SetOwner(this);

	WeaponParam p;
	ColaGun::InitWeponParam(p);

	m_pWepon = new ColaGun(p, this);
	((ColaGun*)m_pWepon)->SetColawaterFactory(ColaGun::UsualColaFactory);


	m_Param.m_view_vec = Vector3(1, 0, 0);
	m_render_Interface = new R_InterFace_Character();

	LPIEX2DOBJ	 pEyeTextures[3];
	GAMEMATCHINFO.GetTextureMgr().GetEyeTextures((int)id, pEyeTextures);

	for (int i = 0; i < 3; ++i)
	{
		m_Eye.SetEyeTexture((CharacterEye::EYE_STATE)i, pEyeTextures[i]);
	}
	
}

Character::~Character()
{
	SAFE_DELETE(m_render_Interface);
	LPMapObjectInterface pRideObj;

	//現在親子関係を結んでいるかチェック
	if (IS_MAPOBJ_ID(m_Param.m_RideObjectId) &&
		(pRideObj = (LPMapObjectInterface)IDMANAGER.GetPointer(m_Param.m_RideObjectId)))
	{
		//乗っていたオブジェクトから自信を切り離す
		pRideObj->ReleaseChild(this);
	}
	//解放
	SAFE_DELETE(m_pStateTable);
	SAFE_DELETE(m_pBrain);
	SAFE_DELETE(m_pCharacterModel);
	SAFE_DELETE(m_pUserInterface);
	SAFE_DELETE(m_pWepon);
	SAFE_DELETE(m_pLockOnClass);
}

//乗っているものが動いたときに呼ばれる関数
void Character::ParentMove(
	LPCMATRIX pParentWorldMat,
	LPCMATRIX pParentWorldMatInv,
	LPCMATRIX pParentMoveMat,
	LPCMATRIX AffineMat)
{
	//移動後の座標
	Vector3 new_position = Vector3MulMatrix(m_base_param.pos, *AffineMat);
	
	//親によって動かされるベクトルを更新
	m_Param.m_parent_move = new_position - m_base_param.pos;

	//移動
	m_base_param.pos = new_position;

	//視点のYだけは変わらないほうがいい？
	float view_y = m_Param.m_view_vec.y;

	MATRIX	rotate_mat = *pParentMoveMat;
	//視点ベクトルは回転のみ影響を受けるので
	rotate_mat._41 = rotate_mat._42 = rotate_mat._43 = 0;
	m_Param.m_view_vec = Vector3MulMatrix(m_Param.m_view_vec, rotate_mat);

	m_Param.m_view_vec.y = view_y;
	m_Param.m_view_vec.Normalize();
}

void Character::PowerUp(MSG_TYPE type, float value)
{
	LPCHARACTERCONSTPARAMETERS pParam = GetConstChrParam(this);

	switch (type)
	{
	case msg_t::up_character_charge_speed:
		m_Param.UpValue.ChargeSpeed += value;
		m_Param.UpValue.ChargeSpeed = min(m_Param.UpValue.ChargeSpeed, pParam->character_charge_speed_up_max_value);
		break;
	case msg_t::up_character_move_speed:
		m_Param.UpValue.MoveSpeed += value;
		m_Param.UpValue.MoveSpeed = min(m_Param.UpValue.MoveSpeed, pParam->character_move_speed_up_max_value);
		break;
	default: 
		MyAssert(false, "Character::PowerUpに対応していないタイプが送られました type=%d", (int)type);
		break;
	}
}

CRCHRFRAMEINFO Character::GetFrameInfo()const
{
	return m_frame_info;
}

Vector3 Character::GetBodyCenter()
{
	return m_base_param.pos + Vector3(0, 0.7f, 0);
}

void Character::Controll()
{
	//このキャラクタを動かす命令を出す脳のUpdate関数を呼ぶ
	SAFE_EXE(m_pBrain, Update, this);
}

void Character::Update(TeamData& teamData)
{
	//カメラ更新
	m_camera.Update();

	//フレーム単位の情報の更新
	m_frame_info.Reset();

	//行動ステートによる移動
	if (m_pStateTable)
	{
		FUNCTIONSPEEDCHECK(m_pStateTable->Execute());
		m_State_type = m_pStateTable->GetState();
	}
	CharacterUtilityFunctions::HitCheckOtherControllObj(this);


	SAFE_EXE(m_pUserInterface, Update);
	SAFE_EXE(m_pWepon, Update);
	SAFE_EXE(m_pWepon,SetPower,1-cosf(m_pUserInterface->GetColaGaugePower()*PI));
	SAFE_EXE(m_pLockOnClass, Update);

	CharacterMoveFunctions::UpdateAimTargetInfo(this, &teamData.m_Viewdata);

	m_frame_info.frame_move = m_base_param.pos - m_frame_info.pre_pos;
	m_frame_info.pre_pos = m_base_param.pos;

	ChrFunc::CreateTransMatrix(m_Param.m_trans_mat, m_Param.m_view_vec, m_base_param.pos);
	ChrFunc::SetOutLineColorHP(this);
	ChrFunc::UpdateUpValueAlpha(this);
	ChrFunc::UpdateDamageLifePoint(this);

	m_Eye.Update();

	m_Param.m_Count += 0.1f;
	if (m_Param.m_Count > 2000)m_Param.m_Count = 0;

	if (m_base_param.pos.y < -10)
	{
		m_pStateTable->SetState(chr_move_state::die_mode);
	}

#ifdef _DEBUG
	MyDebugMgrStr(color_smp::YELLOW,"～キャラクタID %d の情報～", (int)GetId());
	MyDebugMgrStr(color_smp::YELLOW,"life = %4.2f", m_Param.m_LifePoint);
	MyDebugMgrStr(color_smp::YELLOW,"state_type= %d ", m_pStateTable->GetState());
	MyDebugMgrStr(color_smp::YELLOW,"pos x=%3.3f y=%3.3f z=%3.3f", m_base_param.pos.x, m_base_param.pos.y, m_base_param.pos.z);
	MyDebugMgrStr(color_smp::YELLOW,"mov x=%3.3f y=%3.3f z=%3.3f", m_Param.m_move.x, m_Param.m_move.y, m_Param.m_move.z);
	MyDebugMgrStr(color_smp::YELLOW,"vec x=%3.3f y=%3.3f z=%3.3f", m_Param.m_view_vec.x, m_Param.m_view_vec.y, m_Param.m_view_vec.z);
	MyDebugMgrStr(color_smp::YELLOW,"angle=%f", ChrFunc::GetAngle(this));
	MyDebugMgrStr(color_smp::YELLOW,"ride_obj=%d", m_Param.m_RideObjectId);
	MyDebugMgrStr(color_smp::YELLOW,"");
#endif
	//TestFunction();

}

void Character::Render(ControllObject& viewowner, TeamData& teamdata)
{
	CharacterRenderParam rp(
		viewowner,
		teamdata,
		*this
		);

	m_pStateTable->UpdateTransMatrix();

	m_render_Interface->Render(&rp);
	//m_pCharacterModel->Render(this);

	ChrFunc::RenderEye(this);
	m_pWepon->Render(m_pMyMatchData->team_type,m_pLockOnClass->isShuffle());
	m_pStateTable->Render();

#if 0

	DWORD color[] =
	{
		color_smp::WHITE,
		color_smp::WHITE,
		color_smp::WHITE,
		color_smp::WHITE
	};
		debug_particle::NumberRender(m_base_param.pos + Vector3(0, 1.3f, 0), 0.2f, m_pMyMatchData->player_num, color[m_pMyMatchData->team_type]);
#endif
}
//
//void Character::Render(char* shader_Name)	
////描画
//{
//	m_pCharacterModel->Render(this, shader_Name);
//	m_pWepon->Render();
//}

void Character::RenderUI(ControllObject& viewowner, TeamData& teamdata)
{
	SAFE_EXE(m_pUserInterface, Render, viewowner, teamdata);
}

bool Character::HandleMessage(LPGameMessage msg)
{
	switch (msg->type)
	{
	case msg_t::to_chr_state://行動ステートに対するメッセージ
	case msg_t::chr_table_state_change:	//テーブルに対するメッセージ
	case msg_t::chr_table_state_change_low_lvl:
		if (m_pStateTable)return m_pStateTable->HandleMessage(msg);
		break;
	case msg_t::hit_my_attack:
		m_pUserInterface->SetReaction(1.2f);
		break;
	case msg_t::up_character_move_speed:
		ChrFunc::SetPowerUpEffect(this, msg->type);
		PowerUp(msg->type, *(float*)msg->exinfo);
		break;
	case msg_t::up_character_charge_speed:
		ChrFunc::SetPowerUpEffect(this, msg_t::up_character_charge_speed);
		PowerUp(msg->type, *(float*)msg->exinfo);
		break;
	case msg_t::chr_friend_msg_icon:
		ChrFunc::SendFriendIconMsgToBrain(this, *(FriendMessageIcon::MsgType*)msg->exinfo);
		ChrMove::SetFriendMessegeIcon(this, *(FriendMessageIcon::MsgType*)msg->exinfo);
		break;
	default:
		if (m_pBrain)return m_pBrain->HandleMessage(msg);
		break;
	}
	return false;
}

void	Character::TestFunction()
{
	Vector3 target_Pos;
	m_pUserInterface->GetTargetPos(target_Pos);
	Vector3 my_pos = GetBodyCenter();
	Vector3 vec = target_Pos - my_pos;

	float xz_dist = sqrtf(vec.x * vec.x + vec.z* vec.z);
	float move_power = ChrFunc::GetNowChargeRange(this) / 120.0f;
	float time = xz_dist / move_power;

	vec.Normalize();
	vec *= move_power;

	float gravity = 0.001f;
	float y_pos = vec.y * time + (0.5f * -gravity *time* time);

	Vector3 work = target_Pos;
	work.y = y_pos;
	float rate = time / 30.0f;

	for (float t = 0; t < time; t += rate)
	{
		Vector3 work2;
		work2.x = my_pos.x + vec.x * t;
		work2.z = my_pos.z + vec.z * t;
		work2.y = my_pos.y + vec.y * t + (0.5f * -gravity * t*t);
		debug_particle::point(work2, Vector3(0, 0, 0), 0.1f, dword_color::blue, 10);
	}

	debug_particle::point(work, Vector3(0, 0, 0), 0.5f, dword_color::green);
	MyDebugMgrStr(dword_color::yellow, "着地高さ : %3.3f", y_pos);
	MyDebugMgrStr(dword_color::yellow, "targetpos : X%3.3f,Y%3.3f Z%3.3f", target_Pos.x, target_Pos.y,
		target_Pos.z);

}

void	Character::Render_ShadowMap(ControllObject& viewowner, TeamData& teamdata)
{
	CharacterRenderParam rp(
		viewowner,
		teamdata,
		*this
		);

	m_render_Interface->Render_ShadowMap(&rp);
}