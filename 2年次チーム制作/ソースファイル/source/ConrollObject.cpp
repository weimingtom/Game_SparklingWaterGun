#include "All.h"

ControllObject::ControllObject(ENTITY_ID id, int team_type, TeamData* tData) :
GameBaseEntity(id),
m_pMyMatchData(nullptr),
m_pTeamData(tData)
{

	ControllObjFunc::InitControllObjectParam(&m_base_param);
	m_base_param.team_type = team_type;
}
