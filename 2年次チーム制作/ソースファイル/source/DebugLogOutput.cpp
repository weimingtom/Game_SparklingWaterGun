#include "../Win32/stdafx.h"

#include "DebugLogOutput.h"


//�X���b�h�p���b�p�[�֐�
unsigned int _stdcall ThreadDebugLogOutput(void* owner)
{
	DebugLogOutput* p = (DebugLogOutput*)owner;

	return p->Loop();
}


//���Ԏ擾
void GetTimeStr(LPSTR out)
{
	char time_str[256];

	time_t long_time;
	struct tm *now_time;
	time(&long_time);
	now_time = localtime(&long_time);
	sprintf_s(
		time_str,
		"%04d�N %02d��%02d�� %02d��%02d��%02d�b\n",
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
	//�X���b�h�J�n
	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadDebugLogOutput, this, 0, NULL);
}

DebugLogOutput::~DebugLogOutput()
{
	DestroyWindow(m_hWnd);
}


//���b�Z�[�W���[�v
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
	//�`��ł���ő�s������
	RECT rc;
	GetClientRect(hWnd, &rc);
	max_line = rc.bottom / line_size;

	if (SetTimer(hWnd, ID_MYTIMER, 1000, NULL) == 0) {
		MessageBox(hWnd,
			(LPCSTR)"�^�C�}�[���s�I",
			(LPCSTR)"���s",
			MB_OK | MB_ICONEXCLAMATION);
	}

	//�e�L�X�g�\��
	char str[MAX_PATH];
	GetTimeStr(str);

	DRAW_TEXT text;
	//�쐬����
	text.str = "�쐬����\n" + STRING(str);
	//�`��F
	text.color = color::BLACK;
	//�`��
	ShowText(text);

	return TRUE;
}

// �E�B���h�E�j�����b�Z�[�W
void DebugLogOutput::OnDestroy(HWND hWnd)
{
	//�I��
	PostQuitMessage(0);

	//�X���b�h�I��
	MsgWaitForMultipleObjectsEx(1, &hThread, INFINITE, QS_ALLINPUT, 0);
	CloseHandle(hThread);
	
}


// �E�B���h�E�`�惁�b�Z�[�W
void DebugLogOutput::OnPaint(HWND hWnd)
{
	//�`��J�n
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);

	//��Ɨp�ϐ�
	size_t index = (size_t)0;
	size_t length = (size_t)0;

	//�R�s�[
	DRAW_TEXT work = next_draw_text;

	int line = 0;//�����Ă����s��

	//���s
	while (1){
		//���s����
		length = next_draw_text.str.find('\n', index);

		if (length == STRING::npos)
		{
			if (line == 0 && work.str != "")
			{
				//�z��ɒǉ�
				text_array.push_back(work);
			}
			else break;
		}

		if (index + 1 >= next_draw_text.str.size())break;

		//���s�R�[�h�܂ł̕�����擾
		work.str = next_draw_text.str.substr(index,length-index);

		//�z��ɒǉ�
		text_array.push_back(work);

		//�s���X�V
		line++;

		index = length+1;//index�X�V
	}

	//���͋󕶎���
	next_draw_text.str = "";

	int size = text_array.size();
	for (int i = 0; i < max_line && i < size; i++)
	{
		if (draw + i >= size)break;

		//���݂̈ʒu�̕�����擾
		work = text_array.at(draw + i);

		//�`��F�ݒ�
		SetTextColor(hdc, COLORREF(work.color));

		//�e�L�X�g�o��
		TextOut(
			hdc,
			10,
			(i*line_size + 10), 
			work.str.c_str(),
			work.str.size()
			);
	}

	//�s���X�V
	lines = size;

	//�`��I��
	EndPaint(hWnd, &ps);
}



//�ϒ�������string�ɕϊ�
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
	//�`��f�[�^�̏���
	next_draw_text = text;

	//���O
	text_list.push_back(text);

	//�ĕ`�揈����������
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
	str += "�ɏ������݂܂����I";

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

//�T�C�Y�ύX���b�Z�[�W
void DebugLogOutput::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	RECT rc;
	GetClientRect(hwnd, &rc);

	//�ő�`��s���X�V
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

// �}�E�X�֘A���b�Z�[�W
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

// �X�N���[�����b�Z�[�W
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
	//�t�@�C���n���h���쐬
	HANDLE hFile = nullptr;
	hFile = CreateFile(m_name.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	//�t�@�C���T�C�Y�擾
	DWORD dwAccBytes;
	//�t�@�C���T�C�Y�擾
	GetFileSize(hFile, &dwAccBytes);

	int size = 0;

	STRING str;

	for (DRAW_TEXT_ARRAY::iterator it = text_array.begin();
		it != text_array.end();
		it++)
	{
		str = (*it).str + "\r\n";

		//�o�b�t�@�T�C�Y�擾
		size = str.size();

		//�t�@�C���ɏo��
		WriteFile(hFile, str.c_str(), size, &dwAccBytes, NULL);
	}

	//�n���h���j��
	CloseHandle(hFile);
}
