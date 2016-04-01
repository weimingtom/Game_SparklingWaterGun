#include "../All.h"
#include <io.h>
#include <fcntl.h>



//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************


char*	AppTitle = "�X�p�[�N�����O�E�H�[�^�[�K��";

BOOL	bFullScreen = FALSE;
DWORD	ScreenMode  = SCREEN720p;

Framework*	MainFrame = NULL;

//*****************************************************************************************************************************
//		�A�v���P�[�V����������
//*****************************************************************************************************************************

BOOL	InitApp( HWND hWnd )
{
	//	IEX�V�X�e��������
	IEX_Initialize( hWnd, bFullScreen, ScreenMode );
	IEX_InitAudio();
	IEX_InitInput();

	//	�V�X�e���̏�����
	SYSTEM_Initialize();
	//	���C���t���[�����[�N����
	MainFrame = new Framework(FPS_FLEX);

	collision::InitializeParam();

	//	�����V�[���o�^

	MainFrame->ChangeScene( new SceneSelect() );


	//Lua�̃L�[�쐬
	CreateLuaState();

	//�Q�[���̒萔�p�����[�^�[��ǂݍ���
	MyAssert(LoadLuaGameParam("DATA\\ScriptFile\\GameSystemConstParam.txt"), "�L�����N�^�̒萔�p�����[�^�[�ǂݍ��݂Ɏ��s���܂����B");

	//�L�����N�^�̒萔�p�����[�^�[��ǂݍ���
	MyAssert(LoadLuaCharacterParam("DATA\\ScriptFile\\CharacterConstParam.txt"), "�L�����N�^�̒萔�p�����[�^�[�ǂݍ��݂Ɏ��s���܂����B");

	//�T�E���h�ǂݍ���
	MyAssert(LoadLuaMusic("DATA\\ScriptFile\\SEBGM.txt"), "�T�E���h�ǂݍ��݂Ɏ��s���܂����B");


	return TRUE;
}

//*****************************************************************************************************************************
//		�E�B���h�E�v���V�[�W��
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
//		�E�B���h�E�쐬
//*****************************************************************************************************************************
HWND InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	HWND			hWnd;

	//	�X�N���[���T�C�Y�擾
	RECT	WindowSize;
	iexSystem::GetScreenRect( ScreenMode, WindowSize );

	//	�E�B���h�E�N���X�ݒ�
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
	wc.lpszClassName = "�X�p�[�N�����O�E�H�[�^�[�K��";
	RegisterClass(&wc);

	//	�E�B���h�E�쐬
	if( !bFullScreen ){
		AdjustWindowRect( &WindowSize, WS_OVERLAPPEDWINDOW, FALSE );
		hWnd = CreateWindow( "�X�p�[�N�����O�E�H�[�^�[�K��", AppTitle, WS_OVERLAPPEDWINDOW, 
								0, 0, WindowSize.right-WindowSize.left, WindowSize.bottom-WindowSize.top,
								NULL, NULL, hInstance, NULL);
	} else {
		hWnd = CreateWindow( "�X�p�[�N�����O�E�H�[�^�[�K��", AppTitle, WS_POPUP, 0, 0, WindowSize.right,WindowSize.bottom, NULL, NULL, hInstance, NULL);
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
		//	���C�����[�v
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
		MessageBoxPlus(" �G���[���������܂���\n�@%s \n�^�C�v�ԍ� %d ", err_data.GetStr(), err_data.GetErrData());
	}

	//	�S���	
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
