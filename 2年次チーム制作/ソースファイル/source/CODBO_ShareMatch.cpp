#include "All.h"


///******************************

#define COMPUTER_RENDER FALSE
#define MOVIE_MODE FALSE

///******************************

//****************************************************//
//　初期化+
//****************************************************//

static int numPlayer;

ShareMatchState::ShareMatchState()
{
	m_State = _PRE_PLAY;
	m_TimeCount = 0; 
}

void ShareMatchState::Initialize()
{
	m_isGameFinish = false;

	debug_particle::g_pNumberTexture = new iex2DObj("DATA\\number.png");

#if COMPUTER_RENDER
	numPlayer = GAMEMATCHINFO.GetMatchData().player_num +GAMEMATCHINFO.GetMatchData().computer_num;
#else
	numPlayer = GAMEMATCHINFO.GetMatchData().player_num;
#endif

	//エフェクトテクスチャ読み込み
	POLYGONEFFECT.LoadEffectTextureData();

	//	環境設定
	iexLight::SetAmbient(0x404040);
	iexLight::SetFog(800, 1000, 0);

	Vector3 dir(1.0f, -1.0f, -0.5f);
	dir.Normalize();
	iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);

	//デバック用のノードマネージャ作成
	Field_Debugger::Create_Instance();
	field_Debug->Initialize(GAMEMATCHINFO.GetMatchData().computer_num, 1.0, 1.0f);

	//メッシュマネージャ作成
	GAMEMATCHINFO.GetMeshMgr().MeshRegister(new MYMESH("DATA\\OBJECT\\ColaWater\\ColaWater.imo"), (MeshManager::MESH_ID)GameMatchInfomation::_COLA_WATER);
	GAMEMATCHINFO.GetMeshMgr().MeshRegister(new MYMESH("DATA\\OBJECT\\Cola\\ColaPet.imo"), (MeshManager::MESH_ID)GameMatchInfomation::_COLA_GUN);
	GAMEMATCHINFO.GetMeshMgr().MeshRegister(new MYMESH("DATA\\OBJECT\\Parachute\\Parachute.imo"), (MeshManager::MESH_ID)GameMatchInfomation::_PARACHUTE);
	GAMEMATCHINFO.GetMeshMgr().MeshRegister(new MYMESH("DATA\\CHR\\COLA_KUN\\COLA_KUN_CRASH.imo"), (MeshManager::MESH_ID)GameMatchInfomation::_EMPTY_CAN);

	//エイムターゲットマネージャ
	m_pAimTargetManager = new AimTargetManager;

	//ダメージマネージャ
	m_pDamageObjectManager = new DamageObjectManager;

	//アイコンマネージャ
	m_pScrIconMgr = new ScreenIconManager();

	//ステージマネージャの実体を作成
	MAP_MANAGER.CreateInstance();

	MyAssert(MAP_MANAGER.LoadObject("DATA\\stage_mesh\\test2.cod", "DATA\\stage_mesh\\MeshList.txt"), "マップオブジェクトロードに失敗しました");

	//画面分割設定
	m_pScreenSeparation = new ScreenSeparation(numPlayer, new ScreenCreater_DebugMatch());

	//コントローラマネージャの実体を作成
	m_pControllerMgr = new ControllerManager;

	//AIのフィールド作成
	navigation_system->Initialize("DATA\\NPC\\StageNodeData\\node.txt", "DATA\\NPC\\StageNodeData\\waypointinfo.txt");
	npc_commander->Create_Instance();
	npc_commander->Initialize(2);

	//環境音読み込み
	env_Sound->Initialize();
	Rand::Initialize();
	ai_message->Create_Insatnce();
	//コントロールオブジェクトマネージャの実体を作成
	CONTROLLOBJMGR.InstanceCreate();

	m_pTeamDataMgr = new TeamDataManager(2);	//チーム数によって変える必要がるある

	int nPlayer = GAMEMATCHINFO.GetMatchData().player_num;

	m_pScoreCalcMgr = new ScoreCalcManager();

	//キャラクタとりあえず作ってみる
	for (int i = game_id::id_controll_obj_first, controller_cnt = 0; i < game_id::id_controll_obj_first + GAMEMATCHINFO.GetMatchData().all_player_num; i++, nPlayer--)
	{
		int team_type = (i - game_id::id_controll_obj_first) % 2;	//とりあえずチームを２つ作る

		ENTITY_ID id;
		MyAssert(IDMANAGER.GetEmptyControllObjID(&id), "IDに空きがありません");

#if MOVIE_MODE
		if (i == game_id::id_controll_obj_first)
		{
			CreateMovieModeObj(id, (player_type::PLAYER), 0, team_type);
		}else{
			CreateCharacter(id, (nPlayer > 0) ? (player_type::PLAYER) : (player_type::COMPUTER), i - (int)game_id::id_controll_obj_first, team_type);
		}
#else
		CreateCharacter(id, (nPlayer>0) ? (player_type::PLAYER) : (player_type::COMPUTER), i - (int)game_id::id_controll_obj_first, team_type);
#endif
	}

	//弾マネージャを作成
	m_pBulletManager = new BulletManager;

	//水たまりマネージャを作成
	m_pColaWaterMgr = new WaterPoolManager;

	m_pNewsTelopMgr = new NewsTelopManager();

	m_pNewsUpdater = new NewsTelopUpdaterShareMatch();

	m_pTimeRender = new TimeRender(
		GAMEMATCHINFO.GetTextureMgr().GetTexturePtr(TextureManager::NUMBER),
		GAMEMATCHINFO.GetTextureMgr().GetTexturePtr(TextureManager::TIMER),
		false);
//	m_pTimeRender->SetMaxTime(60*3);
	m_pTimeRender->SetMaxTime(60 * 60 * 3);
	m_pTimeRender->Reset();

	SOUNDPLAYER.PlayBGM(
		sound_t::share_match
		);
		
}



//****************************************************//
//　解放
//****************************************************//

ShareMatchState::~ShareMatchState()
{
	//解放
	npc_commander->Release_Instance();
	navigation_system->Release();
	field_Debug->Release();
	ai_message->Release_Instance();
	SAFE_DELETE(m_pColaWaterMgr);
	SAFE_DELETE(m_pScreenSeparation);
	SAFE_DELETE(m_pControllerMgr);
	SAFE_DELETE(m_pBulletManager);
	CONTROLLOBJMGR.InstanceRelase();
	SAFE_DELETE(m_pDamageObjectManager);
	Field_Debugger::Release();
	MAP_MANAGER.Release();
	SAFE_DELETE(m_pAimTargetManager);	//エイムターゲットのdeleteを直前に実行する必要がある
	POLYGONEFFECT.Release();
	SAFE_DELETE(m_pScoreCalcMgr);
	SAFE_DELETE(m_pScrIconMgr);
	SAFE_DELETE(m_pNewsTelopMgr);
	SAFE_DELETE(m_pNewsUpdater);
	SAFE_DELETE(m_pTimeRender);
	IDMANAGER.Release();	//これは最後で

	delete debug_particle::g_pNumberTexture;
	SAFE_DELETE(m_pTeamDataMgr);
	
}

//****************************************************//
//　毎フレームの更新
//****************************************************//
void ShareMatchState::Update()
{

	static void(ShareMatchState::*functions[])()=
	{
		&ShareMatchState::MatchState_PrePlay,
		&ShareMatchState::MatchState_Play,
		&ShareMatchState::MatchState_Finish,
	};

	m_pTeamDataMgr->Update();

	//デバッグ用文字を初期化
	ITDEBUGSTRMGR.Clear();

	POLYGONEFFECT.Update();

	//マップオブジェクトを描画
	FUNCTIONSPEEDCHECK(MAP_MANAGER.Update());

	//AIのHandleMessage更新
	ai_message->Update();

	(this->*functions[m_State])();

	//コントロールできるオブジェクトの更新
	FUNCTIONSPEEDCHECK(CONTROLLOBJMGR.Update(*m_pTeamDataMgr));


	SetSoundCameraMatrix();

	//弾丸の更新
	FUNCTIONSPEEDCHECK(m_pBulletManager->Update());

	navigation_system->Update();
	env_Sound->Update();

	m_pColaWaterMgr->Update();

	iexParticle::Update();
	

	m_pDamageObjectManager->RenderDebugParticle();

	SetTeamShareRatio();

	m_pScoreCalcMgr->Update();

	m_pScrIconMgr->Update();

	m_pNewsTelopMgr->Update();

	m_pNewsUpdater->Update();

}

//****************************************************//
//　描画
//****************************************************//

//プレイヤーならtrue
static bool select_player_pred(LPCONTROLLER c)
{
#if COMPUTER_RENDER
	return true;
#else
	return c->GetMatchData().player_type == player_type::PLAYER;
#endif
}

void ShareMatchState::Render()
{

	//カメラリスト作成
	CONTROLLOBJLIST obj_list;

	//キャラクタのカメラも入れてみる
	CONTROLLERLIST controller_list = m_pControllerMgr->GetController(select_player_pred);

	for (CONTROLLERLIST::iterator it = controller_list.begin();
		it != controller_list.end();
		++it)
		obj_list.push_back((*it)->GetControllObjectPtr());

	//スクリーン配置座標を作成
	std::list<RECTI>	screen_positions;
	CreateScreenRect(&screen_positions, (int)obj_list.size());

	//配置
	m_pScreenSeparation->SetScreen(screen_positions);

	//画面テクスチャを作成
	FUNCTIONSPEEDCHECK(m_pScreenSeparation->CreateScreenTexture(obj_list, this));

	//描画
	m_pScreenSeparation->DrawScreen();

	//ITDEBUGSTRMGR.Render();
}

void ShareMatchState::SetSoundCameraMatrix()
{
	//キャラクタのカメラも入れてみる
	CONTROLLERLIST controller_list = m_pControllerMgr->GetController(select_player_pred);

	for (CONTROLLERLIST::iterator it = controller_list.begin();
		it != controller_list.end();
		++it)
	{
		SOUNDPLAYER.UpdateCameraMatrix(
			(GameSoundManager::PlayerNum)(*it)->GetMatchData().player_num,
			ControllObjFunc::GetCamera((*it)->GetControllObjectPtr())->GetMatrix());
	}
}

void ShareMatchState::SetNextState(SHARE_MATCH_STATE	next)
{
	m_State = next;
	m_TimeCount = 0;
}

void ShareMatchState::MatchState_PrePlay()
{
	//コントローラー管理クラスのアップデート
//	FUNCTIONSPEEDCHECK(m_pControllerMgr->Update());
	if (GAMEMATCHINFO.isGameStart())
	{
		SetNextState(_PLAY);
	}
}
void ShareMatchState::MatchState_Play()
{
	//キャラクタのカメラも入れてみる
	CONTROLLERLIST controller_list = m_pControllerMgr->GetController(select_player_pred);
	LPCONTROLLER pCont = nullptr;
	if (!controller_list.empty())pCont = *controller_list.begin();

	//コントローラー管理クラスのアップデート
	FUNCTIONSPEEDCHECK(m_pControllerMgr->Update());

	//時間更新
	m_pTimeRender->Update(1);
	m_pNewsUpdater->SetRemainingFrame((DWORD)m_pTimeRender->GetRemainingFrame());
	
	if (m_pTimeRender->GetCount() <= 0)
	{
		SetNextState(_FINISH);
	}
}
void ShareMatchState::MatchState_Finish()
{
	if (m_TimeCount++ == 0)
	{
		m_pScoreCalcMgr->GameEnd();
	}

	if (m_pScoreCalcMgr->isFinishResultUI())
	{
		m_isGameFinish = true;
	}
}

void ShareMatchState::SetTeamShareRatio()
{
	MapObjectManager::SHAREPARAM_ARRAY share;
	MAP_MANAGER.Get_ShareValue(share);

	for (MapObjectManager::SHAREPARAM_ARRAY::iterator it = share.begin();
		it != share.end();
		++it)
	{
		m_pScoreCalcMgr->SetShareRatio(it->team_type, it->share_power);
	}
}

bool ShareMatchState::CreateCharacter(ENTITY_ID id, PLAYER_TYPE type,int player_num,TEAM_TYPE team)
{
	ControllerButtonSet button_set;
	//デフォルトButton配置をセット
	SetUsualButtonSet(button_set, (type == player_type::PLAYER) ? player_num : 0);

	static char name[CODBO_GAME_PLAYER_MAX][5];

	for (int i = 0; i < CODBO_GAME_PLAYER_MAX; ++i)
	{
		sprintf(name[i], "%c", (char)('A' + (char)i));
	}

	CharacterBrainBase* pBRAIN;
	CharacterUI* chr_ui = new CharacterUI;

	if (type==player_type::PLAYER)
		pBRAIN = new CharacterBrainRealPlayer(&button_set);
	else
		pBRAIN = NPC_Factory::Create(
		name[player_num], npc_strength::strong,
		player_num,team,id);

	LPCHARACTER pCharacter = new Character(
		id,
		pBRAIN,
		new CharacterModelResouce(
		"DATA\\CHR\\COLA_KUN\\COLA_KUN.iem",
		GAMEMATCHINFO.GetTextureMgr().GetTexturePtr((TextureManager::TEXTURE_ID)(TextureManager::CHR_TEXTURE_SODA + team))),	//テスト用モデル描画クラスをいれる
		chr_ui,
		team,
		&m_pTeamDataMgr->MyData(team)
		);


	chr_ui->SetOwner(pCharacter);

	//カメラステートをセット
	LPCAMERA cam = ControllObjFunc::GetCamera(pCharacter);
	cam->SetPos(Vector3(0, 40, -10));
	cam->SetTarget(Vector3(0, 35, 0));
	cam->SetNewState(CameraFunction::GetCharacterUsualModeCamera(pCharacter, cam, CameraFunction::_RIGHT));

	//コントローラ作成
	PLAYERMATCHDATA match_data;
	InitPlayerMatchData(&match_data);

	match_data.team_type = team;
	match_data.button_set = button_set;
	match_data.player_num = player_num;
	match_data.player_type = type;

	m_pScoreCalcMgr->CreateTeamData(match_data.team_type);
	m_pScoreCalcMgr->CreatePlayerData(match_data.team_type, match_data.player_num);

	Controller* pController = new Controller(match_data);

	//作成したコントローラの操作するオブジェクトにキャラクタをセット

	chr_ui->SetColaGaugeColor(
		(CODMgr::GetTeamColaWaterColor(team) & 0x00FFFFFF) | 0x50000000);

	m_pControllerMgr->AddController(pController);

	//デバッグ用の経路クラスに登録
	field_Debug->Register(id, dword_color::red);

	ChrFunc::SetStartCharacter(
		pCharacter,
		pController
		);

	return true;
}

bool ShareMatchState::CreateMovieModeObj(ENTITY_ID id, PLAYER_TYPE type, int player_num, TEAM_TYPE team)
{
	ControllerButtonSet button_set;
	//デフォルトButton配置をセット
	SetUsualButtonSet(button_set, (type == player_type::PLAYER) ? player_num : 0);

	MovieControllobj* pObj = new MovieControllobj(
		id,
		team,
		&m_pTeamDataMgr->MyData(team)
		);

	//カメラステートをセット
	LPCAMERA cam = ControllObjFunc::GetCamera(pObj);
	cam->SetPos(Vector3(0, 40, -10));
	cam->SetTarget(Vector3(0, 35, 0));

	//コントローラ作成
	PLAYERMATCHDATA match_data;
	InitPlayerMatchData(&match_data);

	match_data.team_type = team;
	match_data.button_set = button_set;
	match_data.player_num = player_num;
	match_data.player_type = type;

	m_pScoreCalcMgr->CreateTeamData(match_data.team_type);
	m_pScoreCalcMgr->CreatePlayerData(match_data.team_type, match_data.player_num);

	Controller* pController = new Controller(match_data);

	//作成したコントローラの操作するオブジェクトにキャラクタをセット
	m_pControllerMgr->AddController(pController);

	CONTROLLOBJMGR.AddObject(pObj);
	pController->SetControllObj(pObj);

	return true;
}

void ShareMatchState::GameRender(LPIEXVIEW pView, ControllObject	*pOwner)
{

}

void	ShareMatchState::Water_Render(LPIEXVIEW pView, ControllObject*pOwner)
{
	m_pColaWaterMgr->Render();
}

//****************************************************//
//　ゲーム開始
//****************************************************//
void ShareMatchState::GameStart()
{

}

//****************************************************//
//　ゲーム終了確認
//****************************************************//
bool ShareMatchState::isGameFinish()
{
	return m_isGameFinish;
}

void ShareMatchState::GetResultScene(Scene**ppOut)
{
	//*ppOut = new SceneSelect();
	*ppOut = new SceneEnd();
}
