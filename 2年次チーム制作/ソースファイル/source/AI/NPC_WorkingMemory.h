#ifndef __WORKING_MEMORY_H__
#define __WORKING_MEMORY_H__

#include	<list>
#include	"iextreme.h"
#include	"GameSystem\\GameMessageType.h"
//#include	"SoundMessage.h"
#include	<map>
#include	<algorithm>

class NPC_Brain;
class GameMessage;class Character;
class MapObjectInterface;
class ControllObject;
struct AI_MESSAGE;

mapobjcet_type Convert_MapObject_MyEnum(const MAP_OBJ_TYPE& type);

inline char* Get_ExtActStr(expectation_act& tag)
{
	char* str[] =
	{
		"none",
		"i_dont_know",
		"get_publicity_point",
		"fire_weapon",
		"attack_me",
		"move_field",
		"dieing",
		"die"
	};
	int t = enum_cast(tag);
	return str[t];
}

class WorkingMemory
{
private :
	NPC_Brain*		m_holder;				//この記憶の保持者へのポインタ
private :
	/**
	*@brief 古すぎる記憶、死んだ敵の記憶の削除を行う
	*/
	void	Update_Character_Record();
	/**
	*@brief　ポスターなど固定配置オブジェクトに対する初期記憶処理
	*/
	void	Initialize_MapObject();
public :
	MEMORY	memory;	//情報構造体

	WorkingMemory(){}
	~WorkingMemory();
	void Initialize(NPC_Brain* brain);
	/**
	*@brief ハンドルメッセージ
	*/
	bool HandleMessage(AI_MESSAGE* msg);
	
	/**
	*@brief　記憶更新
	*@note	 
	*/
	bool Update(const int& current_Time, Character* object);
};

#endif
