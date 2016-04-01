#ifndef __FRIND_MESSAGE_H__
#define __FRIND_MESSAGE_H__

#include "ScreenIcon.h"

class FriendMessageIcon :public ScreenIconBase
{
public:
	enum MsgType
	{
		_ERROR =-1,
		_HELP,
		_GO,
		_COME_ON,
		_OK
	};
private:
	ENTITY_ID			m_Parent;
	LPIEX2DOBJ			m_pTexture;
	TextureRect			m_TextureRect;
	Vector3				m_Pos;
	Vector2				m_Size;
	TEAM_TYPE			m_TeamType;
	int					m_LiveTime;
	float				m_SizeCount;
	const float			m_MaxSizeCount;
public:
	FriendMessageIcon(
		ENTITY_ID				Parent,
		LPIEX2DOBJ				pTexture,
		const TextureRect &		texRect,
		CRVector3				pos,
		CRVector2				size
		);
	~FriendMessageIcon();
private:
	void Setpos(CRVector3 pos);
public:
	void SetTeamType(TEAM_TYPE	TeamType);
	bool Update();
	void Render(
		LPCONTROLLOBJCT		pViewOwner,
		LPTEAMDATA			pTeamData
		);
};

//GameMessageのExInfo用構造体

struct FriendIconMsgInfo
{
	FriendMessageIcon::MsgType	type;	//表示アイコンタイプ
	ENTITY_ID					sender;	//送り主のポインタ
	ENTITY_ID					to;
};

#endif