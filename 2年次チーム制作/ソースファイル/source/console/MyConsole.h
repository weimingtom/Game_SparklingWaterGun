#pragma once

#define ID_MYTIMER 565656

#define NEW_LINE "\r\n"

#include	<string>
#include	<list>
#include	"WindowClass.h"
typedef std::string STRING;



struct DRAW_TEXT
{
	STRING str;
};


typedef std::list<DRAW_TEXT> DRAW_TEXT_LIST;

class MyConsole :public CSWindowClass
{
private:
	FILE* m_log_fp;//�t�@�C���o�͗p
	DRAW_TEXT_LIST text_list;//�`��e�L�X�g
	HWND m_hEdit;
	DWORD dwMyMask;
public:
	MyConsole();
	~MyConsole();
public:
	// �E�B���h�E�쐬���b�Z�[�W
	BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
	// �E�B���h�E�j�����b�Z�[�W
	void OnDestroy(HWND hWnd);
	// �E�B���h�E�`�惁�b�Z�[�W
	void OnPaint(HWND hWnd);
public:
	//�ϒ�������string�ɕϊ�
	STRING FormatString(const char* format, ...);
	//�e�L�X�g�\��
	void ShowText( STRING str);
	//�t�@�C���ɕۑ�
	void OutputLog(const char* file);
	void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy);

};

//�����o�֐����ƌĂԂ̂���ςȂ̂�
STRING FormatString(const char* format, ...);



