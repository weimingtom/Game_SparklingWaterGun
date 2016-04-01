#include "All.h"

DamageObject::DamageObject(DamageObjectManager*	pMgr) :
pMgr(pMgr),
pos(0,0,0),
size(1)
{
	sp_obj = new sp_object(this);
	param.type = 0;
	param.val = 0;
	param.hit_count = 0;
	param.parent = game_id::id_error;
	param.color = 0xFFFFFFFF;
	VECTOR3TOZERO(param.pos);
	VECTOR3TOZERO(param.vec);
}


DamageObject::~DamageObject()
{
	delete sp_obj;
}


void DamageObject::Apper()	//ダメージ判定を有効にする
{
	sp_obj->EntryToField(pMgr->GetField(), &pos);
}

void DamageObject::Exit()//ダメージ判定を無効にする
{
	sp_obj->ExitCell();
}

bool DamageObject::isEnable()const
{
	return sp_obj->isEntry();
}

CRVector3 DamageObject::GetPos()const
{
	return pos;
}

float DamageObject::GetSize()const
{
	return size;
}

void DamageObject::SetPos(CRVector3 pos)
{
	this->pos = pos;
	sp_obj->PositionUpdate(&pos);
}

void DamageObject::SetSize(float s)
{
	size = s;
}


DamageObjectManager::DamageObjectManager():
GameBaseEntity(game_id::id_damage_manager)
{
	m_pField = new sp_field(&Vector3(0, 0, 0), MAP_MANAGER.Get_FieldSize(), 8);
}

DamageObjectManager::~DamageObjectManager()
{
	delete m_pField;
}

lp_sp_field	DamageObjectManager::GetField()const
{
	return m_pField;
}

void DamageObjectManager::RenderDebugParticle()
{
	DAMAGEVECTOR v = GetInDamage(VECTOR3ZERO, 100);
	PARTICLE p;
	p.aColor = 0xFFFF0000;
	p.aFrame = 0;
	p.angle = 0;
	p.eColor = 0xFFFF0000;
	p.eFrame = 2;
	p.flag = RS_COPY;
	p.mColor = 0xFFFF0000;
	p.mFrame = 1;
	p.Move = VECTOR3ZERO;
	p.Power = VECTOR3ZERO;
	p.rotate = 0;
	p.stretch = 1;
	p.type = 0;

	for (DAMAGEVECTOR::iterator it = v.begin();
		it != v.end();
		++it)
	{
		p.Pos = (*it)->GetPos();
		p.scale = (*it)->GetSize();

		iexParticle::Set(
			&p
			);
	}
}

DAMAGEVECTOR DamageObjectManager::GetInDamage(CRVector3 pos, float size)
{
	std::vector<void*>obj_array = m_pField->SearchObject(&pos, size);
	int sizen = (int)obj_array.size();

	if (sizen <= 0)return DAMAGEVECTOR();

	DAMAGEVECTOR ret((UINT)sizen);

	memcpy(&ret.at(0), &obj_array.at(0), sizeof(void*)*sizen);

	return ret;
}


bool DamageObjectManager::HandleMessage(LPGameMessage msg)
{

	return false;
}


bool damage_check::isCureDamage(
	LPCONTROLLOBJCT me,
	const DamageParam& param)
{
	if (param.type&damage_type_bits::all_character_cure)return true;
	if ((!(param.type&damage_type_bits::team_cure_damage))||
		((param.type&damage_type_bits::except_myself)&&param.parent==me->GetId()))return false;
	
	LPCONTROLLOBJCT	pEnemy = (LPCONTROLLOBJCT)IDMANAGER.GetPointer(param.parent);

	return (pEnemy&&
		ControllObjFunc::GetMatchData(pEnemy)->team_type == ControllObjFunc::GetMatchData(me)->team_type);
}

bool damage_check::isEnemyDamage(LPCONTROLLOBJCT me, const DamageParam& param)
{
	if (param.type&damage_type_bits::all_character_damage)return true;
	if ((!(param.type&damage_type_bits::enemy_damage))||
		((param.type&damage_type_bits::except_myself)&&param.parent == me->GetId()))return false;

	LPCONTROLLOBJCT	pEnemy = (LPCONTROLLOBJCT)IDMANAGER.GetPointer(param.parent);

	return (!pEnemy||
		ControllObjFunc::GetMatchData(pEnemy)->team_type != ControllObjFunc::GetMatchData(me)->team_type);

}