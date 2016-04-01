#ifndef __LUA_KEY_DEFINE_H__
#define __LUA_KEY_DEFINE_H__


//luaステートに割り振るキー（番号）の列挙をするヘッダ
namespace lua_key
{
	enum
	{
		game_system_key,
		character_key,
		mesh_pass_key,

		__key_max
	};
}

//すべてのキーに対応したステートを作成する
void CreateLuaState();

#endif