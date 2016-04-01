#ifndef __CONTROLLER_MANAGER_H__
#define __CONTROLLER_MANAGER_H__

#include "CODBO.h"
#include "Controller.h"

class ControllObject;
typedef std::list<LPCONTROLLER> CONTROLLERLIST;

class ControllerManager:public GameBaseEntity
{
private:
	LPCONTROLLER m_pController_array[CODBO_GAME_PLAYER_MAX];
public:
	ControllerManager();
	~ControllerManager();
public:
	template <class pred_type>
	CONTROLLERLIST GetController(pred_type& pred);
	bool SwapControllObject(ControllObject* pre_obj, ControllObject* next_obj);
	void AddController(LPCONTROLLER controller);
	void Update();
	bool HandleMessage(LPGameMessage msg);
};


template <class pred_type>
CONTROLLERLIST ControllerManager::GetController(pred_type& pred)
{
	CONTROLLERLIST ret;
	for (int i = 0; i < CODBO_GAME_PLAYER_MAX; i++)
	{
		if (m_pController_array[i]&&
			pred(m_pController_array[i]))ret.push_back(m_pController_array[i]);
	}
	return ret;
}

#endif