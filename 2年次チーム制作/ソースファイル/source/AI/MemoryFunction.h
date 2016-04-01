#ifndef __MEMORYFUNCTION_H__
#define __MEMORYFUNCTION_H__

class Character;
class NPC_Brain;
class Character_Record;
class MEMORY;
class MapObject_Record;

class MemoryFunction
{
public:
	template<class pred>
	static int	Get_CharacterRecordList(MEMORY& memory, pred& p,
		std::vector<Character_Record*>* ret_Container)
	{
		int ret = 0;
		for (auto it = memory.m_character_Record.begin(); it != memory.m_character_Record.end(); it++)
		{
			if (p(it->second))
			{
				ret_Container->push_back(it->second);
				ret++;
			}
		}
		return ret;
	}
	template<class pred>
	static int	Get_MapObjectRecordList(MEMORY& memory, pred& p,
		std::vector<MapObject_Record*>* ret_Container)
	{
		int ret = 0;
		for (auto it = memory.m_mapobject_Record.begin(); it != memory.m_mapobject_Record.end(); it++)
		{
			if (p(it->second))
			{
				ret_Container->push_back(it->second);
				ret++;
			}
		}
		return ret;
	}

public :	//PlanningMemory(NPC_Censor��Planning���ׂ��Ɣ��f����ENTITY_ID���L�^����)
	static int		Is_Register_PlanningMemory(NPC_Brain* access_Brain, const censor_type& type, const ENTITY_ID& target_ID);
	static bool	Register_PlanningMemory(NPC_Brain* access_Brain, const censor_type& type, const ENTITY_ID& target_ID);
	static bool	Remove_PlannningMemory(NPC_Brain* access_Brain, const ENTITY_ID& remove_id);
	static bool	Character_IsDie(MEMORY* access_memory, const ENTITY_ID& die_character);
};

namespace memory_predicate
{
	/**
	*@brief GoalAttackMemory�̌��
	*/
	bool	Candidate_AttackTarget(Character_Record* record);
	/**
	*@brief �L���I�u�W�F�N�g�̋L�����E���Ă���
	*/
	bool	Candidate_AdvertisementPoint(MapObject_Record* record);
	/**
	*@brief GoalMedicalFriend�̌��
	*/
	bool	Candidate_MedicalFriend(Character_Record* record);
}

#endif
