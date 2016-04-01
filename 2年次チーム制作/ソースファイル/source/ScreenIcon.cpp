#include "ScreenIcon.h"

ScreenIconManager::ScreenIconManager():
GameBaseEntity(game_id::id_screen_icon_manager)
{

}

ScreenIconManager::~ScreenIconManager()
{
	for (
		IconList::iterator it = m_IconList.begin();
		it != m_IconList.end();
		++it)
	{
		delete *it;
	}
}

void ScreenIconManager::AddIcon(LPICONBASE icon)
{
	m_IconList.push_back(icon);
}

void ScreenIconManager::Update()
{
	IconList::iterator it = m_IconList.begin();

	while (it != m_IconList.end())
	{
		if (!(*it)->Update())
		{
			delete *it;
			it=m_IconList.erase(it);
			continue;
		}
		++it;
	}
}

void ScreenIconManager::Render(
	LPCONTROLLOBJCT		pViewOwner,
	LPTEAMDATA			pTeamData)
{
	for (
		IconList::iterator it = m_IconList.begin();
		it != m_IconList.end();
	++it)
	{
		(*it)->Render(pViewOwner, pTeamData);
	}
}

bool ScreenIconManager::HandleMessage(LPGameMessage msg)
{
	switch (msg->type)
	{
	case msg_t::send_screen_icon_msg:
		AddIcon((LPICONBASE)msg->exinfo);
		return true;

	case msg_t::icon_render_msg:
		Render(((RenderOwnerMsgExinfo*)msg->exinfo)->pViewOwner, ((RenderOwnerMsgExinfo*)msg->exinfo)->pTeamData);
		return true;

	default:break;

	}

	return false;
}