#ifndef __IT_DEBUG_STRING_MANAGER_H__
#define __IT_DEBUG_STRING_MANAGER_H__

#include <list>
#include <string>

//�ȈՌĂяo��
#if 0
#define MyDebugMgrStr(color,str,...)\
	{\
	char t[1000]; \
	sprintf_s<1000>(t, str, __VA_ARGS__); \
	itDebugStringManager::GetInstance().SetDebugStr(t, color); \
	itDebugStringManager::GetInstance().NextLine(); \
	}
#else
#define MyDebugMgrStr(color,str, ...)
#endif

#if 1
#define MyDebugMgrStr2(color,str,...)\
	{\
	char t[1000]; \
	sprintf_s<1000>(t, str, __VA_ARGS__); \
	itDebugStringManager::GetInstance().SetDebugStr(t, color); \
	itDebugStringManager::GetInstance().NextLine(); \
	}
#else
#define MyDebugMgrStr(color,str, ...)
#endif

//*****************************************************
//	�f�o�b�O�p�����\���N���X
//*****************************************************
struct DEBUGSTRING
{
	std::string buffer;
	DWORD color;
	DEBUGSTRING(const char* buffer, const DWORD& color) :
		buffer(buffer), color(color){}
};

class itDebugStringManager
{
	typedef std::list<DEBUGSTRING> DEBUGSTRINGLIST;
private:
	DEBUGSTRINGLIST m_data;
private:
	itDebugStringManager();
	~itDebugStringManager();
public:
	static itDebugStringManager& GetInstance();

	void Clear();						//���݂̕�����f�[�^��j��
	void SetDebugStr(const char *str, DWORD color);	//��������Z�b�g
	void NextLine();					//���̍s�Ɉړ�����
	void Render();						//���݂̃f�[�^��`��
};

#define ITDEBUGSTRMGR (itDebugStringManager::GetInstance())

#endif