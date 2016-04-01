#include "All.h"


//****************************************************//
//　初期化
//****************************************************//

static int numPlayer;

DebugMatchState::DebugMatchState():
m_pScreenSeparation(nullptr),
m_pCamera(nullptr),
m_pControllerMgr(nullptr)
{

}

void DebugMatchState::Initialize()
{
	debug_particle::g_pNumberTexture = new iex2DObj("DATA\\number.png");

	numPlayer = GAMEMATCHINFO.GetMatchData().player_num;

	//Luaのキー作成
	CreateLuaState();

	//ゲームの定数パラメーターを読み込み
	MyAssert(LoadLuaGameParam("DATA\\ScriptFile\\GameSystemConstParam.txt"), "キャラクタの定数パラメーター読み込みに失敗しました。");

	//キャラクタの定数パラメーターを読み込み
	MyAssert(LoadLuaCharacterParam("DATA\\ScriptFile\\CharacterConstParam.txt"), "キャラクタの定数パラメーター読み込みに失敗しました。");

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
	GAMEMATCHINFO.GetMeshMgr().MeshRegister(new MYMESH("DATA\\OBJECT\\ColaWater\\ColaWater.imo"), (MeshManager::MESH_ID)GameMatchInfomation::_COLA_GUN);


	//エイムターゲットマネージャ
	m_pAimTargetManager = new AimTargetManager;

	//ダメージマネージャ
	m_pDamageObjectManager = new DamageObjectManager;

	//ステージマネージャの実体を作成
	MAP_MANAGER.CreateInstance();
	MyAssert(MAP_MANAGER.LoadObject("DATA\\stage_mesh\\test2.cod","DATA\\stage_mesh\\MeshList.txt"), "マップオブジェクトロードに失敗しました");



	//画面分割設定
	m_pScreenSeparation = new ScreenSeparation(numPlayer,new ScreenCreater_DebugMatch());

	//コントローラマネージャの実体を作成
	m_pControllerMgr = new ControllerManager;

	//AIのフィールド作成
	navigation_system->Initialize("DATA\\NPC\\StageNodeData\\node.txt", "DATA\\NPC\\StageNodeData\\waypointinfo.txt");
	//field_Info->Load_Node();
	//field_Info->Load_Visibility("DATA\\NPC\\StageNodeData\\visibility.txt");
	//環境音読み込み
	env_Sound->Initialize();
	Rand::Initialize();
	ai_message->Create_Insatnce();

	//コントロールオブジェクトマネージャの実体を作成
	CONTROLLOBJMGR.InstanceCreate();

	static char* name[4] =
	{
		"jack",
		"devid",
		"snake",
		"hal",
	};
	DWORD color[2] =
	{
		dword_color::red,
		dword_color::white,
	};

	int nPlayer = GAMEMATCHINFO.GetMatchData().player_num;

	//キャラクタとりあえず作ってみる
	for (int i = game_id::id_controll_obj_first, controller_cnt = 0; i < game_id::id_controll_obj_first + GAMEMATCHINFO.GetMatchData().all_player_num; i++, nPlayer--)
	{
		//コントローラボタン配置構造体
		ControllerButtonSet button_set;

		//デフォルトButton配置をセット
		SetUsualButtonSet(button_set, controller_cnt++);

		CharacterUI* chr_ui = new CharacterUI;

		ENTITY_ID id;
		MyAssert(IDMANAGER.GetEmptyControllObjID(&id), "IDに空きがありません");

		CharacterBrainBase* pBRAIN;
		if (nPlayer > 0)
			pBRAIN = new CharacterBrainRealPlayer(&button_set);
		else
			int a = 0;
			/*pBRAIN = new NPC_Brain(
			name[i - game_id::id_controll_obj_first],
			i - game_id::id_controll_obj_first);*/

		LPCHARACTER pCharacter = new Character(
			id,												
			pBRAIN,
			new CharacterModelResouce("DATA\\CHR\\COLA_KUN\\cola_kun.iem",0),	//テスト用モデル描画クラスをいれる
			//new TestCharacterModel("DATA\\CHR\\cola_test.IEM"),
			chr_ui,
			0,//////,
			nullptr///////
			);

		chr_ui->SetOwner(pCharacter);

		//カメラステートをセット
		LPCAMERA cam = ControllObjFunc::GetCamera(pCharacter);
		cam->SetPos(Vector3(0, 40, -10));
		cam->SetTarget(Vector3(0, 35, 0));
		cam->SetNewState(CameraFunction::GetCharacterUsualModeCamera(pCharacter, cam,CameraFunction::_RIGHT));

		//コントローラ作成
		PLAYERMATCHDATA match_data;
		InitPlayerMatchData(&match_data);

		match_data.team_type = (i - game_id::id_controll_obj_first) % 2;	//とりあえずチームを２つ作る
		match_data.button_set = button_set;
		match_data.player_num = i - game_id::id_controll_obj_first;
		match_data.player_type = (nPlayer > 0) ? (player_type::PLAYER) : (player_type::COMPUTER);

		Controller* pController = new Controller(match_data);

		//作成したコントローラの操作するオブジェクトにキャラクタをセット
		pController->SetControllObj(pCharacter);
		m_pControllerMgr->AddController(pController);

		//コントローラマネージャに追加
		CONTROLLOBJMGR.AddObject(pCharacter);
		//デバッグ用の経路クラスに登録
		field_Debug->Register((ENTITY_ID)i, dword_color::red);
	}

	//弾マネージャを作成
	m_pBulletManager = new BulletManager;

	//水たまりマネージャを作成
	m_pColaWaterMgr = new WaterPoolManager;

	//カメラ設定（とりあえず映るかどうか）
	m_pCamera = new Camera;
	m_pCamera->SetPos(Vector3(10, 7, -20));
	m_pCamera->SetTarget(Vector3(0, 5, 0));
	m_pCamera->SetNewState(new FreezeCamera);

	POLYGONEFFECT.LoadEffectTextureData();
}



//****************************************************//
//　解放
//****************************************************//

DebugMatchState::~DebugMatchState()
{
	//解放
	navigation_system->Release();
	field_Debug->Release();
	ai_message->Release_Instance();
	SAFE_DELETE(m_pColaWaterMgr);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pScreenSeparation);
	SAFE_DELETE(m_pControllerMgr);
	SAFE_DELETE(m_pBulletManager);
	CONTROLLOBJMGR.InstanceRelase();
	SAFE_DELETE(m_pDamageObjectManager);
	Field_Debugger::Release();
	MAP_MANAGER.Release();
	SAFE_DELETE(m_pAimTargetManager);	//エイムターゲットのdeleteを直前に実行する必要がある
	POLYGONEFFECT.Release();
	IDMANAGER.Release();	//これは最後で

	delete debug_particle::g_pNumberTexture;
}

//****************************************************//
//　毎フレームの更新
//****************************************************//
void DebugMatchState::Update()
{
	//デバッグ用文字を初期化
	ITDEBUGSTRMGR.Clear();

	//マップオブジェクトを描画
	FUNCTIONSPEEDCHECK(MAP_MANAGER.Update());

	//AIのHandleMessage更新
	ai_message->Update();


	//コントロールできるオブジェクトの更新
//	FUNCTIONSPEEDCHECK(CONTROLLOBJMGR.Update());

	//コントローラー管理クラスのアップデート
	FUNCTIONSPEEDCHECK(m_pControllerMgr->Update());

	//弾丸の更新
	FUNCTIONSPEEDCHECK(m_pBulletManager->Update());

	navigation_system->Update();
	env_Sound->Update();

	if (GetAsyncKeyState('F') & 0x01)
	{
		env_Sound->Register(sound_type::gun, (int)GAMEMATCHINFO.GetMatchTime(), Vector3(-3.014f, -.0f, 0.623f), 1);
	}
	m_pColaWaterMgr->Update();

	iexParticle::Update();
	POLYGONEFFECT.Update();

	//MyDebugMgrStr(color_smp::RED, "Update経過時間 %d ms", (int)ProgramSpeedCheck::GetTime());
}

//****************************************************//
//　描画
//****************************************************//

//プレイヤーならtrue
static bool select_player_pred(LPCONTROLLER c)
{
	return c->GetMatchData().player_type == player_type::PLAYER;
}

void DebugMatchState::Render()
{

	//カメラリスト作成
	CONTROLLOBJLIST obj_list;

	//とりあえずデバッグようカメラを放り込む
	//	camera_list.push_back(m_pCamera);

	//キャラクタのカメラも入れてみる
	CONTROLLERLIST controller_list = m_pControllerMgr->GetController(select_player_pred);

	for (CONTROLLERLIST::iterator it = controller_list.begin();
		it != controller_list.end();
		++it)
		obj_list.push_back((*it)->GetControllObjectPtr());

	//スクリーン配置座標を作成
	std::list<RECTI>	screen_positions;
	CreateScreenRect(&screen_positions, GAMEMATCHINFO.GetMatchData().player_num);

	//配置
	m_pScreenSeparation->SetScreen(screen_positions);

	//画面テクスチャを作成
	m_pScreenSeparation->CreateScreenTexture(obj_list, this);

	//描画
	m_pScreenSeparation->DrawScreen();

//	MyDebugMgrStr(color_smp::BLACK, "Render経過時間 %d ms", (int)ProgramSpeedCheck::GetTime());

	static bool dbgDraw=true;
	//デバッグ用文字を描画
	if (dbgDraw)ITDEBUGSTRMGR.Render();
	if (KEY(KEY_ENTER, 0) == 3)dbgDraw = !dbgDraw;

}

void DebugMatchState::GameRender(LPIEXVIEW pView, ControllObject	*pOwner)
{
	////ビュー行列設定
	//pView->Activate();

	////ビューポートセット
	////m_pScreenSeparation->Set_ViewPort(numPlayer);

	////クリア
	//pView->Clear();
	//
	////マップオブジェクトを描画
	////MAP_MANAGER.Render("static_object");
	//MAP_MANAGER.Render();

	////コントロール可能なオブジェクトを描画
	////CONTROLLOBJMGR.Render("toon");
	//CONTROLLOBJMGR.Render();



	////弾描画
	//m_pBulletManager->Render();
	////水たまり描画
	//m_pColaWaterMgr->Render();
	//iexParticle::Render();
	//

	////field_Debug->Render_GoalNode();
	////field_Debug->Reder_GoalRoute();
	////field_Debug->Render_AllEdge();
	//field_Debug->Render_Edge(ControllObjFunc::GetPos(pOwner));
	//pOwner->RenderUI();
}

void	DebugMatchState::Water_Render(LPIEXVIEW pView, ControllObject*pOwner)
{
	m_pColaWaterMgr->Render();
}

//****************************************************//
//　ゲーム開始
//****************************************************//
void DebugMatchState::GameStart()
{

}

//****************************************************//
//　ゲーム終了確認
//****************************************************//
bool DebugMatchState::isGameFinish()
{
	return false;
}

