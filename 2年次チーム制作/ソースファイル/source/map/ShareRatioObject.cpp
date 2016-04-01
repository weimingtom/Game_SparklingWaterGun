#include "../GameSystem/GameIDManager.h"
#include "../DamageHit.h"
#include "../AimPoint.h"
#include "ShareRatioObjectAim.h"
#include "MapObject.h"
#include "ShareRatioObject.h"
#include "../ControllObjectFunction.h"
#include "../MeshManager.h"
#include "../EffectResource.h"
#include "../GameMatchInfomation.h"
#include "../CharacterMoveFunction.h"
#include "../WaterPool.h"
#include "../system/System.h"
#include "../CharacterUtilityFunction.h"
#include "../ShareIcon.h"
#include "../CODPlaySound.h"
#include "../GameSound/GameSound.h"
#include "../SourChargeEffect.h"
#include "../ScoreCalcManager.h"

ShareRatioObjectBase::ShareRatioObjectBase(
	ENTITY_ID               id,
	ObjectParam            param,
	LPIEXMESH              mesh,
	MAP_OBJ_TYPE       type
	) :
	MapObjectInterface(id, param, mesh, type),
	damage_pos_(param.pos)
{
	// deleteしなくていいの？
	m_pShareObjIcon =
		new ShareObjIcon(
		GAMEMATCHINFO.GetTextureMgr().GetTexturePtr(TextureManager::SHARE_OBJ_ICON),
		{ 0, 0, 256, 256 },
		parameters_.pos,
		Vector2(50, 50)
		);
	GameMessage msg;

	msg.to = game_id::id_screen_icon_manager;
	msg.from = game_id::id_error;
	msg.type = msg_t::send_screen_icon_msg;
	msg.exinfo = m_pShareObjIcon;

	MyAssert(IDMANAGER.Dispatch(&msg), "シェアオブジェクトアイコンの作成メッセージ送信に失敗しました");

	m_ShareRatio.share_power = 0.1f;
	m_ShareRatio.team_type = -1;
	m_ShareRatio.NotDamageCount = 0;
	m_ShareRatio.max_share_value = m_ShareRatio.share_value = 100;

	aim_target_.SetOwner(this);

	collision_size_ = 1.0f;

	m_pShareObjIcon->SetTeamType(m_ShareRatio.team_type);
}


ShareRatioObjectBase::~ShareRatioObjectBase()
{
	
}


RATIO ShareRatioObjectBase::GetSeeThroughAlpha()
{
	return sinf((float)GAMEMATCHINFO.GetMatchTime()*0.1f)*0.3f + 0.6f;
}

bool ShareRatioObjectBase::SetChangeSourEffect(CRVector3 pos, ENTITY_ID parentId)
{
	//スクリーン空間上での泡の飛び散りを出現させる
	const int ChangeParticle = 15;
	GameMessage msg;
	Vector3 ScrPos;
	Vector2 ScrPosV2, Size, Target, Move;
	LPCONTROLLOBJCT pObj;
	LPIEX2DOBJ pSourTexture = POLYGONEFFECT.GetTexture(EffectTex::_SOUR).m_pTexture;

	msg.to = game_id::id_screen_icon_manager;
	msg.from = game_id::id_error;
	msg.type = msg_t::send_screen_icon_msg;

	//描画主が存在しない場合は失敗
	if (!IS_CONTROLLOBJ_ID(parentId) ||
		(pObj = (LPCONTROLLOBJCT)IDMANAGER.GetPointer(parentId)) == nullptr)
	{
		return false;
	}
	
	//画面内ではなかった場合も失敗
	if (!ControllObjFunc::GetCamera(pObj)->ClientToScreen(ScrPos, pos))
	{
		return false;
	}

	//出現位置指定
	ScrPos.x = (ScrPos.x + 1)*0.5f*(float)iexSystem::ScreenWidth;
	ScrPos.y = (ScrPos.y *-0.5f + 0.5f)*(float)iexSystem::ScreenHeight;

	//移動目標指定
	const TEAM_TYPE type = ControllObjFunc::GetTeamType(pObj);

	if (((ScoreCalcManager*)IDMANAGER.GetPointer(game_id::id_score_calc_manager))->GetTeamGaugePos(
		type, Target) == false)
	{
		return false;
	}


	for (int i = 0; i < ChangeParticle; ++i)
	{

		ScrPosV2.x = ScrPos.x + fRAND(-10, 10);
		ScrPosV2.y = ScrPos.y + fRAND(-10, 10);

		Size.x = Size.y = fRAND(10, 20);

		Move.x = fRAND(-10, 10);
		Move.y = fRAND(-10, 10);

		msg.exinfo =
			new SourChargeEffect(
			pSourTexture,
			ScrPosV2,
			Size,
			Move,
			Target,
			0.5f,
			20,
			type,
			parentId
			);

		IDMANAGER.Dispatch(&msg);
	}

	return false;
}

void ShareRatioObjectBase::SetChangeShareIcon(CRVector3 pos, ENTITY_ID parentId)
{
	GameMessage msg;
	TextureRect texrc;


	texrc.left = 0;
	texrc.top = 0;
	texrc.width = 404;
	texrc.height = 64;

	ScreenIconBase* icon =
		new ShareIcon(
		GAMEMATCHINFO.GetTextureMgr().GetTexturePtr(TextureManager::SHARE_UP),
		texrc,
		parentId,
		pos,
		Vector3(0, 0.01f, 0),
		VECTOR3ZERO,
		Vector2(150, 70),
		60,
		false
		);

	msg.to = game_id::id_screen_icon_manager;
	msg.from = game_id::id_error;
	msg.type = msg_t::send_screen_icon_msg;
	msg.exinfo = icon;

	IDMANAGER.Dispatch(&msg);

	SetChangeSourEffect(pos,parentId);
}


void ShareRatioObjectBase::SetChangeTeamEffect(
	bool set_water_pool,
	bool set_smoke)
{
	const int explode_cola_particle_num = 20;
	const int explode_smoke_num = 5;
	Vector3 move,pos;
	const Vector3 glavity(0, -0.003f, 0);

	DWORD color = CODMgr::GetTeamColaWaterColor(m_ShareRatio.team_type);

	if (set_smoke)
	{
		for (int i = 0; i < explode_smoke_num; i++)
		{
			pos.x = parameters_.pos.x + fRAND(-0.5f, 0.5f);
			pos.y = parameters_.pos.y + fRAND(-0.5f, 0.5f);
			pos.z = parameters_.pos.z + fRAND(-0.5f, 0.5f);

			POLYGONEFFECT.SetEffect(
				pos,
				VECTOR3ZERO,
				VECTOR3ZERO,
				fRAND(2.2f, 3.5f),
				EffectTex::_SMOKE,
				color,
				RS_ADD
				);
		}
	}

	for (int i = 0; i < explode_cola_particle_num; i++)
	{
		move.x = fRAND(-0.1f, 0.1f);
		move.z = fRAND(-0.1f, 0.1f);

		move.y = fRAND(-0.1f, 0.55f);

		POLYGONEFFECT.SetEffect(
			parameters_.pos,
			move,
			glavity,
			fRAND(0, 0.15f),
			EffectTex::_SOUR,
			color,
			RS_COPY,
			40 + rand() % 10
			);
	}

	if (set_water_pool)
	{

		LPWATERPOOLMANAGER pool = (LPWATERPOOLMANAGER)IDMANAGER.GetPointer(game_id::id_water_pool_manager);
		Vector3 front(RT_matrix_._31, RT_matrix_._32, RT_matrix_._33);
		front.Normalize();

		LPPARENT parent = GetParent();

		const ENTITY_ID parentId = (parent) ? ((LPMapObjectInterface)parent)->GetId() : game_id::id_error;

		pool->SetPool(
			parameters_.pos,
			front,
			3.0f,
			0,
			color,
			parentId,
			2
			);
	}

	ChrMove::SetExplositonBlur(parameters_.pos, 1);

	m_pShareObjIcon->SetTeamType(m_ShareRatio.team_type);

	SOUNDPLAYER.PlaySE(
		sound_t::poster_update,
		SOUNDPLAYER.GetSoundVolume(parameters_.pos, sound_t::poster_update)
		);
}


void ShareRatioObjectBase::Update()
{
	HitCheck();
	MapObjectInterface::Update();
	UpdateShareValueCure();
	m_pShareObjIcon->SetPos(damage_pos_);
}


const ShareRatioParam& ShareRatioObjectBase::Get_ShareParam()const
{
	return m_ShareRatio;
}


DAMAGEFLAGS ShareRatioObjectBase::HitCheck()
{
	DamageObjectManager* damage = (DamageObjectManager*)IDMANAGER.GetPointer(game_id::id_damage_manager);
	if (!damage)return (DAMAGEFLAGS)0;

	DAMAGEVECTOR damage_array;
	damage_array = damage->GetInDamage(damage_pos_, 1.0f);

	int i = (int)damage_array.size();

	if (i <= 0)return (DAMAGEFLAGS)0;
	--i;

	DAMAGEFLAGS ret = 0x0;
	Vector3 temp;
	float l;

	for (DamageObject** first = &damage_array.at(0); i >= 0; --i)
	{
		VECTOR3POINTTOPOINT(l, first[i]->param.pos, damage_pos_, temp);

		if (l < (collision_size_ + first[i]->GetSize()))
		{
			//あたった時の処理書く
			if(CalcHitDamage(first[i]))
				Hit(first[i]);
		}
	}
	return ret;
}


void ShareRatioObjectBase::UpdateShareValueCure()
{
	if (++m_ShareRatio.NotDamageCount < 30)
	{
		return;
	}

	m_ShareRatio.NotDamageCount = 30;
	m_ShareRatio.share_value += (m_ShareRatio.max_share_value - m_ShareRatio.share_value)*0.05f;

	UpdateColor();
}


void ShareRatioObjectBase::UpdateColor()
{
	const DWORD color = (m_ShareRatio.team_type != -1) ? CODMgr::GetTeamColor(m_ShareRatio.team_type) : 0xFFFFFFFF;

	m_ShareRatio.render_param.NowColor = COLORf::Lerp(
		m_ShareRatio.render_param.DamageTeamColor,
		COLORf(color),
		m_ShareRatio.share_value / m_ShareRatio.max_share_value);
}


bool ShareRatioObjectBase::CalcHitDamage(DamageObject* p)
{
	LPCONTROLLOBJCT pOwner = nullptr;

	if (!IS_CONTROLLOBJ_ID(p->param.parent))return false;
	
	pOwner = (LPCONTROLLOBJCT)IDMANAGER.GetPointer(p->param.parent);

	if (!pOwner)return false;

	const int team_type = ControllObjFunc::GetMatchData(pOwner)->team_type;

	if (m_ShareRatio.team_type == team_type)
	{
		m_ShareRatio.share_value += p->param.val;
		if (m_ShareRatio.share_value >= m_ShareRatio.max_share_value)
		{
			return false;
		}
	}
	else
	{
		m_ShareRatio.NotDamageCount = 0;
		m_ShareRatio.share_value -= p->param.val;
	}

	p->param.hit_count++;

	m_ShareRatio.render_param.DamageTeamColor = COLORf(CODMgr::GetTeamColor(team_type));


	if (m_ShareRatio.share_value < 0.0f)
	{
		m_ShareRatio.share_value = m_ShareRatio.max_share_value;
		m_ShareRatio.team_type = team_type;

		if (rand() % 2 == 0)
		{
			ChrFunc::SendChargeSpeedUpMsg(p->param.parent);
		}
		else{
			ChrFunc::SendMoveSpeedUpMsg(p->param.parent);
		}

		ChangeTeam(p);
	}

	UpdateColor();

	return true;
}


//*************************************************************
// ポスタークラス
//*************************************************************

PosterShareRatioObject::PosterShareRatioObject(
	ENTITY_ID               id,
	ObjectParam            param,
	LPIEXMESH              mesh,
	MAP_OBJ_TYPE       type
	) :
	ShareRatioObjectBase(id, param, mesh, type)
{
	collision_size_ = 1.0f;
	m_ShareRatio.share_power = 0.1f;
	m_ShareRatio.share_value = m_ShareRatio.max_share_value = 50;
}

PosterShareRatioObject::~PosterShareRatioObject()
{

}


void PosterShareRatioObject::Hit(DamageObject* p)
{
	POLYGONEFFECT.SetEffect(
		p->param.pos,
		VECTOR3ZERO,
		VECTOR3ZERO,
		0.4f,
		EffectTex::_COLA_HIT,
		p->param.color,
		RS_COPY
		);

	POLYGONEFFECT.SetEffect(
		p->param.pos,
		VECTOR3ZERO,
		VECTOR3ZERO,
		1.2f,
		EffectTex::_SPLASH,
		p->param.color,
		RS_COPY
		);
}


void PosterShareRatioObject::ChangeTeam(DamageObject* p)
{
	POLYGONEFFECT.SetEffect(
		parameters_.pos + Vector3(sinf(parameters_.angle.y), 0, cosf(parameters_.angle.y)),
		VECTOR3ZERO,
		VECTOR3ZERO,
		2,
		EffectTex::_STAR
		);

	SetChangeTeamEffect();
	SetChangeShareIcon(parameters_.pos + Vector3(0, 1, 0), p->param.parent);
}


void PosterShareRatioObject::ParentMove(
	LPCMATRIX pParentWorldMat,
	LPCMATRIX pParentWorldMatInv,
	LPCMATRIX pParentMoveMat,
	LPCMATRIX AffineMat
	)
{
	Vector3 new_position = Vector3MulMatrix(parameters_.pos, *AffineMat);
	parameters_.pos = new_position;
}


void PosterShareRatioObject::Update()
{
	ShareRatioObjectBase::Update();
}


void PosterShareRatioObject::Render()
{
	if (m_ShareRatio.team_type >= 0)
	{
		mesh_->SetTexure(
			0,
			GAMEMATCHINFO.GetTextureMgr().GetADTexturePtr(m_ShareRatio.team_type)->GetTexture()
			);
	}

	const RATIO alpha = ShareRatioObjectBase::GetSeeThroughAlpha()*0.6f;
	
	shader->SetValue("OutlineSize", 0.05f);

	mesh_->TransMatrix = SRT_matrix_;
	mesh_->Render();


	mesh_->SetTexure(
		0,
		nullptr
		);

	CRVector3 p = damage_pos_;
	float camera_z = p.x*matView._13 + p.y*matView._23 + p.z*matView._33 + matView._43;

	if (camera_z < 0)return;

	const float render_begin = 25.0f;
	const float render_end = 30.0f;

	if (camera_z > render_end)
	{
		return;
	}

	if (camera_z < render_begin)
	{
		camera_z = 1;
	}
	else
	{
		camera_z -= render_begin;
		camera_z /= render_end - render_begin;

		if (camera_z>1)
		{
			return;
		}
		camera_z = 1 - camera_z;
	}

	

	shader->SetValue("outlinecolor_A", alpha*camera_z);

	shader->SetValue("outlinecolor_R", m_ShareRatio.render_param.NowColor.r);
	shader->SetValue("outlinecolor_G", m_ShareRatio.render_param.NowColor.g);
	shader->SetValue("outlinecolor_B", m_ShareRatio.render_param.NowColor.b);

	if (m_ShareRatio.team_type >= 0)
	{
		mesh_->SetTexure(
			0,
			GAMEMATCHINFO.GetTextureMgr().GetADTexturePtr(m_ShareRatio.team_type)->GetTexture()
			);
	}

	mesh_->Render(shader, "see_through", SRT_matrix_);

	
	mesh_->SetTexure(
		0,
		nullptr
		);
}


void PosterShareRatioObject::Render(char* shaderName)
{
	mesh_->Render(shader, shaderName, SRT_matrix_);
}


void PosterShareRatioObject::Render_ShadowMap()
{
	mesh_->Render(shader, "ShadowBuf", SRT_matrix_);
}



//*************************************************************
// ポスター作成クラス
//*************************************************************

bool PosterFactory::CreateNewObject(const ObjectData &data)
{
	MeshManager* mesh_manager;
	MAP_MANAGER.Get_MeshManager(&mesh_manager);

	// 作成に必要なものを用意する

	ObjectParam parameter;
	parameter.pos = data.pos;
	parameter.angle = data.angle;
	parameter.scale = data.scale;

	ENTITY_ID id =
		(ENTITY_ID)(game_id::id_map_object_min + count);

	LPIEXMESH poster_mesh =
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)data.mesh_type);

	// 作成
	PosterShareRatioObject* poster =
		new PosterShareRatioObject(
		id,
		parameter,
		poster_mesh,
		(MAP_OBJ_TYPE)(data.type)
		);
	count++;

	// 登録
	MAP_MANAGER.AddObject(poster);

	return true;
}

// EOF