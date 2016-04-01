#ifndef __GAME_ID_DEFINE_H__
#define __GAME_ID_DEFINE_H__

#include "../CODBO.h"

#define GAME_ID_MAX 5000

namespace game_id
{
	enum _ENUM_ENTITY_ID
	{
		id_error,					//�G���[ID

		id_controller_manager,		//�R���g���[���}�l�[�W��
		id_sound_manager,			//���}�l�[�W��
		id_bullet_manager,			//�e���˃}�l�[�W��
		id_water_pool_manager,		//�����܂�}�l�[�W��
		id_damage_manager,			//�_���[�W�}�l�[�W��
		id_aim_target_manager,		//�G�C���^�[�Q�b�g�}�l�[�W��
		id_score_calc_manager,		//�X�R�A�J�E���g�}�l�[�W��
		id_screen_icon_manager,
		id_news_telop_manager,
		id_timer,

		id_controll_obj_first,			//�L�����N�^�[ID�̏���
		id_controll_obj_max = id_controll_obj_first + CODBO_CONTROLL_OBJECT_MAX,	//�L�����N�^�ő�ID+1

		id_map_object_min,			//�}�b�v�I�u�W�F�N�g�̏���
		id_map_object_max = id_map_object_min + CODBO_MAP_OBJECT_MAX,
		
	};
}

typedef game_id::_ENUM_ENTITY_ID	ENTITY_ID;	//�Q�[����ID

#define IS_CONTROLLOBJ_ID(id)	((id)>=game_id::id_controll_obj_first	&&		(id)<game_id::id_controll_obj_max)
#define IS_MAPOBJ_ID(id)		((id)>=game_id::id_map_object_min		&&		(id)<game_id::id_map_object_max)

#endif