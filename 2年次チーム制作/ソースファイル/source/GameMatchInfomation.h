#ifndef __GAME_MATCH_INFOMATION_H__
#define __GAME_MATCH_INFOMATION_H__

#include "CODBO.h"
#include "iextreme.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "NewsTelop.h"

//********************************************************************//
//		マッチ情報管理クラス
//********************************************************************//

struct ViewInformation
{
	float									m_zNear;
	float									m_zFar;
	float									m_Fov;
	inline void	 Set(const float& znear, const float& zfar, const float& fov)
	{
		m_Fov = fov; m_zNear = znear; m_zFar = zfar;
	}

};

class GameMatchInfomation
{
public:
	enum GAME_MESH_ID
	{
		_COLA_WATER,
		_COLA_GUN,
		_PARACHUTE,
		_EMPTY_CAN,
	};
private:
	static GameMatchInfomation*		pInstance;
	DWORD							past_time;		//マッチの開始
	CODMatchData					match_data;		//マッチデータ
	RECTI							screen_size;	//スクリーンサイズ
	MeshManager						mesh_mgr;
	TextureManager					texture_mgr;
	ViewInformation					m_view_Info;
	bool							m_GameStart;
private:
	GameMatchInfomation();
	~GameMatchInfomation();
public:
	static GameMatchInfomation& GetInstance();
	static void ReleaseInstance();
public:
	void MatchDataSet(const CODMatchData& data);
public:
	void MatchStart();
	void MatchFinish();
	void MatchUpdate();
public:
	void GameStart();
	bool isGameStart()const;
	const CODMatchData& GetMatchData()const;
	const DWORD&		GetMatchTime()const;
	const RECTI&		GetScreenSize()const;
	MeshManager&		GetMeshMgr();
	TextureManager&		GetTextureMgr();

	bool Get_Screen_Division(
		const int& num_Player,
		int& num_Width,
		int& num_Height);

	bool GetNowGameScrDivision(int& num_Width, int& num_Height)
	{
		return Get_Screen_Division(match_data.player_num, num_Width, num_Height);
	}

	inline void	 Set_ViewInfo(const float& znear, const float& zfar, const float& fov)
	{
		m_view_Info.Set(znear, zfar, fov);
	}
	const ViewInformation& Get_ViewInfo(){ return m_view_Info; }

};

#define GAMEMATCHINFO	(GameMatchInfomation::GetInstance())

#endif