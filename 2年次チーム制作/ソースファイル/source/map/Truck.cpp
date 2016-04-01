#include "../All.h"
#include "Truck.h"

#include "ShareRatioObject.h"

#include "WayPointObject.h"

//----------------------------------------
// 角度コントロール
//----------------------------------------
void angleContRoll(CRVector3 MyPos, CRVector3 ViewPos, float *angle, float MaxSpped)
{
	if (MyPos == ViewPos)return;

	Vector3 v1(sinf(*angle), 0, cosf(*angle));
	Vector3 v2(ViewPos - MyPos);

	v2.Normalize();

	float dot = Vector3Dot(v1, v2);

	dot = 1 - dot;
	if (dot >MaxSpped)
		dot = MaxSpped;

	float crossY = v1.z*v2.x - v1.x*v2.z;

	if (crossY>0)
		*angle += dot;

	else *angle -= dot;
}

//*************************************************************
// トラッククラス
//*************************************************************

Truck::Truck(
	ENTITY_ID               id,
	ObjectParam            param,
	LPIEXMESH              mesh,
	MAP_OBJ_TYPE       type
	) :
	ShareRatioObjectBase(id, param, mesh, type),
	target_pos_(param.pos),
	speed_(0.1f),
	damage_object_((DamageObjectManager*)IDMANAGER.GetPointer(game_id::id_damage_manager)),
	is_step_brakes_(false)
{
	// 最初の座標設定
	int random_way_point_num =
		rand() % ((int)map_obj::_way_point_obj_end - (int)map_obj::_way_point_obj_begin);
	random_way_point_num += (int)map_obj::_way_point_obj_begin;

	MapObjectManager::MAPOBJ_ARRAY way_points;
	MAP_MANAGER.Get_TypeObject(way_points, (MAP_OBJ_TYPE)(random_way_point_num));
	assert((way_points.empty() == false) && "WayPoint取得に失敗しました。");
	
	WayPointObject* first_target_point = (WayPointObject*)way_points.at(0);
	parameters_.pos = first_target_point->Get_Param().pos;
	// ターゲット設定
	target_point_ = first_target_point->GetNextWayPoint();
	target_pos_ = target_point_->Get_Param().pos;

	//移動量設定
	if (target_point_ != nullptr)
	{
		move_ = target_pos_ - parameters_.pos;
		move_.Normalize();
		move_ *= speed_;

		for (int i = 0; i < 4; ++i)
		{
			angleContRoll(parameters_.pos, target_pos_, &parameters_.angle.y, 1.0f);
		}
	}

	//ダメージ判定作成
	Vector3 front = Vector3(sinf(parameters_.angle.y), 0, cosf(parameters_.angle.y));
	Vector3 damage_position = parameters_.pos + front;
	float	damage_size = 1.5f;
	float damage_value = 500.0f;
	DAMAGEFLAGS damage_type =
		damage_type_bits::all_character_damage |
		damage_type_bits::apper_crushed_mesh;

	damage_object_.SetPos(damage_position);
	damage_object_.SetSize(damage_size);
	damage_object_.param.val = damage_value;
	damage_object_.param.type = damage_type;
	damage_object_.param.parent = id;
	damage_object_.param.vec = move_;

	//ダメージ有効化
	damage_object_.Apper();

	collision_size_ = 2.5f;
}


Truck::~Truck()
{

}


void Truck::Update()
{
	is_step_brakes_ = false;

	// 前方チェック
	CheckForward();

	// ターゲット更新
	UpdateTarget();
	UpdateAngle();

	// 速度更新
	UpdateSpeed();

	Vector3 front_vec(0, 0, 0);
	front_vec.x = sinf(parameters_.angle.y);
	front_vec.z = cosf(parameters_.angle.y);

	move_ = front_vec*speed_;
	parameters_.pos += move_;

	// 遅かったらダメージなし
	if (speed_ < 0.05f)
	{
		damage_object_.param.val = 0.0f;
	}
	else
	{
		damage_object_.param.val = 500;
	}

	damage_object_.SetPos(parameters_.pos + front_vec);
	damage_object_.param.vec = move_;

	ShareRatioObjectBase::Update();

	damage_pos_ = parameters_.pos;
	damage_pos_.y += 1;
}

void Truck::Hit(DamageObject* p)
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

void Truck::ChangeTeam(DamageObject* p)
{
	POLYGONEFFECT.SetEffect(
		parameters_.pos + Vector3(sinf(parameters_.angle.y), 0, cosf(parameters_.angle.y)),
		VECTOR3ZERO,
		VECTOR3ZERO,
		2,
		EffectTex::_STAR
		);

	SetChangeTeamEffect(false, false);
	SetChangeShareIcon(parameters_.pos + Vector3(0, 1, 0), p->param.parent);
}

bool Truck::RayPick(
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


bool Truck::Is_FriendForward()
{
	struct
	{
		TEAM_TYPE team_type;

		bool operator()(LPCONTROLLOBJCT obj)
		{
			return (ControllObjFunc::GetTeamType(obj) == team_type);
		}
	}pred;

	pred.team_type = this->m_ShareRatio.team_type;

	CONTROLLOBJLIST object = CONTROLLOBJMGR.GetObjectList(pred);

	Vector3 v, front(sinf(parameters_.angle.y), 0, cosf(parameters_.angle.y));
	front *= 7.0f;
	front += parameters_.pos;

	for (auto& it : object)
	{
		v = front;
		v -= ControllObjFunc::GetPos(it);

		if (v.Length() < 7.0f)
		{
			return true;
		}
	}
	return false;
}


bool Truck::Is_TruckForward()
{
	MapObjectManager::MAPOBJ_ARRAY object;

	MAP_MANAGER.Get_TypeObject(object, map_obj::_truck);

	Vector3 v, front(sinf(parameters_.angle.y), 0, cosf(parameters_.angle.y));
	front *= 7.0f;
	front += parameters_.pos;

	for (auto& it : object)
	{
		if (it->GetId() <= this->GetId())
		{
			continue;
		}

		v = front;
		v -= it->Get_Param().pos;

		if (v.Length() < 7.0f)
		{
			return true;
		}
	}
	return false;
}


void Truck::CheckForward()
{
	if (Is_FriendForward() == true)
	{
		is_step_brakes_ = true;
	}
	else if (Is_TruckForward() == true)
	{
		is_step_brakes_ = true;
	}
}


// 速度更新
void Truck::UpdateSpeed()
{
	if (is_step_brakes_ == true)
	{
		speed_ *= 0.98f;
	}
	else
	{
		speed_ += (0.15f - speed_)*0.05f;
	}
}


void Truck::UpdateTarget()
{
	target_point_->UsePoint();

	//target_posに着いたら
	float distance = (target_pos_ - parameters_.pos).Length();

	if (distance < 8.0f)
	{
		WayPointObject* next_target = 
			target_point_->GetNextWayPoint();

		if (next_target == target_point_)
		{
			// 減速
			is_step_brakes_ = true;
		}

		if (target_point_->Get_Type() == map_obj::_way_point_obj_end)
		{
			parameters_.pos = next_target->Get_Param().pos;
		}

		target_point_->UnUsePoint();
		target_point_ = next_target;

		target_pos_ = target_point_->Get_Param().pos;
	}
}


void Truck::UpdateAngle()
{
	const float rotate_speed_max = 0.02f;
	float rotate_speed = (rotate_speed_max / 0.15f * speed_);

	if (is_step_brakes_ == false)
	{
		angleContRoll(parameters_.pos, target_pos_, &parameters_.angle.y, 0.02f);
	}
}


void	Truck::Render()
{
	// テクスチャの変更は後で
	// とりあえず描画しておく
#if 1
	mesh_->SetTexure(
		0,
		GAMEMATCHINFO.GetTextureMgr().GetTruckTexturePtr(m_ShareRatio.team_type)->GetTexture()
		);


	const RATIO alpha = ShareRatioObjectBase::GetSeeThroughAlpha()*0.6f;

	shader->SetValue("OutlineSize", 0.05f);

	mesh_->TransMatrix = SRT_matrix_;

#if LIGHTMAP
	mesh_->Render(shader,"truck");
#else
	mesh_->Render(shader, "mainstage_realtime_shadow");
#endif

	mesh_->SetTexure(
		0,
		nullptr
		);
#else
	ShareRatioObjectBase::Render();
#endif
}


void	Truck::Render_ShadowMap()
{
	if (mesh_)
	{
		mesh_->TransMatrix = SRT_matrix_;
		mesh_->Render(shader, "ShadowBuf");
	}
}


//*************************************************************
// トラック作成クラス
//*************************************************************

bool TruckFactory::CreateNewObject(const ObjectData &data)
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

	LPIEXMESH truck_mesh = 
		mesh_manager->GetMeshPtr((MeshManager::MESH_ID)(data.mesh_type));
	
	// 作成
	Truck* truck =
		new Truck(
		id,
		parameter,
		truck_mesh,
		map_obj::_truck
		);
	++count;

	// 登録
	MAP_MANAGER.AddObject(truck);

	return true;
}

// EOF