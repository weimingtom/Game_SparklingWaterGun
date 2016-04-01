#ifndef __COD_PLAY_SOUND_H__
#define __COD_PLAY_SOUND_H__


namespace sound_t{
	enum 
	{
		error = -1,

		shot_weapon_begin,
		shot_weapon,	//���픭�˒�
		foot_sound,
		jump,
		damage_hit,

		landing,
		poster_update,	//�|�X�^�[����
		shake,
		explode,
		apper_vending,
		rolling_start,

		//��������BGM

		share_match,

		_max_sound,
	};
};

namespace sound_t_names{

	static const char* str[] =
	{
		{ "shot_weapon_begin" },
		{ "shot_weapon" },
		{ "foot_sound" },
		{ "jump" },
		{ "damage_hit" },

		{ "landing" },
		{ "poster_update" },
		{ "shake" },
		{ "explode" },
		{ "apper_vending" },
		{ "rolling_start" },

		//��������BGM
		{"share_match"}
	};
};



#endif