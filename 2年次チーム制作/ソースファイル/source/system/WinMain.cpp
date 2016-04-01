#include "../All.h"
#include <io.h>
#include <fcntl.h>



//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************


char*	AppTitle = "スパークリングウォーターガン";

BOOL	bFullScreen = FALSE;
DWORD	ScreenMode  = SCREEN720p;

Framework*	MainFrame = NULL;

//*****************************************************************************************************************************
//		アプリケーション初期化
//*****************************************************************************************************************************

BOOL	InitApp( HWND hWnd )
{
	//	IEXシステム初期化
	IEX_Initialize( hWnd, bFullScreen, ScreenMode );
	IEX_InitAudio();
	IEX_InitInput();

	//	システムの初期化
	SYSTEM_Initialize();
	//	メインフレームワーク生成
	MainFrame = new Framework(FPS_FLEX);

	collision::InitializeParam();

	//	初期シーン登録

	MainFrame->ChangeScene( new SceneSelect() );


	//Luaのキー作成
	CreateLuaState();

	//ゲームの定数パラメーターを読み込み
	MyAssert(LoadLuaGameParam("DATA\\ScriptFile\\GameSystemConstParam.txt"), "キャラクタの定数パラメーター読み込みに失敗しました。");

	//キャラクタの定数パラメーターを読み込み
	MyAssert(LoadLuaCharacterParam("DATA\\ScriptFile\\CharacterConstParam.txt"), "キャラクタの定数パラメーター読み込みに失敗しました。");

	//サウンド読み込み
	MyAssert(LoadLuaMusic("DATA\\ScriptFile\\SEBGM.txt"), "サウンド読み込みに失敗しました。");


	return TRUE;
}

//*****************************************************************************************************************************
//		ウィンドウプロシージャ
//*****************************************************************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case WM_DESTROY: PostQuitMessage(0); return 0;
#ifdef CONSOLE
	case WM_CREATE:
		AllocConsole();
		*stdout = *_fdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT), "w");
		setvbuf(stdout, 0, _IONBF, 0);
		break;
#endif
	case WM_KEYDOWN:
			switch (wParam){
			case VK_ESCAPE: PostMessage(hWnd, WM_CLOSE, 0, 0); return 0;
			case VK_F1:		iexSystem::OpenDebugWindow(); return 0;
			}
			break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

//*****************************************************************************************************************************
//		ウィンドウ作成
//*****************************************************************************************************************************
HWND InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	HWND			hWnd;

	//	スクリーンサイズ取得
	RECT	WindowSize;
	iexSystem::GetScreenRect( ScreenMode, WindowSize );

	//	ウィンドウクラス設定
	WNDCLASS	wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "スパークリングウォーターガン";
	RegisterClass(&wc);

	//	ウィンドウ作成
	if( !bFullScreen ){
		AdjustWindowRect( &WindowSize, WS_OVERLAPPEDWINDOW, FALSE );
		hWnd = CreateWindow( "スパークリングウォーターガン", AppTitle, WS_OVERLAPPEDWINDOW, 
								0, 0, WindowSize.right-WindowSize.left, WindowSize.bottom-WindowSize.top,
								NULL, NULL, hInstance, NULL);
	} else {
		hWnd = CreateWindow( "スパークリングウォーターガン", AppTitle, WS_POPUP, 0, 0, WindowSize.right,WindowSize.bottom, NULL, NULL, hInstance, NULL);
		ShowCursor(FALSE);
	}
	if( !hWnd ) return NULL;
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

    return hWnd;
}

//*****************************************************************************************************************************
//		WinMain
//*****************************************************************************************************************************
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	srand((UINT)time(0));

	MSG		msg;
	HWND	hWnd;

	MyMemoryLeakCheck;

	if( GetAsyncKeyState(VK_CONTROL)&0x8000 ) bFullScreen = TRUE;

	hWnd = InitWindow(hInstance, nCmdShow);
	InitApp(hWnd);

	try{
		//	メインループ
		for (;;){
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){
				if (!GetMessage(&msg, NULL, 0, 0)) break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				if (MainFrame->Update()) MainFrame->Render();
			}
		}
	}

	catch (Error_GameExit& err_data)
	{
		MessageBoxPlus(" エラーが発生しました\n　%s \nタイプ番号 %d ", err_data.GetStr(), err_data.GetErrData());
	}

	//	全解放	
	delete	MainFrame;
	GameMatchInfomation::ReleaseInstance();
	GameSoundManager::ReleaseInstance();
	LUA.Release();
	iexParticle::Release();
	SYSTEM_Release();
	iexSystem::CloseDebugWindow();
	IEX_ReleaseInput();
	IEX_ReleaseAudio();
	IEX_Release();

	return 0;
}
