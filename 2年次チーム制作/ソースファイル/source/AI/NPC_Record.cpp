#include	"..\All.h"


float Character_Record::Calculate_Confidence(const int& current_Frame, const float&ReliabilityTime)
{
	int time = current_Frame - m_last_SensedTime;
	return 1.0f- Min(1.0f,(float)((float)time / ReliabilityTime));
}

bool	Character_Record::Can_Use(const int& current_Frame, const int& ReliabilityTime, float* Reliability)
{
	*Reliability = Calculate_Confidence(current_Frame, (float)ReliabilityTime);
	return (*Reliability > .0f) ? true : false;
}

float MapObject_Record::Calculate_Confidence(const int& current_Frame, const float&ReliabilityTime)
{
	return 1.0f;
}

bool	MapObject_Record::Can_Use(const int& current_Frame, const int& ReliabilityTime, float* Reliability)
{
	*Reliability = Calculate_Confidence(current_Frame, (float)ReliabilityTime);
	return (*Reliability > .0f) ? true : false;
}

void	MEMORY::All_Clear(){ m_character_Record.clear(); m_mapobject_Record.clear(); }

void	MEMORY::All_Release()
{
	SAFE_DELETE(m_goal_Result);
	SAFE_DELETE(m_planning_Memory);
	for (std::map<ENTITY_ID, Character_Record*>::iterator it = m_character_Record.begin();
		it != m_character_Record.end();
		++it)
	{
		SAFE_DELETE(it->second);
	}
	for (auto it = m_mapobject_Record.begin();
		it != m_mapobject_Record.end();
		++it)
	{
		SAFE_DELETE(it->second);
	}
}

/**
*@brief		第一引数のキャラクターを記憶しているかどうか確認、
記憶していない場合は領域を確保する
*@param1	確認するキャラクター情報
*/
void	MEMORY::Is_Record_Character(const ENTITY_ID& check_object)
{
	auto it = m_character_Record.find(check_object);
	//ない場合は領域作成
	if (it == m_character_Record.end())
	{
		m_character_Record.insert(std::pair<ENTITY_ID, Character_Record*>(check_object, new Character_Record()));
		//idは最初に登録
		m_character_Record[check_object]->m_id = check_object;
	}
}

void	MEMORY::Is_Record_MapObject(const ENTITY_ID& check_object)
{
	auto it = m_mapobject_Record.find(check_object);
	if (it == m_mapobject_Record.end())
	{
		m_mapobject_Record.insert(std::pair<ENTITY_ID, MapObject_Record*>(check_object, new MapObject_Record()));
		//idは最初に登録
		m_mapobject_Record[check_object]->m_id = check_object;
	}
}

bool	MEMORY::Is_Record_Threat_OutSight(const ENTITY_ID& check_object)
{
	if (m_threat_Outsight.empty())
		return false;
	for (auto& it : m_threat_Outsight)
	{
		if (it.m_id == check_object)
			return true;
	}
	return false;
}

MapObject_Record* MEMORY::Get_MapObjectMemory(const ENTITY_ID& id, const int& reliability_Time, float*reliability)
{
	auto itaretor = m_mapobject_Record.find(id);
	if (itaretor == m_mapobject_Record.end())
		return nullptr;
	bool flg = itaretor->second->Can_Use((int)GAMEMATCHINFO.GetMatchTime(), reliability_Time, reliability);
	return itaretor->second;
}
Character_Record* MEMORY::Get_CharacterMemory(const ENTITY_ID& id, const int& reliability_Time, float*reliability)
{
	auto itaretor = m_character_Record.find(id);
	if (itaretor == m_character_Record.end())
		return nullptr;
	bool flg = itaretor->second->Can_Use(GAMEMATCHINFO.GetMatchTime(), reliability_Time, reliability);
	
	return itaretor->second ;
}

bool	MEMORY::Registar_Threat_OutSight(const ENTITY_ID& id, const Vector3& threar_Vec)
{
	m_threat_Outsight.emplace_back(id, threar_Vec);
	return true;
}

bool		MEMORY::Erase_Threar_OutSight(const ENTITY_ID& erase_ID)
{
	auto erase = m_threat_Outsight.begin();
	for (; erase != m_threat_Outsight.end(); erase++)
	{
		if (erase->m_id == erase_ID)
		{
			m_threat_Outsight.erase(erase);
			return true;
		}
	}
	return true;
}

Threat_OutSight*const MEMORY::Get_Threat_OutSight(const ENTITY_ID& id)
{
	Threat_OutSight*ret;
	for (auto& it : m_threat_Outsight)
	{
		if (it.m_id == id)
		{
			ret = &it;
			return ret;
		}
	}
	return nullptr;
}

void	MEMORY::Reset_UpdateFlg_CharacterRecord()
{
	for (auto& it : m_character_Record)
	{
		it.second->m_update = false;
	}
}

