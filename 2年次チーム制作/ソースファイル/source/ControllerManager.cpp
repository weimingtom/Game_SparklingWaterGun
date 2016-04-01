#include "All.h"

ControllerManager::ControllerManager():
GameBaseEntity(game_id::id_controller_manager)
{

	for (int i = 0; i < CODBO_GAME_PLAYER_MAX; i++)
	{
		m_pController_array[i] = nullptr;
	}
}


ControllerManager::~ControllerManager()
{
	for (int i = 0; i < CODBO_GAME_PLAYER_MAX; i++)
	{
		SAFE_DELETE(m_pController_array[i]);
	}
}

bool ControllerManager::SwapControllObject(
	ControllObject* pre_obj,
	ControllObject* next_obj)
{
	MyAssert(pre_obj != nullptr, "nullのオブジェクトが探索されました");

	for (int i = 0; i < CODBO_GAME_PLAYER_MAX; i++)
	{
		if (m_pController_array[i] &&
			m_pController_array[i]->GetControllObjectPtr() == pre_obj)
		{
			m_pController_array[i]->SetControllObj(next_obj);
			return true;
		}
	}

	MyAssert(false, "コントロールオブジェクトの乗り換えに失敗しました");

	return false;
}

void ControllerManager::AddController(LPCONTROLLER controller)
{
	for (int i = 0; i < CODBO_GAME_PLAYER_MAX; i++)
	{
		if (!m_pController_array[i])
		{
			m_pController_array[i] = controller;
			return;
		}
	}
	MyAssert(false, "ControllerManager::AddController コントローラが最大数に達しました");
}

void ControllerManager::Update()
{
	for (int i = 0; i < CODBO_GAME_PLAYER_MAX; i++)
	{
		SAFE_EXE(m_pController_array[i], Controll);
	}
}


bool ControllerManager::HandleMessage(LPGameMessage msg)
{
	return false;
}