#include "TriggerManager.h"

#include "iextreme.h"

TriggerManager::TriggerManager():
m_Trigger_count(0)
{
	ZeroMemory(m_pTrigger_array, sizeof(TriggerBase*)*TRIGGER_MAX);
}

TriggerManager::~TriggerManager()
{
	for (int i = 0; i < TRIGGER_MAX; i++)
	{
		SAFE_DELETE(m_pTrigger_array[i]);
	}
}

void TriggerManager::AddTrigger(TriggerBase* trigger)
{
	for (int i = 0; i < TRIGGER_MAX; i++)
	{
		++m_Trigger_count %= TRIGGER_MAX;

		if (m_pTrigger_array[m_Trigger_count])continue;

		m_pTrigger_array[m_Trigger_count] = trigger;
		return;
	}
	MyAssert(false, "TriggerManager‚É’Ç‰Á‚Å‚«‚éTrigger‚ªÅ‘å”‚É’B‚µ‚Ü‚µ‚½");
	delete trigger;
}

void TriggerManager::Update()
{
	for (int i = 0; i < TRIGGER_MAX; i++)
	{
		if (m_pTrigger_array[i] &&
			m_pTrigger_array[i]->Update())
		{
			delete m_pTrigger_array[i];
			m_pTrigger_array[i] = nullptr;
		}
	}
}