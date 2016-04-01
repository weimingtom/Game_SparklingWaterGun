#include "All.h"
#include "ColorUtility.h"

#define FRAND ((float)rand()/(float)RAND_MAX)
const float AimPoint::max_scale = 80.0f;

AimTarget::AimTarget():
myId ( (AIMTARGET_ID)((AimTargetManager*)IDMANAGER.GetPointer(game_id::id_aim_target_manager))->RegistObject(this))
{
	
}

AimTarget::~AimTarget()
{
	((AimTargetManager*)IDMANAGER.GetPointer(game_id::id_aim_target_manager))->EraceObject(myId);
}

AimPoint::AimPoint(
	LPIEX2DOBJ	texture,
	float		movearea_size,
	float		default_size,
	float		scale_speed,
	float		move_speed
	):
	m_pTexture(texture),
	m_position2d(0,0),
	m_scale(default_size),
	m_movearea_size(movearea_size),
	m_speed(move_speed),
	m_color(0xFFFFFFFF),
	m_default_size(default_size),
	m_next_weight(1),
	m_scale_speed(scale_speed),
	m_position(0,0,0),
	m_change_target(true),
	m_hit_reticle(0)
{
	for (int i = 0; i < 2; i++)
	{
		VECTOR3TOZERO(m_point[i].cli_pos);
		m_point[i].scr_pos.x = m_point[i].scr_pos.y = 0;
		m_point[i].color = color_smp::WHITE;
		m_point[i].id = ERR_AIMTARGET_ID;
	}
	m_point[0].size = m_point[1].size = GetConstChrParam(nullptr)->default_sight_size;
}


AimPoint::~AimPoint()
{

}


void AimPoint::GetGoPosition(
	AimTargetInfo &out,
	CRAimOwnerInfo owner)
{
	//移動目標の情報を得る
	AimTargetManager* manager = (AimTargetManager*)IDMANAGER.GetPointer(game_id::id_aim_target_manager);


	if (m_change_target)
	{
		manager->GetTarget(out, owner);
	}
	else{
		AimOwnerInfo tmp = owner;
		tmp.circle_size = 0.25f;
		manager->GetTarget(out, tmp);
	}
}


Vector2	 AimPoint::GetInCircleRandPos(RATE seed)
//サークル内の場所をランダムに返す
{
	static const float RAND_MAX_M2 = (float)RAND_MAX / 2.0f;
	Vector2 rv((float)rand(), (float)rand());

	rv.x -= RAND_MAX_M2;
	rv.y -= RAND_MAX_M2;

	rv = Vector2Normalize(rv);

	return m_position2d + rv*(m_scale / 2);
}


CRVector2	AimPoint::GetPosition()
//サークル中心の画面座標を得る
{
	return m_position2d;
}


void	AimPoint::SetChangeTarget(bool changeFlg)
{
	m_change_target = changeFlg;
}

void AimPoint::GetReactionTargetPos(CRVector3 arm_pos, Vector3 &out)
{
	const float max = max_scale - GetConstChrParam(nullptr)->default_sight_size;
	float v = (m_scale - GetConstChrParam(nullptr)->default_sight_size) / max;	
	v = max(0.2f, v);//0.3~1

	const Vector3 target = m_point[1].cli_pos*m_next_weight + m_point[0].cli_pos * (1 - m_next_weight);
	Vector3 tt(target - arm_pos), rv(FRAND, FRAND, FRAND);
	rv.Normalize();

	const float angle = (FRAND - 0.5f)*v*(PI / 18);
	if (angle>0.01f)
		tt = Vector3RotateAxis(rv, angle, tt);
	
	out = arm_pos + tt;
}

void	AimPoint::SetReaction(float power)
//サークルを大きくする
{
	m_hit_reticle = power;
}

//反動を更新
void AimPoint::ReactionUpdate(float target)
{
	m_hit_reticle -= 0.05f;
	m_hit_reticle = max(m_hit_reticle, 0);
}

void	AimPoint::Update(CRAimOwnerInfo owner)
//更新
{
	//目標の情報を取得
	AimTargetInfo target_info;
	GetGoPosition(target_info, owner);

	if (m_point[1].id != target_info.id)
	{
		m_next_weight = 0;
		m_point[0].color = m_color;
		m_point[0].id = m_point[1].id;
		m_point[0].cli_pos = m_position;
		m_point[0].scr_pos = m_position2d;
	}
	else{
		m_next_weight += m_speed;
		m_next_weight = min(m_next_weight, 1);
	}
	
	m_point[1] = target_info;

	float weight = sinf(PI_d2*m_next_weight);

	m_position = m_point[0].cli_pos * (1 - weight) + m_point[1].cli_pos *weight;
	m_position2d = m_point[0].scr_pos * (1 - weight) + m_point[1].scr_pos *weight;
	m_color = ColorLerp(m_point[0].color, m_point[1].color, weight);

	ReactionUpdate(m_default_size);
}


void	AimPoint::Render()							
//描画
{
#if 1
	const RECTI& rect=GAMEMATCHINFO.GetScreenSize();
	const float r = m_scale / 2.0f,
		w = (float)rect.width,
		h = (float)rect.height;
#else
	const float r = m_scale / 2.0f,
		w = (float)(iexSystem::ScreenWidth),
		h = (float)(iexSystem::ScreenHeight);
#endif

	//COLORf col(m_color);

	//col.a = col.a*0.5f + min(m_hit_reticle, 1)*0.5f;

	m_pTexture->Render(
		(int)(((m_position2d.x + 1)*0.5f)*w - r),
		(int)((1 - (m_position2d.y + 1)*0.5f)*h - r),
		(int)(m_scale),
		(int)(m_scale),
		0,
		0,
		512,
		512,
		RS_COPY,
		//col.toDWORD()
		m_color
		);

#if 1
	const float hit_reticle_size = m_scale*1.5f;

	m_pTexture->Render(
		(int)(((m_position2d.x + 1)*0.5f)*w - (hit_reticle_size/2)),
		(int)((1 - (m_position2d.y + 1)*0.5f)*h - (hit_reticle_size/2)),
		(int)(hit_reticle_size),
		(int)(hit_reticle_size),
		0,
		0,
		512,
		512,
		RS_COPY,
		m_color & 0x00FFFFFF | (DWORD(255 * min(m_hit_reticle, 1)) << 24)
		);

#else


#endif

	if (m_point[1].id == ERR_AIMTARGET_ID)return;

	//ロックオン始動時の円

	const float r2size = ((1 - m_next_weight)*50) + r;

	m_pTexture->Render(
		(int)(((m_position2d.x + 1)*0.5f)*w - r2size),
		(int)((1 - (m_position2d.y + 1)*0.5f)*h - r2size),
		(int)(r2size*2),
		(int)(r2size*2),
		0,
		0,
		512,
		512,
		RS_COPY,
		(m_color & 0x00FFFFFF) | (0x50000000)
		);
}

AimTargetManager::AimTargetManager():
GameBaseEntity(game_id::id_aim_target_manager)
{

}


AimTargetManager::~AimTargetManager()
{
	
}

void AimTargetManager::GetTarget(
	AimTargetInfo &out,
	CRAimOwnerInfo info)
{
	bool found = false;
	AimTargetInfo temp;
	out.weight = 0;

	for (int i = 0; i < AIM_TARGET_MAX; ++i)
	{

		if (m_pObject_data[i]&&
			m_pObject_data[i]->isTarget(temp, info))
		{
			found = true;
			if (out.weight < temp.weight ||
				(out.size < temp.size && FLOATEQU(out.weight, temp.weight)))
			{
				out = temp;
				out.id = (AIMTARGET_ID)i;
			}
		}
	}

	if (!found)
	{
		Vector3 farp;
		info.pCamera->ScreenToClient(farp, Vector3(0, 0, 1));
		info.pCamera->ScreenToClient(out.cli_pos, Vector3(0, 0, 0));
		out.cli_pos += Vector3Normalize(farp - out.cli_pos)*20.0f;
		out.color = color_smp::WHITE;
		out.scr_pos.x = out.scr_pos.y = 0;
		VECTOR3TOZERO(out.cli_move);
		out.team_type = -1;
		out.weight = 0;
		out.size = GetConstChrParam(nullptr)->default_sight_size;
		out.id = ERR_AIMTARGET_ID;
	}
}

bool AimTargetManager::HandleMessage(LPGameMessage msg)
{
	
	return false;
}

