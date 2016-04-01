#pragma once

#define ID_MYTIMER 565656

typedef std::string STRING;

struct DRAW_TEXT
{
	STRING str;//�`�悷�镶����
	DWORD color;//�`��F
};

typedef std::list<DRAW_TEXT> DRAW_TEXT_LIST;
typedef std::vector<DRAW_TEXT> DRAW_TEXT_ARRAY;

class DebugLogOutput :public CSWindowClass
{
	DRAW_TEXT_LIST text_list;				//���O
	DRAW_TEXT next_draw_text;				//���ɕ`�悷��e�L�X�g���
	DRAW_TEXT_ARRAY text_array;			//�`��p�e�L�X�g�z��
	HANDLE hThread;								//�X���b�h�n���h��
	const STRING m_name;						//�t�@�C����

	static const int line_size;						//��s�̃T�C�Y
	int draw;												//�`�悵�Ă���ꏊ(�s��)
	int page_size;										//1�y�[�W�̃T�C�Y
	int lines;												//���݉��s���邩
	int max_line;										//�`��ł���ő�s��
	SCROLLINFO si;
public:
	DebugLogOutput(STRING name);
	~DebugLogOutput();
public:
	// �E�B���h�E�쐬���b�Z�[�W
	BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
	// �E�B���h�E�j�����b�Z�[�W
	void OnDestroy(HWND hWnd);
	// �E�B���h�E�`�惁�b�Z�[�W
	void OnPaint(HWND hWnd);
	// �L�[���͊֌W���b�Z�[�W
	void OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
	//�T�C�Y�ύX���b�Z�[�W
	void OnSize(HWND hwnd, UINT state, int cx, int cy);
	// �}�E�X�֘A���b�Z�[�W
	void OnMouseWheel(HWND hWnd, int xPos, int yPos, int zDelta, UINT fwKeys);
	// �X�N���[�����b�Z�[�W
	void Cls_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
public:
	//�ϒ�������string�ɕϊ�
	static STRING FormatString(const char* format, ...);
	//�e�L�X�g�\��
	void ShowText(DRAW_TEXT text);
	void ShowText(STRING str, DWORD color);
	//���b�Z�[�W���[�v
	unsigned int _stdcall Loop();
private:
	//�t�@�C���ɕۑ�
	void OutputLog(const char* file);
};

