#ifndef __LUA_LOAD_PARAM_H__
#define __LUA_LOAD_PARAM_H__

#include "CODBOForwardDecl.h"
//*****************************************************
//	Lua�p�����[�^�ǂݍ��݃w�b�_
//*****************************************************

#define LUA_PARAMETER_LOAD_ENDSTR "_end"

//*****************************************************
//	�L�����N�^�[�p�����[�^�[�֘A
//*****************************************************

//lua��ł�ParameterTable��
static const char CHARACTERCONSTPARAMTABLENAME[]=
{
	"CharacterConstParameters"
};

//lua��ł�parameter��
static const char* CHARACTERCONSTPARAMETERSLUANAME[]=
{
	{ "usual_move_acceleration" },
	{ "usual_move_down_speed" },
	{ "usual_move_max_xz_speed" },
	{ "usual_rotate_speed" },
	{ "usual_rotate_max_up_angle" },
	{ "usual_rotate_max_down_angle" },
	{ "glavity" }, 
	{ "jump_power" },
	{ "jump_frame" },
	{ "jump_end_frame" },
	{ "jump_xz_acceleration" },
	{ "ride_height" },
	{ "adjust_floor" },
	{ "check_wall_size" },
	{ "check_wall_height" },
	{ "jump_move_min_val" },

	{ "rolliingMovePower" },
	{ "rolliingNeedCharge" },
	{ "rollingAtkValue" },

	{ "max_hp" },
	{ "dying_hp" },
	{ "damage_hit_width_size" },

	{ "default_sight_size" },
	{ "default_auto_aim_area_size" },
	
	{ "dying_move_xz_max_speed" },
	{ "dying_rotate_speed" },
	{ "dying_life_damage_speed" },
	{ "dying_begin_frame" },
	{ "revive_time" },

	{ "character_move_speed_up_value" },
	{ "character_move_speed_up_max_value" },

	{ "character_charge_speed_up_value" },
	{ "character_charge_speed_up_max_value" },

	{ "near_atk_cola_damage_frame" },
	{ "near_atk_kick_damage_frame" },

	{ "cola_gauge_up_speed" },
	{ "cola_gauge_down_speed" },
	{ "cola_gauge_fire_down_speed" },
	
	{ "explode_blur_len" },
	
	{ LUA_PARAMETER_LOAD_ENDSTR }
};


class Character;
typedef Character* LPCHARACTER;

//�萔�p�����[�^�\����(���ԕς��Ȃ��Ł`)
typedef struct
{
	union{
		struct{
			float move_acceleration;			//�ʏ�ړ����̉����x
			float move_down_speed;				//�������ʂɂ����Ĉړ����x�̗����銄��
			float move_max_xz_speed;			//�������ʂɂ����Ă̍ő呬�x
			float rotate_speed;					//�ő��]���x
			float rotate_max_up;				//�ǂ��܂ŏ�����邱�Ƃ��ł��邩
			float rotate_max_down;				//�ǂ��܂ŉ������邱�Ƃ��ł��邩
			float glavity;						//�d�͉����x
			float jump_power;					//�W�����v��
			float jump_frame;					//�W�����v������܂ł̃t���[��
			float jump_end_frame;				//�W�����v�I���̍d���t���[��
			float jump_xz_acceleration;			//�W�����v���̂w�y���ʂɂ���������x
			float ride_height;					//�ǂꂾ���̍����̒i��������ēo��邩
			float adjust_floor;					//���̋z���t��
			float check_wall_size;				//�ǂƂ̂����蔻��T�C�Y
			float check_wall_height;			//�ǂƂ����蔻��̂Ƃ�L�����N�^�̍���
			float jump_move_min_val;			//�W�����v���Ɉړ��ł���Œ�ړ���
			
			float rolliingMovePower;			//���[�����O�̍ő�charge���̏����x
			float rolliingNeedCharge;			//���[�����O������̂ɕK�v��charge��
			float rollingAtkValue;				//���[�����O�̍U����

			float max_hp;						//�ő�̗�
			float dying_hp;						//�m����ԂƔ��肷��HP
			float damage_hit_width_size;		//�����蔻��̑傫���i�悱�̑����j
			float default_sight_size;			//�ʏ펞(�������b�N���Ă��Ȃ�)�̃G�C���̑傫��
			float default_auto_aim_area_size;	//�f�t�H���g��auto�G�C���͈�

			float dying_move_xz_max_speed;		//�m����Ԃ�xz���ʂ̍ő呬�x
			float dying_rotate_speed;			//�m����Ԃ̉�]���x
			float dying_life_damage_speed;		//�m����Ԃ̗̑͂̌���X�s�[�h
			float dying_begin_frame;			//�m���J�n���̎���		
			float revive_time;					//���S���琶���Ԃ�܂ł̎���

			float character_move_speed_up_value;	//�V�F�A�I�u�W�F�N�g��ς������ɏオ��ړ����x�̒l
			float character_move_speed_up_max_value;//�V�F�A�I�u�W�F�N�g��ς������ɏオ��ړ����x�̍ő�l

			float character_charge_speed_up_value;//�V�F�A�I�u�W�F�N�g��ς������ɏオ��charge���x�̒l
			float character_charge_speed_up_max_value;//�V�F�A�I�u�W�F�N�g��ς������ɏオ��charge���x�̍ő�l

			float near_atk_cola_damage_frame;	//�ߋ����R�[���i�O���U���̃_���[�W
			float near_atk_kick_damage_frame;	//�ߋ����L�b�N�̃_���[�W	
		
			float cola_gauge_up_speed;			//�U�������̃Q�[�W�㏸�X�s�[�h
			float cola_gauge_down_speed;		//�U���ĂȂ����̃Q�[�W���~�X�s�[�h
			float cola_gauge_fire_down_speed;	//�ł������̎��̃Q�[�W���~�X�s�[�h

			float explode_blur_len;				//�����Ƀu���[�������鋗��


		};
		float _PARAMETER_ARRAY[ARRAYSIZE(CHARACTERCONSTPARAMETERSLUANAME) - 1];	//����p�̃p�����[�^�[�z��
	};
}
CHARACTERCONSTPARAMETERS;

typedef const CHARACTERCONSTPARAMETERS*	LPCHARACTERCONSTPARAMETERS;

//�L�����N�^�̒萔�p�����[�^�[�ւ̃|�C���^�𓾂�
LPCHARACTERCONSTPARAMETERS GetConstChrParam(LPCHARACTER ptr);


//*****************************************************
//	�Q�[�����p�����[�^
//*****************************************************

static const char GAMEPLAYCONSTPARAMETERTABLENAME[]=
{
	"CODBO_GAME_PARAMETERS"
};

static const char* GAMEPLAYCONSTPARAMETERLUANAME[]=
{
	{ "death_score_rate" },			//���S���̃X�R�A
	{ "kill_score_rate" },			//�|�������̃X�R�A
	{ "share_score" },				//��莞�Ԃ��ƂɃV�F�A�������Ƃɉ��Z�����X�R�A
	{ "share_change_score" },		//�L���I�u�W�F�N�g��ύX�����Ƃ��̃X�R�A
	{ "share_score_add_frame" },	//�V�F�A�X�R�A�����Z���鎞�ԊԊu

	{ LUA_PARAMETER_LOAD_ENDSTR },
};


typedef struct
{
	union{
		struct{
			struct 
			{
				float death_score_rate;
				float kill_score_rate;
				float share_score;
				float share_change_score;
				float share_score_add_frame;
			}ScoreRates;

		};
		float _PARAMETER_ARRAY[ARRAYSIZE(GAMEPLAYCONSTPARAMETERLUANAME)-1];
	};
}GAMESYSTEMCONSTPARAMETERS, *LPGAMESYSTEMCONSTPARAMETERS;

//�Q�[���V�X�e���̒萔�p�����[�^�[�\���̂ւ̃|�C���^�𓾂�
LPGAMESYSTEMCONSTPARAMETERS GetConstGameParam();

//************************************************************************
//	�p�����[�^�[�������֐�
//************************************************************************

//�t�@�C������p�����[�^�ǂݍ���
bool LoadLuaCharacterParam(const char *fileName);

//�t�@�C������p�����[�^�ǂݍ���
bool LoadLuaGameParam(const char *fileName);

//�����t�@�C������ǂݍ���
bool LoadLuaMusic(const char *fileName);

#endif