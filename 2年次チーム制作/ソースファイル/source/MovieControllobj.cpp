#include "MovieControllobj.h"
#include "CharacterUtilityFunction.h"
#include "ControllObjectFunction.h"
#include "ControllObjectManager.h"
#include "ControllerManager.h"
#include "GameSystem\GameIDManager.h"
#include "CharacterMoveState.h"

MovieControllobj::MovieControllobj(
	ENTITY_ID				myId,
	int						team_type,
	TeamData*				tData) :
ControllObject(myId, team_type, tData)
{
	m_base_param.pos = Vector3(0, 0, 0);
	m_camera.SetNewState(new FreeMoveCamera());
}


MovieControllobj::~MovieControllobj()
{

}

void MovieControllobj::Controll()
{

}

void MovieControllobj::Update(TeamData& teamdata)
{
	m_camera.Update();
}

void MovieControllobj::RenderUI(ControllObject& viewowner, TeamData& teamdata)
{

}


bool MovieControllobj::HandleMessage(LPGameMessage msg)
{
	return false;
}