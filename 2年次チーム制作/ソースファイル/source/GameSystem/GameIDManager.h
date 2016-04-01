
#ifndef __GAME_ID_MANAGER_H__
#define __GAME_ID_MANAGER_H__

#include "GameBase.h"
#include "../CODBOForwardDecl.h"

//***************************************************************
//	ID�o�^�Ǘ��N���X
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
	bool Register(LPGameBaseEntity regist_entity_ptr);	//�o�^���I�I
	bool Erace(ENTITY_ID id);							//�������I�I
	bool Dispatch(LPGameMessage msg_ptr);				//���b�Z�[�W�������那�I�I
};

struct RenderOwnerMsgExinfo
{
	LPCONTROLLOBJCT		pViewOwner;
	LPTEAMDATA			pTeamData;
};

#define IDMANAGER (GameIDManager::GetInstance())		//�l�����y�ɂ��邽�߂Ɂc

#endif