#ifndef __SCREEN_ICON_H__
#define __SCREEN_ICON_H__

#include "iextreme.h"
#include "CODBOForwardDecl.h"
#include <list>
#include "GameSystem\GameIDManager.h"

class ScreenIconBase
{
public:
	virtual~ScreenIconBase(){}
	virtual bool Update() = 0;
	virtual void Render(
		LPCONTROLLOBJCT		pViewOwner,
		LPTEAMDATA			pTeamData
		) = 0;
};

class ScreenIconManager:public GameBaseEntity
{
private:
	typedef ScreenIconBase* LPICONBASE;
	typedef std::list<LPICONBASE> IconList;
public:

private:
	IconList m_IconList;
public:
	ScreenIconManager();
	~ScreenIconManager();
private:
	void AddIcon(LPICONBASE icon);
public:
	void Update();
	void Render(
		LPCONTROLLOBJCT		pViewOwner,
		LPTEAMDATA			pTeamData);

	bool HandleMessage(LPGameMessage msg);
};

#endif