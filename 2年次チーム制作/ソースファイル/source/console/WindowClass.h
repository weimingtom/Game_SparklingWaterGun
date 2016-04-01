	//---------------------------------------//
	//	�E�B���h�E�N���X
	//
	//	�쐬�ҁFDaiki Terai
	//	�쐬���F2010/11/29
	//	�C�������F
	//---------------------------------------//

#pragma once
#include	<tchar.h>
	static const TCHAR* WC_BASIC = _T("Basic");		// ��{�E�B���h�E�N���X��

	// �E�B���h�E�N���X
	class CSWindowClass
	{
	private:
		static HINSTANCE		m_hInstance;	// �C���X�^���X�n���h��
		HWND					m_hWnd;			// �E�B���h�E�n���h��
		static	CSWindowClass*	m_pThis;		// �o�^�p

	public:
		// �R���X�g���N�^
		CSWindowClass();
		// �f�X�g���N�^
		virtual ~CSWindowClass();

		// �V���v���E�B���h�E�N���X�̓o�^
		static bool RegistBasicWindowClass(); 
		
		//�E�B���h�E�N���X�o�^
		static bool RegisterWindowClass(LPCSTR class_name);

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
		virtual bool CreateWnd(
			const TCHAR* Title,int Width = CW_USEDEFAULT,int Height = CW_USEDEFAULT,int X = CW_USEDEFAULT,int Y = CW_USEDEFAULT,
			HWND hParentWnd = NULL,HMENU hMenu = NULL,const TCHAR* ClassName = WC_BASIC,DWORD Style = WS_OVERLAPPEDWINDOW | WS_VISIBLE,DWORD ExStyle = 0);

	private:
		// �E�B���h�E�v���V�[�W���̊Ǘ���
		static LRESULT CALLBACK WndProcManager(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
		// �E�B���h�E�v���V�[�W��
		virtual LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

	public:

		// ���b�Z�[�W���[�v
		static void MsgLoop(int ArrayLength = 0,CSWindowClass* pWindowArray = NULL);

		// ���b�Z�[�W�������Ă��Ȃ��Ƃ��ɂ��鏈��
		virtual void Idle();
		// �E�B���h�E�쐬���b�Z�[�W
		virtual BOOL OnCreate(HWND hWnd,LPCREATESTRUCT lpCreateStruct);
		// �E�B���h�E�j�����b�Z�[�W
		virtual void OnDestroy(HWND hWnd);
		// �E�B���h�E�`�惁�b�Z�[�W
		virtual void OnPaint(HWND hWnd);
		// �L�[���͊֌W���b�Z�[�W
		virtual void OnChar(HWND hWnd, TCHAR ch, int cRepeat);
		virtual void OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
		// �}�E�X�֘A���b�Z�[�W
		virtual void OnLButtonDown(HWND hWnd, BOOL isDoubleClick, int x, int y, UINT keyFlags);
		virtual void OnRButtonDown(HWND hWnd, BOOL isDoubleClick, int x, int y, UINT keyFlags);
		virtual void OnMButtonDown(HWND hWnd, BOOL isDoubleClick, int x, int y, UINT keyFlags);
		virtual void OnLButtonUp(HWND hWnd,int x, int y, UINT keyFlags);
		virtual void OnRButtonUp(HWND hWnd,int x, int y, UINT keyFlags);
		virtual void OnMButtonUp(HWND hWnd,int x, int y, UINT keyFlags);
		virtual void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
		virtual void OnMouseWheel(HWND hWnd, int xPos, int yPos, int zDelta, UINT fwKeys);
		//�^�C�}�[���b�Z�[�W
		virtual void OnTimer(HWND hWnd);
		///	saizuhenkou 
		virtual void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy);
};
