
#ifndef __GAME_ID_MANAGER_H__
#define __GAME_ID_MANAGER_H__

#include "GameBase.h"
#include "../CODBOForwardDecl.h"

//***************************************************************
//	ID登録管理クラス
//***************************************************************

class GameIDManager
{
private:
	static GameIDManager*	m_pInstance;
	int						m_id_count;
	LPGameBaseEntity		m_Entity_array[GAME_ID_MAX];	
public:
	GameIDManager();
	~GameIDManager();
public:
	static GameIDManager&	GetInstance();
	static void				Release();
public:
	bool GetEmptyControllObjID(ENTITY_ID* pOut);
public:
	LPGameBaseEntity GetPointer(ENTITY_ID id);
	bool Register(LPGameBaseEntity regist_entity_ptr);	//登録ぅ！！
	bool Erace(ENTITY_ID id);							//解除ぉ！！
	bool Dispatch(LPGameMessage msg_ptr);				//メッセージをおくるぅ！！
};

struct RenderOwnerMsgExinfo
{
	LPCONTROLLOBJCT		pViewOwner;
	LPTEAMDATA			pTeamData;
};

#define IDMANAGER (GameIDManager::GetInstance())		//人生を楽にするために…

#endif