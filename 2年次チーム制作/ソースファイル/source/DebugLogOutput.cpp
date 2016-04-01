#include "../Win32/stdafx.h"

#include "DebugLogOutput.h"


//スレッド用ラッパー関数
unsigned int _stdcall ThreadDebugLogOutput(void* owner)
{
	DebugLogOutput* p = (DebugLogOutput*)owner;

	return p->Loop();
}


//時間取得
void GetTimeStr(LPSTR out)
{
	char time_str[256];

	time_t long_time;
	struct tm *now_time;
	time(&long_time);
	now_time = localtime(&long_time);
	sprintf_s(
		time_str,
		"%04d年 %02d月%02d日 %02d時%02d分%02d秒\n",
		now_time->tm_year + 1900,
		now_time->tm_mon + 1,
		now_time->tm_mday + 1,
		now_time->tm_hour,
		now_time->tm_min,
		now_time->tm_sec);

	int len = strnlen_s(time_str, MAX_PATH);
	for (int i = 0; i < len; i++)
	{
		out[i] = time_str[i];
	}
	out[len] = 0;
}


const int DebugLogOutput::line_size = 20;

DebugLogOutput::DebugLogOutput(STRING name) :
hThread(nullptr),
m_name(name),
draw(0),
max_line(0),
lines(1),
page_size(1)
{
	//スレッド開始
	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadDebugLogOutput, this, 0, NULL);
}

DebugLogOutput::~DebugLogOutput()
{
	DestroyWindow(m_hWnd);
}


//メッセージループ
unsigned int _stdcall DebugLogOutput::Loop()
{
	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}


BOOL DebugLogOutput::OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	//描画できる最大行数決定
	RECT rc;
	GetClientRect(hWnd, &rc);
	max_line = rc.bottom / line_size;

	if (SetTimer(hWnd, ID_MYTIMER, 1000, NULL) == 0) {
		MessageBox(hWnd,
			(LPCSTR)"タイマー失敗！",
			(LPCSTR)"失敗",
			MB_OK | MB_ICONEXCLAMATION);
	}

	//テキスト表示
	char str[MAX_PATH];
	GetTimeStr(str);

	DRAW_TEXT text;
	//作成日時
	text.str = "作成日時\n" + STRING(str);
	//描画色
	text.color = color::BLACK;
	//描画
	ShowText(text);

	return TRUE;
}

// ウィンドウ破棄メッセージ
void DebugLogOutput::OnDestroy(HWND hWnd)
{
	//終了
	PostQuitMessage(0);

	//スレッド終了
	MsgWaitForMultipleObjectsEx(1, &hThread, INFINITE, QS_ALLINPUT, 0);
	CloseHandle(hThread);
	
}


// ウィンドウ描画メッセージ
void DebugLogOutput::OnPaint(HWND hWnd)
{
	//描画開始
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);

	//作業用変数
	size_t index = (size_t)0;
	size_t length = (size_t)0;

	//コピー
	DRAW_TEXT work = next_draw_text;

	int line = 0;//送られてきた行数

	//改行
	while (1){
		//改行検索
		length = next_draw_text.str.find('\n', index);

		if (length == STRING::npos)
		{
			if (line == 0 && work.str != "")
			{
				//配列に追加
				text_array.push_back(work);
			}
			else break;
		}

		if (index + 1 >= next_draw_text.str.size())break;

		//改行コードまでの文字列取得
		work.str = next_draw_text.str.substr(index,length-index);

		//配列に追加
		text_array.push_back(work);

		//行数更新
		line++;

		index = length+1;//index更新
	}

	//次は空文字列
	next_draw_text.str = "";

	int size = text_array.size();
	for (int i = 0; i < max_line && i < size; i++)
	{
		if (draw + i >= size)break;

		//現在の位置の文字列取得
		work = text_array.at(draw + i);

		//描画色設定
		SetTextColor(hdc, COLORREF(work.color));

		//テキスト出力
		TextOut(
			hdc,
			10,
			(i*line_size + 10), 
			work.str.c_str(),
			work.str.size()
			);
	}

	//行数更新
	lines = size;

	//描画終了
	EndPaint(hWnd, &ps);
}



//可変長引数をstringに変換
STRING DebugLogOutput::FormatString(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	char temp[1024];
	vsprintf_s(temp, format, ap);

	STRING ret = temp;

	va_end(ap);

	return ret;
}

void DebugLogOutput::ShowText(DRAW_TEXT text)
{
	//描画データの準備
	next_draw_text = text;

	//ログ
	text_list.push_back(text);

	//再描画処理をさせる
	InvalidateRect(m_hWnd, NULL, FALSE);
	UpdateWindow(m_hWnd);
}

void DebugLogOutput::ShowText(STRING str, DWORD color)
{
	DRAW_TEXT temp;
	temp.str = str;
	temp.color = color;

	ShowText(temp);
}

void DebugLogOutput::OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	STRING str = m_name;
	str += "に書き込みました！";

	switch (vk)
	{
	case VK_F2:
		OutputLog(m_name.c_str());
		MessageBox(hwnd, str.c_str(), "message!", 0);
		break;
	default:
		break;
	}
}

//サイズ変更メッセージ
void DebugLogOutput::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	RECT rc;
	GetClientRect(hwnd, &rc);

	//最大描画行数更新
	max_line = rc.bottom / line_size;
	page_size = max_line;

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = lines + 1;
	si.nPage = page_size;
	si.nPos = draw;
	SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

// マウス関連メッセージ
void DebugLogOutput::OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{
	int dy = 0;

	dy = -zDelta / (line_size*3);

	if (si.nMax > si.nPage){

		dy = max(-1 * si.nPos, min(dy, si.nMax - si.nPos));
		if (dy != 0) {
			si.nPos += dy;
			draw = si.nPos;
			SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
			ScrollWindow(hwnd, 0, -dy * line_size, NULL, NULL);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
		}
	}
}

// スクロールメッセージ
void DebugLogOutput::Cls_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	int dy = 0;
	switch (code)
	{
	case SB_LINEUP:
		dy = -1;
		break;
	case SB_LINEDOWN:
		dy = 1;
		break;
	case SB_PAGEUP:
		dy = -1*page_size;
		break;
	case SB_PAGEDOWN:
		dy = page_size;
		break;
	case SB_THUMBTRACK:
		dy = (pos - page_size);
		break;
	default:
		break;
	}

	dy = max(-1 * si.nPos, min(dy, si.nMax - si.nPos));
	if (dy != 0) {
		si.nPos += dy;
		draw = si.nPos;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		ScrollWindow(hwnd, 0, -dy * line_size, NULL, NULL);
		InvalidateRect(hwnd, NULL, TRUE);
		UpdateWindow(hwnd);
	}
}

void DebugLogOutput::OutputLog(const char* file)
{
	//ファイルハンドル作成
	HANDLE hFile = nullptr;
	hFile = CreateFile(m_name.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	//ファイルサイズ取得
	DWORD dwAccBytes;
	//ファイルサイズ取得
	GetFileSize(hFile, &dwAccBytes);

	int size = 0;

	STRING str;

	for (DRAW_TEXT_ARRAY::iterator it = text_array.begin();
		it != text_array.end();
		it++)
	{
		str = (*it).str + "\r\n";

		//バッファサイズ取得
		size = str.size();

		//ファイルに出力
		WriteFile(hFile, str.c_str(), size, &dwAccBytes, NULL);
	}

	//ハンドル破棄
	CloseHandle(hFile);
}
