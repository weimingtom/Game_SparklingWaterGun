#ifndef __SOUND_MESSAGE_H__
#define __SOUND_MESSAGE_H__

#include "../EnumCP11.h"
//***************************************************************
//	環境音メッセージ
//***************************************************************

_ENUM_BEGIN(SOUND_TYPE, sound_type)
	error = -1,
	foot,	//あしおと
	gun,	//銃声

	num_envsound,
	_ENUM_END(SOUND_TYPE, sound_type)

//そのうち場所を移す
struct Sound_Msg
{
	
	SOUND_TYPE			type;
	Vector3						source_Pos;	//音の位置
	float							power;					//音の大きさ
	int								end_Frame;	//音の消失フレーム（end_Frame - current_Frame）
	Sound_Msg(){}
	Sound_Msg(const SOUND_TYPE& t, const Vector3& pos, const int& end_F, const float& p)
	{
		type = t;		source_Pos = pos;		end_Frame = end_F;	power = p;
	}
};

#endif
