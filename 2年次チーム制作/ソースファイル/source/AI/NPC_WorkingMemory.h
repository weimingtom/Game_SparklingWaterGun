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
	NPC_Brain*		m_holder;				//���̋L���̕ێ��҂ւ̃|�C���^
private :
	/**
	*@brief �Â�����L���A���񂾓G�̋L���̍폜���s��
	*/
	void	Update_Character_Record();
	/**
	*@brief�@�|�X�^�[�ȂǌŒ�z�u�I�u�W�F�N�g�ɑ΂��鏉���L������
	*/
	void	Initialize_MapObject();
public :
	MEMORY	memory;	//���\����

	WorkingMemory(){}
	~WorkingMemory();
	void Initialize(NPC_Brain* brain);
	/**
	*@brief �n���h�����b�Z�[�W
	*/
	bool HandleMessage(AI_MESSAGE* msg);
	
	/**
	*@brief�@�L���X�V
	*@note	 
	*/
	bool Update(const int& current_Time, Character* object);
};

#endif
