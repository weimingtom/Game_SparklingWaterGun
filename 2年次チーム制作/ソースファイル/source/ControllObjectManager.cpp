#include "All.h"

#define IDtoINDEX(id)((int)(id)-(game_id::id_controll_obj_first))

ControllObjectManager* ControllObjectManager::m_pInstance = 0;

ControllObjectManager::ControllObjectManager()
{
	for (int i = 0; i < CODBO_CONTROLL_OBJECT_MAX; ++i)
	{
		m_pControllObject_array[i] = nullptr;
		m_isDelete[i] = false;
	}
}


ControllObjectManager::~ControllObjectManager()
{
	for (int i = 0; i < CODBO_CONTROLL_OBJECT_MAX; i++)
	{
		delete (m_pControllObject_array[i]);
	}
}


void ControllObjectManager::InstanceCreate()
{
	if (!m_pInstance)
	{
		m_pInstance = new ControllObjectManager;
	}
}


ControllObjectManager&	ControllObjectManager::InstanceGet()
{
	return *m_pInstance;
}


void ControllObjectManager::InstanceRelase()
{
	SAFE_DELETE(m_pInstance);
}

void ControllObjectManager::AddObject(LPCONTROLLOBJCT addchr)
{
	int id = IDtoINDEX(addchr->GetId());
	MyAssert(!m_pControllObject_array[id],
		"CharacterManager::AddCharacter関数でキャラクタの二重登録が行われました。\n ID = %d \n キャラ番号 = %d", addchr->GetId(), id);

	m_pControllObject_array[id] = addchr;
}

LPCONTROLLOBJCT	 ControllObjectManager::GetObjectPtr(ENTITY_ID id)
{
	int n = IDtoINDEX(id);
	return m_pControllObject_array[n];
}

bool ControllObjectManager::DeleteObject(ENTITY_ID id)				
//指定したIDのオブジェクトをdeleteし,そのindexを再利用可能にする
{
	int n = IDtoINDEX(id);

	if (!m_pControllObject_array[n])return false;

	m_isDelete[n] = true;

	return true;
}

void ControllObjectManager::Update(TeamDataManager& teamData)
{

	for (int i = 0; i < CODBO_CONTROLL_OBJECT_MAX; ++i)
	{
		if (m_isDelete[i])
		{
			m_isDelete[i] = false;
			delete m_pControllObject_array[i];
			m_pControllObject_array[i] = nullptr;
			continue;
		}
		SAFE_EXE(
			m_pControllObject_array[i],
			Update,
			*ControllObjFunc::GetTeamData(m_pControllObject_array[i])
			);
	}
}

void ControllObjectManager::Render(ControllObject& viewowner, TeamData& teamdata)
{
	for (int i = 0; i < CODBO_CONTROLL_OBJECT_MAX; ++i)
	{
		if (m_pControllObject_array[i]&& !m_isDelete[i])
			m_pControllObject_array[i]->Render(viewowner, teamdata);
	}
}


void	ControllObjectManager::Render_ShadowMap(ControllObject& viewowner, TeamData& teamdata)
{
	for (int i = 0; i < CODBO_CONTROLL_OBJECT_MAX; ++i)
	{
		if (m_pControllObject_array[i])
		{
			m_pControllObject_array[i]->Render_ShadowMap(viewowner, teamdata);
		}
	}
}

