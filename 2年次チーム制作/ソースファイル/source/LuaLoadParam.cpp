#define _USE_LUA_LIB
#include "All.h"

static CHARACTERCONSTPARAMETERS CharaParam;
static GAMESYSTEMCONSTPARAMETERS GameSystemParam;

//定数parameterへのポインタを得る
LPCHARACTERCONSTPARAMETERS GetConstChrParam(LPCHARACTER ptr)
{
	return &CharaParam;
}

//ゲームシステムの定数パラメーター構造体へのポインタを得る
LPGAMESYSTEMCONSTPARAMETERS GetConstGameParam()
{
	return &GameSystemParam;
}

//ファイルからパラメータ読み込み
bool LoadLuaCharacterParam(const char *fileName)
{
	//とりあえず値放り込みます
	MyAssert(LUA.ChangeState(lua_key::character_key), "キャラクタのluaステートキー切り替えが完了しませんでした");
	MyAssert(lua_func::OpenFile(fileName), "ファイルオープン失敗");
	
	lua_State* L = LUA.State();
	lua_settop(L, 0);

	lua_getglobal(L, CHARACTERCONSTPARAMTABLENAME);

	for (int i = 0; i < sizeof(CHARACTERCONSTPARAMETERS) / sizeof(float); i++)
	{
		lua_getfield(L, -1, CHARACTERCONSTPARAMETERSLUANAME[i]);

		CharaParam._PARAMETER_ARRAY[i] = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	return true;
}

//ファイルからパラメータ読み込み
bool LoadLuaGameParam(const char *fileName)
{
	MyAssert(LUA.ChangeState(lua_key::game_system_key), "ゲームシステムのluaステートキー切り替えが完了しませんでした");
	MyAssert(lua_func::OpenFile(fileName), "ファイルオープン失敗");

	lua_State* L = LUA.State();
	lua_settop(L, 0);

	lua_getglobal(L, GAMEPLAYCONSTPARAMETERTABLENAME);

	for (int i = 0; i < sizeof(GAMESYSTEMCONSTPARAMETERS) / sizeof(float); i++)
	{
		lua_getfield(L, -1, GAMEPLAYCONSTPARAMETERLUANAME[i]);

		GameSystemParam._PARAMETER_ARRAY[i] = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	return true;
}

//音読み込み

bool LoadLuaMusic(const char *fileName)
{
	MyAssert(LUA.ChangeState(lua_key::game_system_key), "ゲームシステムのluaステートキー切り替えが完了しませんでした");
	MyAssert(lua_func::OpenFile(fileName), "ファイルオープン失敗");

	const char* music_file, *type;
	float start,end;
	lua_State* L = LUA.State();
	lua_settop(L, 0);
	char temp[MAX_PATH];
	int multi;

	for (int i = 0; i < ARRAYSIZE(sound_t_names::str); ++i)
	{
		//テーブル取得
		lua_getglobal(L, sound_t_names::str[i]);

		MyAssert(lua_istable(L, -1), "LuaMusicファイルに音テーブルの書き忘れがあります テーブル名 %s", sound_t_names::str[i]);

		//ファイル名
		lua_getfield(L, -1, "file");
		MyAssert(lua_isstring(L, -1), "LuaMusicファイルに音ファイルの書き忘れがあります テーブル名 %s", sound_t_names::str[i]);
		music_file = lua_tostring(L, -1);

		strcpy_s<MAX_PATH>(temp, music_file);

		//音タイプ
		lua_getfield(L, -2, "type");
		MyAssert(lua_isstring(L, -1), "LuaMusicファイルにタイプの書き忘れがあります テーブル名 %s", sound_t_names::str[i]);
		type = lua_tostring(L, -1);

		//読み込み
		if (strcmp(type, "SE") == 0)
		{
			lua_getfield(L, -3, "minus_start");
			MyAssert(lua_isnumber(L, -1), "LuaMusicファイルに聞こえる距離の書き忘れがあります テーブル名 %s", sound_t_names::str[i]);
			start = (float)lua_tonumber(L, -1);

			lua_getfield(L, -4, "minus_end");
			MyAssert(lua_isnumber(L, -1), "LuaMusicファイルに聞こえる距離の書き忘れがあります テーブル名 %s", sound_t_names::str[i]);
			end = (float)lua_tonumber(L, -1);

			lua_getfield(L, -5, "multi");
			MyAssert(lua_isnumber(L, -1), "LuaMusicファイルに同時再生数の書き忘れがあります テーブル名 %s", sound_t_names::str[i]);
			multi = lua_tointeger(L, -1);

			SOUNDPLAYER.RegistSE(i, temp, start, end, multi);
		}
		else if (strcmp(type, "BGM") == 0)
		{
			SOUNDPLAYER.RegistBGM(i, temp);
		}

		lua_settop(L, 0);
	}

	return true;
}