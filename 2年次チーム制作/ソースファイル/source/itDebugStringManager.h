#ifndef __IT_DEBUG_STRING_MANAGER_H__
#define __IT_DEBUG_STRING_MANAGER_H__

#include <list>
#include <string>

//簡易呼び出し
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
//	デバッグ用文字表示クラス
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

	void Clear();						//現在の文字列データを破棄
	void SetDebugStr(const char *str, DWORD color);	//文字列をセット
	void NextLine();					//次の行に移動する
	void Render();						//現在のデータを描画
};

#define ITDEBUGSTRMGR (itDebugStringManager::GetInstance())

#endif