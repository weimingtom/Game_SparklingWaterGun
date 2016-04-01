#ifndef __LUA_LOAD_PARAM_H__
#define __LUA_LOAD_PARAM_H__

#include "CODBOForwardDecl.h"
//*****************************************************
//	Luaパラメータ読み込みヘッダ
//*****************************************************

#define LUA_PARAMETER_LOAD_ENDSTR "_end"

//*****************************************************
//	キャラクターパラメーター関連
//*****************************************************

//lua上でのParameterTable名
static const char CHARACTERCONSTPARAMTABLENAME[]=
{
	"CharacterConstParameters"
};

//lua上でのparameter名
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

//定数パラメータ構造体(順番変えないで～)
typedef struct
{
	union{
		struct{
			float move_acceleration;			//通常移動時の加速度
			float move_down_speed;				//ｘｚ平面において移動速度の落ちる割合
			float move_max_xz_speed;			//ｘｚ平面においての最大速度
			float rotate_speed;					//最大回転速度
			float rotate_max_up;				//どこまで上を見ることができるか
			float rotate_max_down;				//どこまで下を見ることができるか
			float glavity;						//重力加速度
			float jump_power;					//ジャンプ力
			float jump_frame;					//ジャンプをするまでのフレーム
			float jump_end_frame;				//ジャンプ終わりの硬直フレーム
			float jump_xz_acceleration;			//ジャンプ時のＸＺ平面における加速度
			float ride_height;					//どれだけの高さの段差を歩いて登れるか
			float adjust_floor;					//床の吸い付き
			float check_wall_size;				//壁とのあたり判定サイズ
			float check_wall_height;			//壁とあたり判定のとるキャラクタの高さ
			float jump_move_min_val;			//ジャンプ中に移動できる最低移動量
			
			float rolliingMovePower;			//ローリングの最大charge時の初速度
			float rolliingNeedCharge;			//ローリングをするのに必要なcharge率
			float rollingAtkValue;				//ローリングの攻撃力

			float max_hp;						//最大体力
			float dying_hp;						//瀕死状態と判定するHP
			float damage_hit_width_size;		//あたり判定の大きさ（よこの太さ）
			float default_sight_size;			//通常時(何もロックしていない)のエイムの大きさ
			float default_auto_aim_area_size;	//デフォルトのautoエイム範囲

			float dying_move_xz_max_speed;		//瀕死状態のxz平面の最大速度
			float dying_rotate_speed;			//瀕死状態の回転速度
			float dying_life_damage_speed;		//瀕死状態の体力の減るスピード
			float dying_begin_frame;			//瀕死開始時の時間		
			float revive_time;					//死亡から生き返るまでの時間

			float character_move_speed_up_value;	//シェアオブジェクトを変えた時に上がる移動速度の値
			float character_move_speed_up_max_value;//シェアオブジェクトを変えた時に上がる移動速度の最大値

			float character_charge_speed_up_value;//シェアオブジェクトを変えた時に上がるcharge速度の値
			float character_charge_speed_up_max_value;//シェアオブジェクトを変えた時に上がるcharge速度の最大値

			float near_atk_cola_damage_frame;	//近距離コーラナグリ攻撃のダメージ
			float near_atk_kick_damage_frame;	//近距離キックのダメージ	
		
			float cola_gauge_up_speed;			//振った時のゲージ上昇スピード
			float cola_gauge_down_speed;		//振ってない時のゲージ下降スピード
			float cola_gauge_fire_down_speed;	//打った時の時のゲージ下降スピード

			float explode_blur_len;				//爆発にブラーをかける距離


		};
		float _PARAMETER_ARRAY[ARRAYSIZE(CHARACTERCONSTPARAMETERSLUANAME) - 1];	//代入用のパラメーター配列
	};
}
CHARACTERCONSTPARAMETERS;

typedef const CHARACTERCONSTPARAMETERS*	LPCHARACTERCONSTPARAMETERS;

//キャラクタの定数パラメーターへのポインタを得る
LPCHARACTERCONSTPARAMETERS GetConstChrParam(LPCHARACTER ptr);


//*****************************************************
//	ゲーム環境パラメータ
//*****************************************************

static const char GAMEPLAYCONSTPARAMETERTABLENAME[]=
{
	"CODBO_GAME_PARAMETERS"
};

static const char* GAMEPLAYCONSTPARAMETERLUANAME[]=
{
	{ "death_score_rate" },			//死亡時のスコア
	{ "kill_score_rate" },			//倒した時のスコア
	{ "share_score" },				//一定時間ごとにシェア率をもとに加算されるスコア
	{ "share_change_score" },		//広告オブジェクトを変更したときのスコア
	{ "share_score_add_frame" },	//シェアスコアを加算する時間間隔

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

//ゲームシステムの定数パラメーター構造体へのポインタを得る
LPGAMESYSTEMCONSTPARAMETERS GetConstGameParam();

//************************************************************************
//	パラメーター初期化関数
//************************************************************************

//ファイルからパラメータ読み込み
bool LoadLuaCharacterParam(const char *fileName);

//ファイルからパラメータ読み込み
bool LoadLuaGameParam(const char *fileName);

//音をファイルから読み込み
bool LoadLuaMusic(const char *fileName);

#endif