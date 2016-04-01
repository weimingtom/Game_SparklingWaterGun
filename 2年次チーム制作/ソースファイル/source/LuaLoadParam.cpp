#define _USE_LUA_LIB
#include "All.h"

static CHARACTERCONSTPARAMETERS CharaParam;
static GAMESYSTEMCONSTPARAMETERS GameSystemParam;

//�萔parameter�ւ̃|�C���^�𓾂�
LPCHARACTERCONSTPARAMETERS GetConstChrParam(LPCHARACTER ptr)
{
	return &CharaParam;
}

//�Q�[���V�X�e���̒萔�p�����[�^�[�\���̂ւ̃|�C���^�𓾂�
LPGAMESYSTEMCONSTPARAMETERS GetConstGameParam()
{
	return &GameSystemParam;
}

//�t�@�C������p�����[�^�ǂݍ���
bool LoadLuaCharacterParam(const char *fileName)
{
	//�Ƃ肠�����l���荞�݂܂�
	MyAssert(LUA.ChangeState(lua_key::character_key), "�L�����N�^��lua�X�e�[�g�L�[�؂�ւ����������܂���ł���");
	MyAssert(lua_func::OpenFile(fileName), "�t�@�C���I�[�v�����s");
	
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

//�t�@�C������p�����[�^�ǂݍ���
bool LoadLuaGameParam(const char *fileName)
{
	MyAssert(LUA.ChangeState(lua_key::game_system_key), "�Q�[���V�X�e����lua�X�e�[�g�L�[�؂�ւ����������܂���ł���");
	MyAssert(lua_func::OpenFile(fileName), "�t�@�C���I�[�v�����s");

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

//���ǂݍ���

bool LoadLuaMusic(const char *fileName)
{
	MyAssert(LUA.ChangeState(lua_key::game_system_key), "�Q�[���V�X�e����lua�X�e�[�g�L�[�؂�ւ����������܂���ł���");
	MyAssert(lua_func::OpenFile(fileName), "�t�@�C���I�[�v�����s");

	const char* music_file, *type;
	float start,end;
	lua_State* L = LUA.State();
	lua_settop(L, 0);
	char temp[MAX_PATH];
	int multi;

	for (int i = 0; i < ARRAYSIZE(sound_t_names::str); ++i)
	{
		//�e�[�u���擾
		lua_getglobal(L, sound_t_names::str[i]);

		MyAssert(lua_istable(L, -1), "LuaMusic�t�@�C���ɉ��e�[�u���̏����Y�ꂪ����܂� �e�[�u���� %s", sound_t_names::str[i]);

		//�t�@�C����
		lua_getfield(L, -1, "file");
		MyAssert(lua_isstring(L, -1), "LuaMusic�t�@�C���ɉ��t�@�C���̏����Y�ꂪ����܂� �e�[�u���� %s", sound_t_names::str[i]);
		music_file = lua_tostring(L, -1);

		strcpy_s<MAX_PATH>(temp, music_file);

		//���^�C�v
		lua_getfield(L, -2, "type");
		MyAssert(lua_isstring(L, -1), "LuaMusic�t�@�C���Ƀ^�C�v�̏����Y�ꂪ����܂� �e�[�u���� %s", sound_t_names::str[i]);
		type = lua_tostring(L, -1);

		//�ǂݍ���
		if (strcmp(type, "SE") == 0)
		{
			lua_getfield(L, -3, "minus_start");
			MyAssert(lua_isnumber(L, -1), "LuaMusic�t�@�C���ɕ������鋗���̏����Y�ꂪ����܂� �e�[�u���� %s", sound_t_names::str[i]);
			start = (float)lua_tonumber(L, -1);

			lua_getfield(L, -4, "minus_end");
			MyAssert(lua_isnumber(L, -1), "LuaMusic�t�@�C���ɕ������鋗���̏����Y�ꂪ����܂� �e�[�u���� %s", sound_t_names::str[i]);
			end = (float)lua_tonumber(L, -1);

			lua_getfield(L, -5, "multi");
			MyAssert(lua_isnumber(L, -1), "LuaMusic�t�@�C���ɓ����Đ����̏����Y�ꂪ����܂� �e�[�u���� %s", sound_t_names::str[i]);
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