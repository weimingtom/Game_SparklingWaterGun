#include "../All.h"


GameIDManager* GameIDManager::m_pInstance = 0;

//IDデータ初期化
GameIDManager::GameIDManager():
m_id_count(game_id::id_controll_obj_first)
{
	for (int i = 0; i < GAME_ID_MAX; ++i)
	{
		m_Entity_array[i] = nullptr;
	}

}

GameIDManager::~GameIDManager()
{

}

GameIDManager&	GameIDManager::GetInstance()
{
	if (m_pInstance == 0)
		m_pInstance = new GameIDManager;
	return *m_pInstance;
}

void GameIDManager::Release()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = 0;
	}
}

bool GameIDManager::GetEmptyControllObjID(ENTITY_ID* pOut)
{
	for (int i = game_id::id_controll_obj_first; i < game_id::id_controll_obj_max; ++i)
	{
		if (m_Entity_array[m_id_count])
		{
			if (++m_id_count >= game_id::id_controll_obj_max)
				m_id_count = game_id::id_controll_obj_first;
			continue;
		}
		*pOut = (ENTITY_ID)m_id_count;

		if (++m_id_count >= game_id::id_controll_obj_max)
			m_id_count = game_id::id_controll_obj_first;
		return true;
	}
	return false;
}

LPGameBaseEntity GameIDManager::GetPointer(ENTITY_ID id)
{
	assert(id >= 0 && id < GAME_ID_MAX&&"存在しないID");
	return m_Entity_array[id];
}

bool GameIDManager::Register(LPGameBaseEntity regist_entity_ptr)
{
	ENTITY_ID id = regist_entity_ptr->GetId();

	assert(id >= 0 && id < GAME_ID_MAX&&"存在しないID");

	if (m_Entity_array[id])
		return false;

	MyDebugString("ID番号　%d が登録されました\n", id);

	m_Entity_array[id] = regist_entity_ptr;

	return true;
}

bool GameIDManager::Erace(ENTITY_ID id)
{
	assert(id >= 0 && id < GAME_ID_MAX&&"存在しないID");

	if (m_Entity_array[id] == 0)return false;
	m_Entity_array[id] = 0;
	MyDebugString("ID番号　%d が削除されました\n", id);
	return true;
}

bool GameIDManager::Dispatch(LPGameMessage msg_ptr)
{
	assert(msg_ptr->to >= 0 && msg_ptr->to < GAME_ID_MAX&&"存在しないID");

	if (!m_Entity_array[msg_ptr->to])return false;
	return m_Entity_array[msg_ptr->to]->HandleMessage(msg_ptr);
}