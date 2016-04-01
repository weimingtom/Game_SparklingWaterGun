#pragma once

#define ID_MYTIMER 565656

typedef std::string STRING;

struct DRAW_TEXT
{
	STRING str;//描画する文字列
	DWORD color;//描画色
};

typedef std::list<DRAW_TEXT> DRAW_TEXT_LIST;
typedef std::vector<DRAW_TEXT> DRAW_TEXT_ARRAY;

class DebugLogOutput :public CSWindowClass
{
	DRAW_TEXT_LIST text_list;				//ログ
	DRAW_TEXT next_draw_text;				//次に描画するテキスト情報
	DRAW_TEXT_ARRAY text_array;			//描画用テキスト配列
	HANDLE hThread;								//スレッドハンドル
	const STRING m_name;						//ファイル名

	static const int line_size;						//一行のサイズ
	int draw;												//描画している場所(行目)
	int page_size;										//1ページのサイズ
	int lines;												//現在何行あるか
	int max_line;										//描画できる最大行数
	SCROLLINFO si;
public:
	DebugLogOutput(STRING name);
	~DebugLogOutput();
public:
	// ウィンドウ作成メッセージ
	BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
	// ウィンドウ破棄メッセージ
	void OnDestroy(HWND hWnd);
	// ウィンドウ描画メッセージ
	void OnPaint(HWND hWnd);
	// キー入力関係メッセージ
	void OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
	//サイズ変更メッセージ
	void OnSize(HWND hwnd, UINT state, int cx, int cy);
	// マウス関連メッセージ
	void OnMouseWheel(HWND hWnd, int xPos, int yPos, int zDelta, UINT fwKeys);
	// スクロールメッセージ
	void Cls_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
public:
	//可変長引数をstringに変換
	static STRING FormatString(const char* format, ...);
	//テキスト表示
	void ShowText(DRAW_TEXT text);
	void ShowText(STRING str, DWORD color);
	//メッセージループ
	unsigned int _stdcall Loop();
private:
	//ファイルに保存
	void OutputLog(const char* file);
};

