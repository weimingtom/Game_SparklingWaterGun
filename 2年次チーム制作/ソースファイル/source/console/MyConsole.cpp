#include	<time.h>
#include	<Windows.h>
#include	<WindowsX.h>
#include	<tchar.h>
#include	"WindowClass.h"
#include "MyConsole.h"

#ifdef _DEBUG
#define MyDebugEnable 
#endif

void GetTimeStr(LPSTR out)
{
#ifdef MyDebugEnable
	char time_str[MAX_PATH];

	time_t long_time;
	struct tm *now_time;
	time(&long_time);
	now_time = localtime(&long_time);
	sprintf_s<MAX_PATH>(
		time_str,
		"%04d�N %02d��%02d�� %02d��%02d��%02d�b\r\n",
		now_time->tm_year+1900,
		now_time->tm_mon +1 ,
		now_time->tm_mday+1,
		now_time->tm_hour,
		now_time->tm_min,
		now_time->tm_sec);

	int len = strnlen_s(time_str, MAX_PATH);
	for (int i = 0; i < len; i++)
	{
		out[i] = time_str[i];
	}
	out[len] = 0;
#else 
	out[0] = 0;
#endif
}

MyConsole::MyConsole() :
m_log_fp(nullptr)
{

}

MyConsole::~MyConsole()
{

}


// �E�B���h�E�쐬���b�Z�[�W
BOOL MyConsole::OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
#ifdef MyDebugEnable
	if (SetTimer(hWnd, ID_MYTIMER, 1000, NULL) == 0) {
		MessageBox(hWnd,
			(LPCSTR)"�^�C�}�[���s�I",
			(LPCSTR)"���s",
			MB_OK | MB_ICONEXCLAMATION);
	}

	RECT rc;
	GetClientRect(hWnd, &rc);

	//�G�f�B�b�g�R���g���[���쐬
	m_hEdit = CreateWindow(
		"EDIT",
		NULL,
		WS_CHILD | WS_VISIBLE |
		ES_WANTRETURN | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL |
		ES_AUTOHSCROLL | WS_HSCROLL,
		0, 0,
		rc.right, rc.bottom,
		hWnd,
		(HMENU)NULL,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	
	//���͋֎~!
	SendMessage(m_hEdit, EM_SETLIMITTEXT, (WPARAM)0, 0);
	//�e�L�X�g�\��
	char str[MAX_PATH];
	GetTimeStr(str);

	STRING temp1(str), temp2("�쐬����\r\n");

	DRAW_TEXT dt;
	dt.str = temp2 + temp1;
	text_list.push_back(dt);

	Edit_SetText(m_hEdit, dt.str.c_str());

	Edit_SetReadOnly(m_hEdit, TRUE);

	return TRUE;
#else 
	return FALSE;
#endif
}

// �E�B���h�E�j�����b�Z�[�W
void MyConsole::OnDestroy(HWND hWnd)
{
#ifdef MyDebugEnable
	//�^�C�}�[�؂�
	KillTimer(hWnd, ID_MYTIMER);
	//�G�f�B�b�g�R���g���[���j��
	SendMessage(m_hEdit, WM_DESTROY, NULL, NULL);
#endif
}

// �E�B���h�E�`�惁�b�Z�[�W
void MyConsole::OnPaint(HWND hWnd)
{
#ifdef MyDebugEnable
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	RECT rc;
	GetClientRect(hWnd, &rc);

	for (DRAW_TEXT_LIST::iterator it = text_list.begin();
		it != text_list.end();
		it++)
	{
		Edit_SetText(m_hEdit, (*it).str.c_str());
	}

	EndPaint(hWnd, &ps);
#endif
}


//�ϒ�������string�ɕϊ�
STRING MyConsole::FormatString(const char* format, ...)
{
#ifdef MyDebugEnable
	va_list ap;
	va_start(ap, format);

	char temp[1024];
	vsprintf_s(temp, format, ap);

	STRING ret = temp;

	va_end(ap);

	return ret;
#else
	return STRING();
#endif
}

//�e�L�X�g�\��
void MyConsole::ShowText( STRING str)
{
#ifdef MyDebugEnable
	DRAW_TEXT temp;
	temp.str = str;

	text_list.push_back(temp);
	
	STRING string = "";

	for (DRAW_TEXT_LIST::iterator it = text_list.begin();
		it != text_list.end();
		it++)
	{
		string += (*it).str;
	}
	Edit_SetText(m_hEdit, string.c_str());

#endif
}


void MyConsole::OutputLog(const char* file)
{
#ifdef MyDebugEnable

	HANDLE hFile = NULL;
	hFile = CreateFile(file, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
//	if (hFile == INVALID_HANDLE_VALUE) return;

	SetFilePointer(hFile, 0, 0, FILE_END);

	STRING str;
	for (DRAW_TEXT_LIST::iterator it = text_list.begin();
		it != text_list.end();
		it++)
	{
		str += (*it).str;
	}
	int size = str.size();
	DWORD dwAccBytes;
	GetFileSize(hFile, &dwAccBytes);
	WriteFile(hFile, str.c_str(), size, &dwAccBytes, NULL);

	CloseHandle(hFile);
#endif
}

void MyConsole::Cls_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
#ifdef MyDebugEnable
	RECT rc;

	GetClientRect(hwnd, &rc);

	SetWindowPos(m_hEdit, NULL, rc.left, rc.top, cx, cy, 0);
#endif
}

STRING FormatString(const char* format, ...)
{
#ifdef MyDebugEnable
	//va_list ap;
	//va_start(ap, format);

	char temp[1024];
	//vsprintf_s<1024>(temp, format, ap);

	STRING ret = temp;

	//va_end(ap);

	return ret;

#else
	return STRING();
#endif
}