#ifndef __COD_BO_H__
#define __COD_BO_H__

#include "ScreenSeparation.h"
#include "CODBOForwardDecl.h"
#include "GameSystem\GameController.h"
#include <list>

//********************************************************
//	ColaOfDissension_BloodyVsOcean　
//		を更新するクラス(SceneMainみたいなもの)のヘッダー
//********************************************************


//********************************************************
//	ゲーム上の定数たち
//********************************************************

#define CODBO_GAME_PLAYER_MAX 10		//ゲームをプレイする者(COM含めて)の最大数
#define CODBO_CONTROLL_OBJECT_MAX (35)	//ゲーム中の操作できるものの最大数
#define CODBO_MAP_OBJECT_MAX 100

//********************************************************
//	ゲームタイプ
//********************************************************
namespace game_match_type
{
	enum _ENUM_GAME_MATCH_TYPE
	{
		FLAG_GAME,	//旗とり合戦
		
	};
}

//↑のゲームタイプのtypedef（できれば↑ではなくこちらを使ってほしい）
typedef game_match_type::_ENUM_GAME_MATCH_TYPE		GAME_MATCH_TYPE;

//********************************************************
//	ゲーム用キャラクターパラメーター
//********************************************************

namespace player_type
{
	enum _ENUM_PLAYER_TYPE
	{
		PLAYER,		//プレイヤー(コントローラ操作)
		COMPUTER,	//コンピュータ
		SPECTATOR,	//観戦者
	};
}

typedef player_type::_ENUM_PLAYER_TYPE PLAYER_TYPE;


typedef struct PLAYERMATCHDATA
{
	PLAYER_TYPE				player_type;	//プレイヤータイプ
	int						player_num;		//プレイヤー番号
	TEAM_TYPE				team_type;		//所属しているチーム番号
	ControllerButtonSet		button_set;		//ボタン配置情報

} *LPPLAYERMATCHDATA;

typedef std::list<PLAYERMATCHDATA> PLAYER_MATCHDATA_LIST;

void InitPlayerMatchData(LPPLAYERMATCHDATA pOut);

//********************************************************
//	マッチタイプ別ゲームステート
//********************************************************
class MatchTypeStateBase
{
public:
	virtual ~MatchTypeStateBase(){}
public:
	virtual void GameStart() = 0;			//ゲーム開始時に呼ばれる関数
	virtual bool isGameFinish() = 0;		//ゲームが終わっているか(終わっていたらリザルトに移行)
	virtual void GetResultScene(Scene**ppOut) = 0;
public:
	virtual void Initialize() = 0;			//初期化
	virtual void Update() = 0;				//毎フレームの更新
	virtual void GameRender(LPIEXVIEW, ControllObject*) = 0;	//引数のカメラでゲームオブジェクトを描画
	virtual void Render() = 0;				//描画関数
};

typedef MatchTypeStateBase*		LPMatchTypeStateBase;


//********************************************************
//ゲームを始めるのに必要なデータ
//********************************************************
struct CODMatchData
{
	GAME_MATCH_TYPE			match_type;		//マッチタイプ
	LPMatchTypeStateBase	pMatch_state;	//マッチタイプステート
	PLAYER_MATCHDATA_LIST	player_data;	//プレイヤ情報
	int						player_num;		//プレイヤー数
	int						computer_num;	//コンピューター数
	int						all_player_num;	//キャラクタ総数(プレイヤー数とコンピューター数の合計)
};

//********************************************************
//	ColaOfDissension_BloodyVsOcean　を更新するクラス
//********************************************************
class ColaOfDessensionManager
{
private:
	CODMatchData						m_MatchData;		//マッチデータ
	LPMatchTypeStateBase				m_pState;			//マッチステート(↑のpMatch_stateを取り出しただけ)
public:
	//ゲームを構築するデータ構造体をもとにゲームを構築
	ColaOfDessensionManager(LPCODMatchData pMatchData);
	//マッチステートはdeleteします
	~ColaOfDessensionManager();
public:
	static COLOR GetTeamColor(int team);
	static COLOR GetTeamColaWaterColor(int team);
public:
	bool isResult(Scene**ppOut);
	void Initialize();
	void Update();
	void Render();
};

typedef ColaOfDessensionManager CODMgr;

#endif