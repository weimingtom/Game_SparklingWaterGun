#include "All.h"

void CreateLuaState()
{
	for (int i = 0; i < lua_key::__key_max; i++)
		LUA.CreateNewState(i);
}