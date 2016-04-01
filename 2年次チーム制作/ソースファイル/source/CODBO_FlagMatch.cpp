//#include "All.h"
//
//
////****************************************************//
////　初期化
////****************************************************//
//
//FlagMatchState::FlagMatchState(const CODMatchData& match_data) :
//m_pScreenSeparation(nullptr),
//m_pCamera(nullptr),
//m_pControllerMgr(nullptr),
//m_MatchData(match_data)
//{
//
//}
//
//void FlagMatchState::Initialize()
//{
//
//	//Luaのキー作成
//	CreateLuaState();
//
//	//キャラクタの定数パラメーターを読み込み
//	MyAssert(LoadLuaCharacterParam("DATA\\ScriptFile\\CharacterConstParam.txt"), "キャラクタの定数パラメーター読み込みに失敗しました。");
//
//	//	環境設定
//	iexLight::SetAmbient(0x404040);
//	iexLight::SetFog(800, 1000, 0);
//
//	Vector3 dir(1.0f, -1.0f, -0.5f);
//	dir.Normalize();
//	iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);
//
//	//デバック用のノードマネージャ作成
//	Field_Debugger::Create_Instance();
//	field_Debug->Initialize(m_MatchData.computer_num, 1.0, 1.0f);
//
//	//ステージマネージャの実体を作成
//	MAP_MANAGER.CreateInstance();
//	MyAssert(MAP_MANAGER.LoadObject("DATA\\stage_mesh\\test.cod","DATA\\stage_mesh\\MeshList.txt"), "マップオブジェクトロードに失敗しました");
//
//	//ダメージマネージャ
//	m_pDamageObjectManager = new DamageObjectManager;
//
//	//エイムターゲットマネージャ
//	m_pAimTargetManager = new AimTargetManager;
//
//	//画面分割設定
//	m_pScreenSeparation = new ScreenSeparation(m_MatchData.player_num,);
//
//	//コントローラマネージャの実体を作成
//	m_pControllerMgr = new ControllerManager;
//
//	//コントロールオブジェクトマネージャの実体を作成
//	CONTROLLOBJMGR.InstanceCreate();
//
//	//弾マネージャを作成
//	m_pBulletManager = new BulletManager;
//
//	//キャラクター作成
//	MyAssert(CreateCharacters(), "キャラクタ作成に失敗しました");
//}
//
//
//
////****************************************************//
////　解放
////****************************************************//
//
//FlagMatchState::~FlagMatchState()
//{
//	//解放
//	SAFE_DELETE(m_pCamera);
//	SAFE_DELETE(m_pScreenSeparation);
//	SAFE_DELETE(m_pControllerMgr);
//	SAFE_DELETE(m_pBulletManager);
//	SAFE_DELETE(m_pDamageObjectManager);
//	CONTROLLOBJMGR.InstanceRelase();
//	Field_Debugger::Release();
//	MAP_MANAGER.Release();
//	SAFE_DELETE(m_pAimTargetManager);	//エイムターゲットのdeleteを直前に実行する必要がある
//	
//	IDMANAGER.Release();	//これは最後で
//}
//
////****************************************************//
////　毎フレームの更新
////****************************************************//
//void FlagMatchState::Update()
//{
//
//	ProgramSpeedCheck::Begin();
//
//	//デバッグ用文字を初期化
//	ITDEBUGSTRMGR.Clear();
//
//	//マップオブジェクトを描画
//	FUNCTIONSPEEDCHECK(MAP_MANAGER.Update());
//
//	//コントロールできるオブジェクトの更新
//	FUNCTIONSPEEDCHECK(CONTROLLOBJMGR.Update());
//
//	//コントローラー管理クラスのアップデート
//	FUNCTIONSPEEDCHECK(m_pControllerMgr->Update());
//
//	//弾丸の更新
//	FUNCTIONSPEEDCHECK(m_pBulletManager->Update());
//
//	iexParticle::Update();
//
//	MyDebugMgrStr(color_smp::RED, "Update経過時間 %d ms", (int)ProgramSpeedCheck::GetTime());
//}
//
////****************************************************//
////　描画
////****************************************************//
////プレイヤーならtrue
//static bool select_player_pred(LPCONTROLLER c)
//{
//	return c->GetMatchData().player_type == player_type::PLAYER;
//}
//
//void FlagMatchState::Render()
//{
//	ProgramSpeedCheck::Begin();
//
//	//カメラリスト作成
//	CONTROLLOBJLIST obj_list;
//
//	//とりあえずデバッグようカメラを放り込む
//	//	camera_list.push_back(m_pCamera);
//
//	//キャラクタのカメラも入れてみる
//	CONTROLLERLIST controller_list = m_pControllerMgr->GetController(select_player_pred);
//
//	for (CONTROLLERLIST::iterator it = controller_list.begin();
//		it != controller_list.end();
//		++it)
//		obj_list.push_back((*it)->GetControllObjectPtr());
//
//	//スクリーン配置座標を作成
//	std::list<RECTI>	screen_positions;
//	CreateScreenRect(&screen_positions, m_MatchData.player_num);
//
//	//配置
//	m_pScreenSeparation->SetScreen(screen_positions);
//
//	//画面テクスチャを作成
//	m_pScreenSeparation->CreateScreenTexture(obj_list, this);
//
//	//描画
//	m_pScreenSeparation->DrawScreen();
//
//	MyDebugMgrStr(color_smp::BLACK, "Render経過時間 %d ms", (int)ProgramSpeedCheck::GetTime());
//
//	//デバッグ用文字を描画
//	ITDEBUGSTRMGR.Render();
//}
//
//bool FlagMatchState::CreateCharacters()
//{
//	//キャラクタを作成
//	auto it = m_MatchData.player_data.begin();
//	controller::CONTROLLER_NUM controller_num = 0;
//	LPCHARACTER	pCharacter;
//
//	while (it != m_MatchData.player_data.end())
//	{
//		const PLAYERMATCHDATA& data = *it;
//
//		switch (data.player_type)
//		{
//		case player_type::PLAYER:
//			ChrFunc::GetNewPlayerCharacter(&pCharacter, &data.button_set);
//			break;
//		case player_type::COMPUTER:
//
//		case player_type::SPECTATOR:
//		default:
//			MyAssert(false, "プレイヤータイプが間違っています");
//			break;
//		}
//
//		//カメラステートをセット
//		LPCAMERA cam = ControllObjFunc::GetCamera(pCharacter);
//		cam->SetPos(Vector3(0, 40, -10));
//		cam->SetTarget(Vector3(0, 35, 0));
//		cam->SetNewState(new UsualPlayerMoveCamera(cam, pCharacter, 0.15f, 0.09f, Vector3(0.6f, 0.9f, -2.4f), Vector3(0.6f, 0.9f, 1)));
//
//		//コントローラ作成
//		Controller* pController = new Controller(data);
//
//		//作成したコントローラの操作するオブジェクトにキャラクタをセット
//		pController->SetControllObj(pCharacter);
//		m_pControllerMgr->AddController(pController);
//
//		//コントローラマネージャに追加
//		CONTROLLOBJMGR.AddObject(pCharacter);
//	}
//
//	return true;
//}
//
//void FlagMatchState::GameRender(LPIEXVIEW pView, ControllObject	*pOwner)
//{
//	//ビュー行列設定
//	pView->Activate();
//
//	//ビューポートセット
//	m_pScreenSeparation->Set_ViewPort(m_MatchData.player_num);
//
//	//クリア
//	pView->Clear();
//
//	//マップオブジェクトを描画
//	MAP_MANAGER.Render();
//
//	//コントロール可能なオブジェクトを描画
//	CONTROLLOBJMGR.Render();
//
//	//弾描画
//	m_pBulletManager->Render();
//
//	iexParticle::Render();
//
//	pOwner->RenderUI();
//}
//
////****************************************************//
////　ゲーム開始
////****************************************************//
//void FlagMatchState::GameStart()
//{
//
//}
//
////****************************************************//
////　ゲーム終了確認
////****************************************************//
//bool FlagMatchState::isGameFinish()
//{
//	return false;
//}
//
