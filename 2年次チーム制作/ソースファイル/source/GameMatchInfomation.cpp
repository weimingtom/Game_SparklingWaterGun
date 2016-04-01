#include "GameMatchInfomation.h"

GameMatchInfomation* GameMatchInfomation::pInstance = nullptr;

GameMatchInfomation::GameMatchInfomation():
past_time(0),
mesh_mgr(100),
m_GameStart(false)
{
//	ZeroMemory(&match_data, sizeof(CODMatchData));
	screen_size.x = screen_size.y = 0;
	screen_size.width = 0;
	screen_size.height = 0;
}

GameMatchInfomation::~GameMatchInfomation()
{

}

GameMatchInfomation& GameMatchInfomation::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new GameMatchInfomation;
	}
	return *pInstance;
}

void GameMatchInfomation::ReleaseInstance()
{
	SAFE_DELETE(pInstance);
}

void GameMatchInfomation::MatchDataSet(
	const CODMatchData& data)
{
	int w, h;
	MyAssert(Get_Screen_Division(data.player_num, w, h),"プレイヤー人数が最大数を超えています");
	
	match_data = data;
	screen_size.x = screen_size.y = 0;
	screen_size.width = (int)iexSystem::ScreenWidth / w;
	screen_size.height = (int)iexSystem::ScreenHeight / h;

	texture_mgr.LoadTexture();
}

void GameMatchInfomation::MatchStart()
{
	past_time = 1;
}

void GameMatchInfomation::MatchFinish()
{
	past_time = 0;
}

void GameMatchInfomation::MatchUpdate()
{
	if (past_time > 0 && past_time < ULONG_MAX)++past_time;
}

void GameMatchInfomation::GameStart()
{
	m_GameStart = true;
}
bool GameMatchInfomation::isGameStart()const
{
	return m_GameStart;
}

const CODMatchData& GameMatchInfomation::GetMatchData()const
{
	return match_data;
}

const DWORD& GameMatchInfomation::GetMatchTime()const
{
	return past_time;
}

const RECTI& GameMatchInfomation::GetScreenSize()const
{
	return screen_size;
}
MeshManager&		GameMatchInfomation::GetMeshMgr()
{
	return mesh_mgr;
}
TextureManager&		GameMatchInfomation::GetTextureMgr()
{
	return texture_mgr;
}



bool 	GameMatchInfomation::Get_Screen_Division(const int& num_Player,
	int& num_Width, int& num_Height)
{
	if (num_Player >= 9 || num_Player < 1)
	{
		return false;
	}
	typedef struct tagScreenD
	{
		int width;
		int height;
	}ScreenD;

	static ScreenD ret[10] =
	{
		{ -1, -1 },
		{ 1, 1 },//1
		{ 1, 2 },//2
		{ 2, 2 },//3
		{ 2, 2 },//4
		{ 3, 2 },//5
		{ 3, 2 },//6
		{ 3, 3 },//7
		{ 3, 3 },//8
		{ 3, 3 },//9
	};
	num_Width = ret[num_Player].width;
	num_Height = ret[num_Player].height;
	return true;
}
