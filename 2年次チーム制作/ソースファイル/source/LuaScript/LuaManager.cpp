#include <Windows.h>
#include <lua.hpp>
#include "LuaManager.h"

#pragma comment (lib, "lua5.1.lib")	
using namespace std;
using namespace lua_func;

LuaManager*LuaManager::m_pInstance=0;

void LuaDebugFunction(lua_State *L, lua_Debug *ar)
{
	MyAssert(false, "\nLUA_ERROR__ \n%s %s %s %s \n currentline%d numps%d linedefined%d lastlinedefined %d",
		ar->name,
		ar->namewhat,
		ar->what,
		ar->source,

		ar->currentline,
		ar->nups,
		ar->linedefined,
		ar->lastlinedefined
		);
}

LuaManager::LuaManager():
m_pNowLuaState(0),
m_NowLuaState_key(0)
{

}

LuaManager::LuaManager(const LuaManager&)
{

}

LuaManager&LuaManager::operator=(const LuaManager&)
{
	//使わないけど一応コピーコンストラクタ
	return *this;
}

LuaManager::~LuaManager()
{
	//すべてのLuaステートを解放
	LUA_STATE_MAP::iterator it=m_luaState.begin();
	while(it!=m_luaState.end()){
		lua_close((*it).second);
		++it;
	}
}

LuaManager&LuaManager::GetInstance()
{
	//シングルトンの実体を作って返す
	if(m_pInstance==0)m_pInstance=new LuaManager;
	return *m_pInstance;
}

void LuaManager::Release()
{
	//シングルトンの実体を解放
	if(m_pInstance)delete m_pInstance;
	m_pInstance=0;
}

BOOL LuaManager::ChangeState(LUA_STATE_KEY key)
{
	//キーに対応するLuaステートがあるかどうか
	if(m_luaState.count(key)==0){
		PrintDebugWindow("\nLUA_ERROR__"
			"指定したキーにステートが登録されていませんでした。キー番号 %d \n",(int)key);
		return FALSE;
	}
	
	m_pNowLuaState=m_luaState[key];
	m_NowLuaState_key=key;

	return TRUE;

}

BOOL LuaManager::CreateNewState(LUA_STATE_KEY key)
{
	if(m_luaState.count(key)){
		PrintDebugWindow("\nLUA_ERROR__"
			"指定したキーに既にステートが作成されていました。キー番号 %d \n",(int)key);
		return FALSE;
	}
	//なかった場合作成
	lua_State*L=luaL_newstate();
	luaopen_base(L);
//	lua_sethook(L, LuaDebugFunction, LUA_MASKCALL ,0);

	m_luaState.insert(LUA_STATE_MAP::value_type(key,L));

	return TRUE;
}

BOOL lua_func::OpenFile(const char*filename)
{
	//ファイルを開く
	lua_State*L=LUA.State();
	if(L==0){
		PrintDebugWindow("\nLUA_ERROR__"
			"lua_stateが設定されていない状態でファイルオープンが実行されました。\n"
			"CreateNewState関数でlua_stateを作成してください\n");
		return FALSE;
	}
	if(luaL_dofile(L,filename)){
		PrintDebugWindow(
			"\nLUA_ERROR__指定したファイルが開けませんでした。"
			"ファイルが存在しなかった、またはファイル内のlua文法が間違っている可能性があります\n"
			"ファイル名 ");		
		return FALSE;
	}
	return TRUE;
}

void lua_error_message(int type,const char*name)
{
	switch(type){
	case 0:
		PrintDebugWindow(
			"\nLUA_ERROR__指定した名前の変数を読み込めませんでした。定数が存在しなかった、またはその文法が間違っている可能性があります\n"
			"名前 %s\n",name);
		break;
	case 1:
		PrintDebugWindow(
			"\nLUA_ERROR__現在のキーにスタックが設定されていない状態で"
			"変数の読み込み関数が呼ばれました\n");
		break;
	case 2:
		PrintDebugWindow(
			"\nLUA_ERROR__現在のキーにスタックが設定されていない状態で"
			"スタック表示関数が呼ばれました\n");
		break;
	}
}


int lua_func::Get_int(const char*name)
{
	lua_State*L=LUA.State();

	if(L==0){
		lua_error_message(1,name);
	}
	//スタックの頭を一番下に（中身を空に）
	lua_settop(L,0);

	lua_getglobal(L,name);
	int type=lua_type(L,-1);

	//変数がなかった場合
	if(type==LUA_TNIL){
		lua_error_message(0,name);
		return 0;
	}

	return static_cast<int>(lua_tonumber(L,-1)+0.5);
}

bool lua_func::Get_bool(const char*name)
{
	lua_State*L=LUA.State();

	if(L==0){
		lua_error_message(1,name);
	}
	//スタックの頭を一番下に（中身を空に）
	lua_settop(L,0);

	lua_getglobal(L,name);
	int type=lua_type(L,-1);

	//変数がなかった場合
	if(type==LUA_TNIL){
		lua_error_message(0,name);
		return 0;
	}

	return lua_toboolean(L,-1)!=0;
}

float lua_func::Get_float(const char*name)
{
	lua_State*L=LUA.State();

	if(L==0){
		lua_error_message(1,name);
	}
	//スタックの頭を一番下に（中身を空に）
	lua_settop(L,0);

	lua_getglobal(L,name);
	int type=lua_type(L,-1);

	//変数がなかった場合
	if(type==LUA_TNIL){
		lua_error_message(0,name);
		return 0;
	}

	return static_cast<float>(lua_tonumber(L,-1));
}

string lua_func::Get_string(const char*name)
{
	lua_State*L=LUA.State();

	if(L==0){
		lua_error_message(1,name);
	}
	//スタックの頭を一番下に（中身を空に）
	lua_settop(L,0);

	lua_getglobal(L,name);
	int type=lua_type(L,-1);

	//変数がなかった場合
	if(type==LUA_TNIL){
		lua_error_message(0,name);
		return string();
	}

	return string(lua_tostring(L,-1));
}

double lua_func::Get_double(const char*name)
{
	lua_State*L=LUA.State();

	if(L==0){
		lua_error_message(1,name);
	}
	//スタックの頭を一番下に（中身を空に）
	lua_settop(L,0);

	lua_getglobal(L,name);
	int type=lua_type(L,-1);

	//変数がなかった場合
	if(type==LUA_TNIL){
		lua_error_message(0,name);
		return 0;
	}

	return (lua_tonumber(L,-1));
}

DWORD	lua_func::Get_DWORD(const char*name)
{
	lua_State*L=LUA.State();

	if(L==0){
		lua_error_message(1,name);
	}
	//スタックの頭を一番下に（中身を空に）
	lua_settop(L,0);

	lua_getglobal(L,name);
	int type=lua_type(L,-1);

	//変数がなかった場合
	if(type==LUA_TNIL){
		lua_error_message(0,name);
		return 0;
	}

	return static_cast<DWORD>(lua_tonumber(L,-1));
}

void lua_func::PrintStack()
{
	lua_State*L=LUA.State();
	if(L==0){
		lua_error_message(2,0);
	}
	//出力ウィンドウにスタックの中身を表示
	const int num=lua_gettop(L);

	PrintDebugWindow("\n----------------------------\n");
	PrintDebugWindow("-------||printStack||-------\n");
	PrintDebugWindow("----------------------------\n");
	PrintDebugWindow("-Number---Type---Data-------\n");

	if(num==0){
		PrintDebugWindow("----------No Stack---------\n");
		PrintDebugWindow("------------------------------\n\n");
		return;
	}

	for(int i=num;i>=1;i--){
		PrintDebugWindow("%2d(%3d): ",i,-num+i-1);
		int type=lua_type(L,i);
		switch(type){
		case LUA_TNIL:
			//NULL(値がない)
			PrintDebugWindow("NIL\n");
			break;
		case LUA_TBOOLEAN:
			//bool型
			if(lua_toboolean(L,i)){
				PrintDebugWindow("BOOLIEN true\n");
			}else{
				PrintDebugWindow("BOOLIEN false\n");
			}
			break;
		case LUA_TNUMBER:
			//数字(整数と実数どちらも含む)
			PrintDebugWindow("NUMBER %f\n",lua_tonumber(L,i));
			break;
		case LUA_TLIGHTUSERDATA:
			//void*
			PrintDebugWindow("LIGHTUSERDATA\n");
			break;
		case LUA_TSTRING:
			//文字列(const char*)
			PrintDebugWindow("STRING %s\n",lua_tostring(L,i));
			break;
		case LUA_TTABLE:
			//Luaテーブル
			PrintDebugWindow("TABLE\n");
			break;
		case LUA_TFUNCTION:
			//関数
			PrintDebugWindow("FUNCTION\n");
			break;
		case LUA_TUSERDATA:
			//メモリブロック(void*)
			PrintDebugWindow("USERDATA\n");
			break;
		case LUA_TTHREAD:
			//コルーチン
			PrintDebugWindow("THREAD\n");
			break;
		}
	}
	PrintDebugWindow("------------------------------\n\n");
}