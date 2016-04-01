#include	"../All.h"
#include	<WindowsX.h>
	// �C���X�^���X�n���h��
	HINSTANCE CSWindowClass::m_hInstance = NULL;
	// �o�^�p
	CSWindowClass* CSWindowClass::m_pThis = NULL;		

	// �R���X�g���N�^
	CSWindowClass::CSWindowClass()
	:m_hWnd(NULL)
	{
	}

	// �f�X�g���N�^
	CSWindowClass::~CSWindowClass(){
	}

	// �V���v���E�B���h�E�N���X�̓o�^
	bool CSWindowClass::RegistBasicWindowClass(){ 

		m_hInstance = GetModuleHandle(NULL);

		/********************************************************/
		/*		�E�B���h�E�N���X�̒�`�ƍ쐬					*/
		/********************************************************/

		// �V���v���E�B���h�E�N���X�ݒ�
		WNDCLASSEX wcex ={sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, WndProcManager, 0,0,m_hInstance,
			(HICON)LoadImage(NULL,MAKEINTRESOURCE(IDI_APPLICATION),IMAGE_ICON,0,0,LR_DEFAULTSIZE | LR_SHARED),
			(HCURSOR)LoadImage(NULL,MAKEINTRESOURCE(IDC_ARROW),IMAGE_CURSOR,0,0,LR_DEFAULTSIZE | LR_SHARED), 
			(HBRUSH)GetStockObject(WHITE_BRUSH), NULL, WC_BASIC , NULL};

		// �V���v���E�B���h�E�N���X�쐬
		if(!RegisterClassEx(&wcex)) 
			return false;

		return true;
	}

	//�E�B���h�E�N���X�o�^
	bool CSWindowClass::RegisterWindowClass(LPCSTR class_name)
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, WndProcManager, 0, 0, m_hInstance,
			(HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
			(HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED),
			(HBRUSH)GetStockObject(WHITE_BRUSH), NULL, class_name, NULL };

		// �E�B���h�E�N���X�쐬
		if (!RegisterClassEx(&wcex))
			return false;

		return true;
	}

	//--------------------------------------------------------------------//
	//	�E�B���h�E�쐬
	//
	//	�����F		Title		�^�C�g��
	//				Width		��
	//				Height		����
	//				X			X���W
	//				Y			Y���W
	//				hParentWnd	�e�E�B���h�E�n���h��
	//				hMenu		���j���[�n���h���������̓��\�[�XID
	//				ClassName	�E�B���h�E�N���X��
	//				Style		�E�B���h�E�X�^�C��
	//				ExStyle		�E�B���h�E�g���X�^�C��
	//
	//	�߂�l�F	true:	�쐬����
	//				false:	�쐬���s
	//--------------------------------------------------------------------//
	bool CSWindowClass::CreateWnd(
								  const TCHAR* Title,int Width,int Height,int X ,int Y ,
								  HWND hParentWnd,HMENU hMenu,const TCHAR* ClassName,DWORD Style,DWORD ExStyle)
	{
		// �d�v�F���ƂŐ������E�B���h�E�v���V�[�W�����Ăяo�����߂Ɋo���Ă����܂�
		m_pThis = this;

		/********************************************************/
		/*		�E�B���h�E�̍쐬								*/
		/********************************************************/

		m_hWnd = CreateWindowEx(
			ExStyle,							//�g���E�B���h�E�X�^�C��
			ClassName,							//�E�B���h�E�N���X��
			Title,								//�^�C�g���o�[�ɂ��̖��O���\������܂�
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,	//�E�B���h�E�X�^�C��
			X,									//�w���W
			Y,									//�x���W
			Width,								//��
			Height,								//����
			hParentWnd,							//�e�E�B���h�E�̃n���h���A�e�����Ƃ���NULL
			hMenu,								//���j���[�n���h��or���\�[�XID
			m_hInstance,						//�C���X�^���X�n���h��
			NULL);

		// �E�B���h�E�쐬���s
		if (m_hWnd == NULL)
			return false;

		return true;
	}

	//-----------------------------------------------------//
	//  �E�B���h�E�v���V�[�W���̊Ǘ���
	//	��{�E�B���h�E�N���X�ł͂��̊֐���o�^����
	//	���̊֐����o�R���Ċe�E�B���h�E�̖{���̃v���V�[�W�����Ăяo���܂�
	//
	//	�����F	hWnd	�E�B���h�E�n���h��
	//			msg		���b�Z�[�W
	//			wParam	�p�����[�^
	//			lParam	�p�����[�^
	//
	//	�߂�l�F��������
	//-----------------------------------------------------//
	LRESULT CALLBACK CSWindowClass::WndProcManager(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){

		CSWindowClass* pWindow = (CSWindowClass*)(LONG_PTR)GetWindowLong(hWnd,GWL_USERDATA);

		//this�|�C���^���擾�ł����ꍇ
		if( pWindow != NULL)
		{
			// �{���̃E�B���h�E�v���V�[�W�����Ăяo�����b�Z�[�W��������
			LRESULT lResult = pWindow->WndProc( hWnd, msg, wParam, lParam);
			return lResult;
		}
		//this�|�C���^���擾�ł��Ȃ������ꍇ
		else
		{
			// �E�B���h�E�̍쐬���̏ꍇ
			if( msg == WM_CREATE || msg == WM_INITDIALOG){
				// �v���p�e�B���X�g�ɃI�u�W�F�N�g�|�C���^(this�|�C���^)��ݒ肷��
				SetWindowLongPtr( hWnd, GWL_USERDATA, (LONG)(LONG_PTR)(m_pThis) );
				//PRINT("Create %s\n",typeid(*m_Instance).name());// �ǂ̃E�B���h�E�^�C�v�Ȃ̂��\��
				// ���������b�Z�[�W�̍đ��M
				SendMessage(hWnd,msg,wParam,lParam);

				m_pThis = NULL;	// �o�^�����̂ňꎞ�C���X�^���X�|�C���^�i�[�̉���
			}
		}

		return DefWindowProc(hWnd,msg,wParam,lParam);
	}


	//-----------------------------------------------------//
	//	�E�B���h�E�v���V�[�W��
	//	�f�t�H���g�̏�����DefWindowProc�֐��ōs��
	//
	//	�����F	hWnd	�E�B���h�E�n���h��
	//			msg		���b�Z�[�W
	//			wParam	�p�����[�^
	//			lParam	�p�����[�^
	//
	//	�߂�l�F��������
	//-----------------------------------------------------//
	LRESULT CALLBACK CSWindowClass::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

		switch(msg){
			HANDLE_MSG(hWnd,WM_CREATE,OnCreate);
			HANDLE_MSG(hWnd,WM_DESTROY,OnDestroy);
			HANDLE_MSG(hWnd,WM_PAINT,OnPaint);
			// �L�[�{�[�h
			HANDLE_MSG(hWnd,WM_CHAR,OnChar);
			HANDLE_MSG(hWnd, WM_KEYDOWN, OnKey);
			// �}�E�X
			HANDLE_MSG(hWnd,WM_LBUTTONDOWN,OnLButtonDown);
			HANDLE_MSG(hWnd,WM_RBUTTONDOWN,OnRButtonDown);
			HANDLE_MSG(hWnd,WM_MBUTTONDOWN,OnMButtonDown);
			HANDLE_MSG(hWnd,WM_LBUTTONDBLCLK,OnLButtonDown);
			HANDLE_MSG(hWnd,WM_RBUTTONDBLCLK,OnRButtonDown);
			HANDLE_MSG(hWnd,WM_LBUTTONUP,OnLButtonUp);
			HANDLE_MSG(hWnd,WM_RBUTTONUP,OnRButtonUp);
			HANDLE_MSG(hWnd,WM_MBUTTONUP,OnMButtonUp);
			HANDLE_MSG(hWnd,WM_MOUSEMOVE,OnMouseMove);
			HANDLE_MSG(hWnd,WM_MOUSEWHEEL,OnMouseWheel);

			HANDLE_MSG(hWnd, WM_SIZE, Cls_OnSize);

			// MCI�����I���ʒm���b�Z�[�W
		case MM_MCINOTIFY:{ 
				MCI_PLAY_PARMS mciPlay;

				// �ʒm���b�Z�[�W���s
				if (wParam != MCI_NOTIFY_SUCCESSFUL)
					return 0;
				
				// �����ʒu�ɖ߂�
				mciSendCommand((MCIDEVICEID)lParam, MCI_SEEK, MCI_SEEK_TO_START, 0);

				mciPlay.dwCallback = (DWORD_PTR)hWnd;
				// ���[�v�Đ�
				mciSendCommand((MCIDEVICEID)lParam, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mciPlay);

				return 0;
			}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	// ���b�Z�[�W���[�v
	void CSWindowClass::MsgLoop(int ArrayLength,CSWindowClass* pWindowArray){

		/********************************************************/
		/*		���b�Z�[�W���[�v								*/
		/********************************************************/

		MSG msg = {}; 
		while(msg.message != WM_QUIT) {
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else{
				// ���b�Z�[�W���������ĂȂ��Ƃ����鏈��
				for(int i = 0;i < ArrayLength;++i){
					pWindowArray[i].Idle();
				}
			}
		}	    
	}

	// ���b�Z�[�W�������Ă��Ȃ��Ƃ��ɂ��鏈��
	void CSWindowClass::Idle(){
		// �p�����ɏ���������
	}

	// �E�B���h�E�쐬���b�Z�[�W
	BOOL CSWindowClass::OnCreate(HWND hWnd,LPCREATESTRUCT lpCreateStruct){
		return TRUE;
	}

	// �E�B���h�E�j�����b�Z�[�W
	void CSWindowClass::OnDestroy(HWND hWnd){
		PostQuitMessage(0);
	}

	// �E�B���h�E�`�惁�b�Z�[�W
	void CSWindowClass::OnPaint(HWND hWnd){
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd,&ps);
		EndPaint(hWnd,&ps);
	}

	// �L�[���̓��b�Z�[�W
	void CSWindowClass::OnChar(HWND hWnd, TCHAR ch, int cRepeat){}
	void CSWindowClass::OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags){}

	// �}�E�X���{�^���������b�Z�[�W
	void CSWindowClass::OnLButtonDown(HWND hWnd, BOOL isDoubleClick, int x, int y, UINT keyFlags){}
	// �}�E�X�E�{�^���������b�Z�[�W
	void CSWindowClass::OnRButtonDown(HWND hWnd, BOOL isDoubleClick, int x, int y, UINT keyFlags){}
	// �}�E�X�^�񒆃{�^���������b�Z�[�W
	void CSWindowClass::OnMButtonDown(HWND hWnd, BOOL isDoubleClick, int x, int y, UINT keyFlags){}
	// �}�E�X���{�^�����チ�b�Z�[�W
	void CSWindowClass::OnLButtonUp(HWND hWnd,int x, int y, UINT keyFlags){}
	// �}�E�X�E�{�^�����グ���b�Z�[�W
	void CSWindowClass::OnRButtonUp(HWND hWnd,int x, int y, UINT keyFlags){}
	// �}�E�X�^�񒆃{�^�����チ�b�Z�[�W
	void CSWindowClass::OnMButtonUp(HWND hWnd,int x, int y, UINT keyFlags){}
	// �}�E�X�ړ����b�Z�[�W
	void CSWindowClass::OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags){}
	// �}�E�X�z�C�[�����b�Z�[�W
	void CSWindowClass::OnMouseWheel(HWND hWnd, int xPos, int yPos, int zDelta, UINT fwKeys){}
	//�^�C�}�[���b�Z�[�W
	void CSWindowClass::OnTimer(HWND hWnd){}
	void CSWindowClass::Cls_OnSize(HWND hwnd, UINT state, int cx, int cy){}
