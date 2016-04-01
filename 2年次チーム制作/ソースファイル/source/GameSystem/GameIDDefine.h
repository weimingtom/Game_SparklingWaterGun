#ifndef __GAME_ID_DEFINE_H__
#define __GAME_ID_DEFINE_H__

#include "../CODBO.h"

#define GAME_ID_MAX 5000

namespace game_id
{
	enum _ENUM_ENTITY_ID
	{
		id_error,					//エラーID

		id_controller_manager,		//コントローラマネージャ
		id_sound_manager,			//音マネージャ
		id_bullet_manager,			//弾発射マネージャ
		id_water_pool_manager,		//水たまりマネージャ
		id_damage_manager,			//ダメージマネージャ
		id_aim_target_manager,		//エイムターゲットマネージャ
		id_score_calc_manager,		//スコアカウントマネージャ
		id_screen_icon_manager,
		id_news_telop_manager,
		id_timer,

		id_controll_obj_first,			//キャラクターIDの初め
		id_controll_obj_max = id_controll_obj_first + CODBO_CONTROLL_OBJECT_MAX,	//キャラクタ最大ID+1

		id_map_object_min,			//マップオブジェクトの初め
		id_map_object_max = id_map_object_min + CODBO_MAP_OBJECT_MAX,
		
	};
}

typedef game_id::_ENUM_ENTITY_ID	ENTITY_ID;	//ゲーム内ID

#define IS_CONTROLLOBJ_ID(id)	((id)>=game_id::id_controll_obj_first	&&		(id)<game_id::id_controll_obj_max)
#define IS_MAPOBJ_ID(id)		((id)>=game_id::id_map_object_min		&&		(id)<game_id::id_map_object_max)

#endif