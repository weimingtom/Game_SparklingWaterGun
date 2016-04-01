#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "iextreme.h"
#include "../JoyPad.h"
//***********************************************
//	コントローラーに関するヘッダ
//***********************************************

//前方宣言たち
struct ControllerButtonSet;

namespace controller
{
	typedef int CONTROLLER_NUM;	//コントローラ番号

	namespace button
	{
		enum button_type
		{
			//ボタンタイプ
			maru,
			batu,
			shikaku,
			sankaku,

			//RLボタン
			_R1,
			_R2,
			_R3,
			_L1,
			_L2,
			_L3,

			//方向キータイプ
			up,
			down,
			right,
			left,

			max,//ボタンタイプ最大数
		};
	}
	namespace stick{
		enum stick_type
		{
			//スティックタイプ
			left,
			right,

			max//スティックタイプ最大数
		};
	}
	//押した瞬間かどうか
	bool	GetTRG(button::button_type x, CONTROLLER_NUM num);
	//離した瞬間かどうか
	bool	GetLeave(button::button_type x, CONTROLLER_NUM num);
	//押しているかどうか
	bool	GetPush(button::button_type x, CONTROLLER_NUM num);
	//コントローラーを振っているかどうか
	bool	GetVibration(ControllerButtonSet& cnt);
	//スティックの傾きの値を得る
	Vector2 GetStickValue(stick::stick_type x, CONTROLLER_NUM num);
	
	//指定したコントローラ(オプションで設定した反転とかスティックの感度を適用したもの)のスティックの傾きの値を得る
	Vector2 GetMyControllerSettingStickValue(
		stick::stick_type				stick_t,
		const ControllerButtonSet&		info
		);

	//指定したコントローラーを振動させる
	void SetVibration(RATE power, int frame, CONTROLLER_NUM num);
}

//デフォルトのbuttonセットを設定する
void SetUsualButtonSet(ControllerButtonSet& out, controller::CONTROLLER_NUM number);

//コントローラButtonとstick配置構造体
struct ControllerButtonSet
{
	controller::CONTROLLER_NUM		controller_num;
	controller::button::button_type ButtonSet[controller::button::max];	//Button配置
	controller::stick::stick_type	StickSet[controller::stick::max];	//Stick配置
	bool	stick_reverse		[controller::stick::max][2];			//stick反転フラグ(0にx、１にyのフラグが入っている)
	float	stick_sensitivity	[controller::stick::max];				//stick感度LR
	JoyPad	ps3_parameter;												//ジャイロコントロール用パラメタ
public:
	ControllerButtonSet();
};


#endif