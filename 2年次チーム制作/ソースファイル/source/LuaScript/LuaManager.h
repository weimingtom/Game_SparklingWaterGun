
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
//		Lua�X�N���v�g
//*********************************************************
struct lua_State;
//*********************************************************
//		�f�o�b�O�E�B���h�E(Alt+2�ŕ\��)
//*********************************************************
#define PrintDebugWindow(str,...) MyDebugString(str,__VA_ARGS__)

//*********************************************************
//		LUA�X�e�[�g�̃L�[
//*********************************************************
typedef int LUA_STATE_KEY;

//*********************************************************
/*	�@~����~
	�e�L�X�g�t�@�C������͂��čăR���p�C�������Ƀv���O�����̓��e��ύX���邱�Ƃ�
	�ł��܂��BOpenFile�֐��Ƀt�@�C�����ƃp�X���w�肷�邱�ƂŃt�@�C�����J���܂��B
	�t�@�C�����J������Ԃ� Get�` �֐����ĂԂ��Ƃł��̕ϐ����t�@�C������ǂݍ��݂܂�
	��̃��A�X�e�[�g�Ńt�@�C���𕡐��J�����Ƃ͂ł��܂��񂪁A
	ChangeState�֐����g�����Ƃŕ����̃��A�X�e�[�g���쐬���邱�Ƃ��ł��܂��B
	���p�I������LUA_Release�}�N�����Ăяo���Ă��������B(���p���Ă��Ȃ���Ԃł��Ă��OK)
*/
//*********************************************************

//*********************************************************
//		Lua�X�N���v�g�Ǘ��N���X
//*********************************************************
class LuaManager
{
	typedef lua_State*LPLUA_STATE;
	typedef map<LUA_STATE_KEY,LPLUA_STATE>LUA_STATE_MAP;
private:
	//�V���O���g���̎���
	static LuaManager*			m_pInstance;
	//���A�X�e�[�g(lua������ɂ͕K���K�v)
	LUA_STATE_MAP			m_luaState;
	//���݂̃��A�X�e�[�g�Ƃ��̃L�[
	LUA_STATE_KEY				m_NowLuaState_key;
	LPLUA_STATE					m_pNowLuaState;
private:
	//�V���O���g���Ȃ̂�private�Ɂ`
	LuaManager();
	LuaManager(const LuaManager&);
	LuaManager&operator=(const LuaManager&);
	~LuaManager();
public:
	//���̂̃Q�b�^�[
	static LuaManager& GetInstance();

	//�N���X�̗��p���I������(�v���O�����I���̍ۂɕK���Ăяo���Ă�������)
	static void Release();
public:

	//���A�X�e�[�g��؂�ւ��܂�(���݂��Ȃ��L�[�������ꍇ�͐؂�ւ��Ȃ�)
	//�f�t�H���g�ł�0�Ԃ̃X�e�[�g���g���Ă��܂�
	BOOL ChangeState(LUA_STATE_KEY key);

	//�w�肵���L�[�ɐV���Ƀ��A�X�e�[�g���쐬����
	//(�w�肵���L�[�Ɋ��ɂ������ꍇ�͂Ȃɂ����Ȃ�)
	BOOL CreateNewState(LUA_STATE_KEY key);

	//Lua�X�e�[�g�ւ̃|�C���^�𓾂܂�
	//lua���C�u�����̊֐����g�������Ƃ��ɂǂ���
	inline LPLUA_STATE State()const{
		return m_pNowLuaState;
	}
};

//*********************************************************
//		�֗��֐�
//*********************************************************
namespace lua_func
{
	//�w�肵���t�@�C�����J���܂�
	BOOL		OpenFile	(const char*filename);

	//���݂̐ݒ肵�Ă���X�e�[�g���g���w�肵�����O�̕ϐ��̒l���������܂�
	//(�w�肵�����O�̒萔�����݂��Ȃ������ꍇ,�܂��̓t�@�C�����J���Ă��Ȃ���Ԃ�
	//�֐����Ăяo�����ꍇ��0��ԋp���f�o�b�O�E�B���h�E�ɕ\�����܂�)
	int			Get_int			(const char*name);
	bool		Get_bool		(const char*name);
	float		Get_float		(const char*name);
	double		Get_double		(const char*name);
	string		Get_string		(const char*name);
	DWORD		Get_DWORD		(const char*name);

	//���݂̃L�[��Lua�X�^�b�N�̓��e���o�̓E�B���h�E�ɕ\�����܂�
	void		PrintStack();
};
//*********************************************************
//		�ȈՌĂяo���}�N��
//*********************************************************
//���p�I��
#define LUA_Release (LuaManager::Release())
//���̌Ăяo��
#define LUA (LuaManager::GetInstance())


#endif