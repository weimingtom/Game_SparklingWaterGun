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
	//�g��Ȃ����ǈꉞ�R�s�[�R���X�g���N�^
	return *this;
}

LuaManager::~LuaManager()
{
	//���ׂĂ�Lua�X�e�[�g�����
	LUA_STATE_MAP::iterator it=m_luaState.begin();
	while(it!=m_luaState.end()){
		lua_close((*it).second);
		++it;
	}
}

LuaManager&LuaManager::GetInstance()
{
	//�V���O���g���̎��̂�����ĕԂ�
	if(m_pInstance==0)m_pInstance=new LuaManager;
	return *m_pInstance;
}

void LuaManager::Release()
{
	//�V���O���g���̎��̂����
	if(m_pInstance)delete m_pInstance;
	m_pInstance=0;
}

BOOL LuaManager::ChangeState(LUA_STATE_KEY key)
{
	//�L�[�ɑΉ�����Lua�X�e�[�g�����邩�ǂ���
	if(m_luaState.count(key)==0){
		PrintDebugWindow("\nLUA_ERROR__"
			"�w�肵���L�[�ɃX�e�[�g���o�^����Ă��܂���ł����B�L�[�ԍ� %d \n",(int)key);
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
			"�w�肵���L�[�Ɋ��ɃX�e�[�g���쐬����Ă��܂����B�L�[�ԍ� %d \n",(int)key);
		return FALSE;
	}
	//�Ȃ������ꍇ�쐬
	lua_State*L=luaL_newstate();
	luaopen_base(L);
//	lua_sethook(L, LuaDebugFunction, LUA_MASKCALL ,0);

	m_luaState.insert(LUA_STATE_MAP::value_type(key,L));

	return TRUE;
}

BOOL lua_func::OpenFile(const char*filename)
{
	//�t�@�C�����J��
	lua_State*L=LUA.State();
	if(L==0){
		PrintDebugWindow("\nLUA_ERROR__"
			"lua_state���ݒ肳��Ă��Ȃ���ԂŃt�@�C���I�[�v�������s����܂����B\n"
			"CreateNewState�֐���lua_state���쐬���Ă�������\n");
		return FALSE;
	}
	if(luaL_dofile(L,filename)){
		PrintDebugWindow(
			"\nLUA_ERROR__�w�肵���t�@�C�����J���܂���ł����B"
			"�t�@�C�������݂��Ȃ������A�܂��̓t�@�C������lua���@���Ԉ���Ă���\��������܂�\n"
			"�t�@�C���� ");		
		return FALSE;
	}
	return TRUE;
}

void lua_error_message(int type,const char*name)
{
	switch(type){
	case 0:
		PrintDebugWindow(
			"\nLUA_ERROR__�w�肵�����O�̕ϐ���ǂݍ��߂܂���ł����B�萔�����݂��Ȃ������A�܂��͂��̕��@���Ԉ���Ă���\��������܂�\n"
			"���O %s\n",name);
		break;
	case 1:
		PrintDebugWindow(
			"\nLUA_ERROR__���݂̃L�[�ɃX�^�b�N���ݒ肳��Ă��Ȃ���Ԃ�"
			"�ϐ��̓ǂݍ��݊֐����Ă΂�܂���\n");
		break;
	case 2:
		PrintDebugWindow(
			"\nLUA_ERROR__���݂̃L�[�ɃX�^�b�N���ݒ肳��Ă��Ȃ���Ԃ�"
			"�X�^�b�N�\���֐����Ă΂�܂���\n");
		break;
	}
}


int lua_func::Get_int(const char*name)
{
	lua_State*L=LUA.State();

	if(L==0){
		lua_error_message(1,name);
	}
	//�X�^�b�N�̓�����ԉ��Ɂi���g����Ɂj
	lua_settop(L,0);

	lua_getglobal(L,name);
	int type=lua_type(L,-1);

	//�ϐ����Ȃ������ꍇ
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
	//�X�^�b�N�̓�����ԉ��Ɂi���g����Ɂj
	lua_settop(L,0);

	lua_getglobal(L,name);
	int type=lua_type(L,-1);

	//�ϐ����Ȃ������ꍇ
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
	//�X�^�b�N�̓�����ԉ��Ɂi���g����Ɂj
	lua_settop(L,0);

	lua_getglobal(L,name);
	int type=lua_type(L,-1);

	//�ϐ����Ȃ������ꍇ
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
	//�X�^�b�N�̓�����ԉ��Ɂi���g����Ɂj
	lua_settop(L,0);

	lua_getglobal(L,name);
	int type=lua_type(L,-1);

	//�ϐ����Ȃ������ꍇ
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
	//�X�^�b�N�̓�����ԉ��Ɂi���g����Ɂj
	lua_settop(L,0);

	lua_getglobal(L,name);
	int type=lua_type(L,-1);

	//�ϐ����Ȃ������ꍇ
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
	//�X�^�b�N�̓�����ԉ��Ɂi���g����Ɂj
	lua_settop(L,0);

	lua_getglobal(L,name);
	int type=lua_type(L,-1);

	//�ϐ����Ȃ������ꍇ
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
	//�o�̓E�B���h�E�ɃX�^�b�N�̒��g��\��
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
			//NULL(�l���Ȃ�)
			PrintDebugWindow("NIL\n");
			break;
		case LUA_TBOOLEAN:
			//bool�^
			if(lua_toboolean(L,i)){
				PrintDebugWindow("BOOLIEN true\n");
			}else{
				PrintDebugWindow("BOOLIEN false\n");
			}
			break;
		case LUA_TNUMBER:
			//����(�����Ǝ����ǂ�����܂�)
			PrintDebugWindow("NUMBER %f\n",lua_tonumber(L,i));
			break;
		case LUA_TLIGHTUSERDATA:
			//void*
			PrintDebugWindow("LIGHTUSERDATA\n");
			break;
		case LUA_TSTRING:
			//������(const char*)
			PrintDebugWindow("STRING %s\n",lua_tostring(L,i));
			break;
		case LUA_TTABLE:
			//Lua�e�[�u��
			PrintDebugWindow("TABLE\n");
			break;
		case LUA_TFUNCTION:
			//�֐�
			PrintDebugWindow("FUNCTION\n");
			break;
		case LUA_TUSERDATA:
			//�������u���b�N(void*)
			PrintDebugWindow("USERDATA\n");
			break;
		case LUA_TTHREAD:
			//�R���[�`��
			PrintDebugWindow("THREAD\n");
			break;
		}
	}
	PrintDebugWindow("------------------------------\n\n");
}