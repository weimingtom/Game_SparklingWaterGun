
#ifndef __LUA_SCRIPT_H__
#define __LUA_SCRIPT_H__

#include<string>
#include<map>
#include<Windows.h>
#include"../ItDebug.h"

#ifdef _USE_LUA_LIB
#include <lua.hpp>
#endif

using namespace std;
//*********************************************************
//		Luaスクリプト
//*********************************************************
struct lua_State;
//*********************************************************
//		デバッグウィンドウ(Alt+2で表示)
//*********************************************************
#define PrintDebugWindow(str,...) MyDebugString(str,__VA_ARGS__)

//*********************************************************
//		LUAステートのキー
//*********************************************************
typedef int LUA_STATE_KEY;

//*********************************************************
/*	　~説明~
	テキストファイルを解析して再コンパイルせずにプログラムの内容を変更することが
	できます。OpenFile関数にファイル名とパスを指定することでファイルを開きます。
	ファイルを開いた状態で Get～ 関数を呼ぶことでその変数をファイルから読み込みます
	一つのルアステートでファイルを複数開くことはできませんが、
	ChangeState関数を使うことで複数のルアステートを作成することができます。
	利用終了時はLUA_Releaseマクロを呼び出してください。(利用していない状態でも呼んでOK)
*/
//*********************************************************

//*********************************************************
//		Luaスクリプト管理クラス
//*********************************************************
class LuaManager
{
	typedef lua_State*LPLUA_STATE;
	typedef map<LUA_STATE_KEY,LPLUA_STATE>LUA_STATE_MAP;
private:
	//シングルトンの実体
	static LuaManager*			m_pInstance;
	//ルアステート(luaをするには必ず必要)
	LUA_STATE_MAP			m_luaState;
	//現在のルアステートとそのキー
	LUA_STATE_KEY				m_NowLuaState_key;
	LPLUA_STATE					m_pNowLuaState;
private:
	//シングルトンなのでprivateに～
	LuaManager();
	LuaManager(const LuaManager&);
	LuaManager&operator=(const LuaManager&);
	~LuaManager();
public:
	//実体のゲッター
	static LuaManager& GetInstance();

	//クラスの利用を終了する(プログラム終了の際に必ず呼び出してください)
	static void Release();
public:

	//ルアステートを切り替えます(存在しないキーだった場合は切り替えない)
	//デフォルトでは0番のステートを使っています
	BOOL ChangeState(LUA_STATE_KEY key);

	//指定したキーに新たにルアステートを作成する
	//(指定したキーに既にあった場合はなにもしない)
	BOOL CreateNewState(LUA_STATE_KEY key);

	//Luaステートへのポインタを得ます
	//luaライブラリの関数を使いたいときにどうぞ
	inline LPLUA_STATE State()const{
		return m_pNowLuaState;
	}
};

//*********************************************************
//		便利関数
//*********************************************************
namespace lua_func
{
	//指定したファイルを開きます
	BOOL		OpenFile	(const char*filename);

	//現在の設定しているステートを使い指定した名前の変数の値を所得します
	//(指定した名前の定数が存在しなかった場合,またはファイルを開いていない状態で
	//関数を呼び出した場合は0を返却しデバッグウィンドウに表示します)
	int			Get_int			(const char*name);
	bool		Get_bool		(const char*name);
	float		Get_float		(const char*name);
	double		Get_double		(const char*name);
	string		Get_string		(const char*name);
	DWORD		Get_DWORD		(const char*name);

	//現在のキーのLuaスタックの内容を出力ウィンドウに表示します
	void		PrintStack();
};
//*********************************************************
//		簡易呼び出しマクロ
//*********************************************************
//利用終了
#define LUA_Release (LuaManager::Release())
//実体呼び出し
#define LUA (LuaManager::GetInstance())


#endif