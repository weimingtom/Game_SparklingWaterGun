#include "All.h"

void InitPlayerMatchData(LPPLAYERMATCHDATA pOut)
{
	pOut->player_type = player_type::PLAYER;
	pOut->team_type = 0;
}


ColaOfDessensionManager::ColaOfDessensionManager(
	LPCODMatchData pMatchData):
m_MatchData(*pMatchData),
m_pState(nullptr)
{
	
}

ColaOfDessensionManager::~ColaOfDessensionManager()
{
	SAFE_DELETE(m_pState);
	GAMEMATCHINFO.ReleaseInstance();
}

COLOR ColaOfDessensionManager::GetTeamColor(int team)
{
	static COLOR colors[]=
	{
		//0xFF55ffdd,
		0xFF1995ff,
		0xFFfcff19,
		0xFF4fff19,
		0xFFc51e1e,
		

	};
	return colors[team];
}

COLOR ColaOfDessensionManager::GetTeamColaWaterColor(int team)
{
	//static COLOR colors[] =
	//{
	//	0xFFc51e1e,
	//	0xFFc51e1e,
	//	0xFFFFFFFF,
	//	//0xFF3f3fc3
	//};
	static COLOR colors[] =
	{
		0xFF55ffdd,
		0xFFfcff19,
		0xFF4fff19,
		0xFFc51e1e,
		

		//0xFF3f3fc3
	};

	return colors[team];
}

bool ColaOfDessensionManager::isResult(Scene**ppOut)
{
	if (m_pState&&m_pState->isGameFinish())
	{
		m_pState->GetResultScene(ppOut);
		return true;
	}
	return false;
}

void ColaOfDessensionManager::Initialize()
{
	MyAssert(
		m_MatchData.pMatch_state,
		"マッチステートがNULLの状態でゲームが開始されました");

	GAMEMATCHINFO.MatchDataSet(m_MatchData);
	GAMEMATCHINFO.MatchStart();

	//マッチステート初期化
	m_pState = m_MatchData.pMatch_state;
	SAFE_EXE(m_pState, Initialize);
}

void ColaOfDessensionManager::Update()
{
	(SAFE_EXE(m_pState, Update));
	GAMEMATCHINFO.MatchUpdate();
}

void ColaOfDessensionManager::Render()
{
	(SAFE_EXE(m_pState, Render));
	//SAFE_EXE(m_pState, Render);
}