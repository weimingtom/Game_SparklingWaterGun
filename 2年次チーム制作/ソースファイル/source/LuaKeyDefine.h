#ifndef __LUA_KEY_DEFINE_H__
#define __LUA_KEY_DEFINE_H__


//lua�X�e�[�g�Ɋ���U��L�[�i�ԍ��j�̗񋓂�����w�b�_
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

//���ׂẴL�[�ɑΉ������X�e�[�g���쐬����
void CreateLuaState();

#endif