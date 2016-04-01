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
	FILE* m_log_fp;//ファイル出力用
	DRAW_TEXT_LIST text_list;//描画テキスト
	HWND m_hEdit;
	DWORD dwMyMask;
public:
	MyConsole();
	~MyConsole();
public:
	// ウィンドウ作成メッセージ
	BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
	// ウィンドウ破棄メッセージ
	void OnDestroy(HWND hWnd);
	// ウィンドウ描画メッセージ
	void OnPaint(HWND hWnd);
public:
	//可変長引数をstringに変換
	STRING FormatString(const char* format, ...);
	//テキスト表示
	void ShowText( STRING str);
	//ファイルに保存
	void OutputLog(const char* file);
	void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy);

};

//メンバ関数だと呼ぶのが大変なので
STRING FormatString(const char* format, ...);



