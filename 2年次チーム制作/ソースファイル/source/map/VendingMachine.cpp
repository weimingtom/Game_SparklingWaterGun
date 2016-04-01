#include "../All.h"
#include "VendingMachine.h"

#include "ShareRatioObject.h"


VendingMachine::VendingMachine(
	ENTITY_ID               id,
	ObjectParam            param,
	LPIEXMESH              mesh,
	MAP_OBJ_TYPE       type
	) :
	ShareRatioObjectBase(id, param, mesh, type)
{
	m_ShareRatio.team_type = (int)type - (int)map_obj::_resporn_point_begin;
	damage_pos_.y += 0.5f;
	m_pShareObjIcon->SetTeamType(m_ShareRatio.team_type);

	m_ShareRatio.share_value = m_ShareRatio.max_share_value = 50;
	collision_size_ = 2;
}


VendingMachine::~VendingMachine()
{

}


void VendingMachine::Update()
{
	ShareRatioObjectBase::Update();
}


void VendingMachine::Render()
{
	// テクスチャの変更は後で
	// とりあえず描画しておく
#if 1
	if (m_ShareRatio.team_type >= 0)
	{
		mesh_->SetTexure(
			0,
			GAMEMATCHINFO.GetTextureMgr().GetVendingTexturePtr(m_ShareRatio.team_type)->GetTexture()
			);
	}

	const RATIO alpha = ShareRatioObjectBase::GetSeeThroughAlpha()*0.6f;

	shader->SetValue("OutlineSize", 0.05f);

	mesh_->TransMatrix = SRT_matrix_;
	


#if LIGHTMAP
	mesh_->Render();
#else
	mesh_->Render(shader, "mainstage_realtime_shadow");
#endif

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
			GAMEMATCHINFO.GetTextureMgr().GetVendingTexturePtr(m_ShareRatio.team_type)->GetTexture()
			);
	}

	mesh_->Render(shader, "see_through", SRT_matrix_);

	mesh_->SetTexure(
		0,
		nullptr
		);
#else
	ShareRatioObjectBase::Render();
#endif
}


void VendingMachine::Render_ShadowMap()
{
	ShareRatioObjectBase::Render_ShadowMap();
}


bool VendingMachine::RayPick(
	Vector3* out,
	Vector3* pos,
	Vector3*vec,
	float* dist
	)
{
	if (mesh_ == nullptr)
	{
		return false;
	}

	bool result = (mesh_->RayPickPlus(out, pos, vec, dist, SRT_matrix_, SRT_matrix_inverse_) != -1);

	return result;
}


void VendingMachine::Hit(DamageObject* p)
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


void VendingMachine::ChangeTeam(DamageObject* p)
{
	POLYGONEFFECT.SetEffect(
		parameters_.pos + Vector3(sinf(parameters_.angle.y), 0, cosf(parameters_.angle.y)),
		VECTOR3ZERO,
		VECTOR3ZERO,
		2,
		EffectTex::_STAR
		);


	object_type_ = (MAP_OBJ_TYPE)((int)map_obj::_resporn_point_begin + (int)m_ShareRatio.team_type);

	SetChangeTeamEffect(false);
	SetChangeShareIcon(parameters_.pos + Vector3(0, 1, 0), p->param.parent);

}


//*************************************************************
// 自動販売機作成クラス
//*************************************************************

bool VendingMachineFactory::CreateNewObject(const ObjectData &data)
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

	LPIEXMESH vending_machine_mesh =
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)(data.mesh_type));

	// 作成
	VendingMachine* vending_machine =
		new VendingMachine(
		id,
		parameter,
		vending_machine_mesh,
		(MAP_OBJ_TYPE)data.type
		);
	++count;

	// 登録
	MAP_MANAGER.AddObject(vending_machine);

	return true;
}

// EOF