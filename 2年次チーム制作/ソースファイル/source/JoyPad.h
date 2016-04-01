#ifndef __JOYPAD_H_
#define __JOYPAD_H_

#include "iextreme.h"
#include <list>

class JoyPad
{
private:
	struct GyroValue
	{
		int X;
		int Y;
	};
	typedef std::list<GyroValue> ValueList;
private:
	ValueList			m_List;
	int					m_Search_flame;
	int					m_Shake_value;
	int					m_Controller_num;
	bool				m_ShakeFlg;
public:
	JoyPad(
		UINT	search_flame,	//現在のフレームから振動を検知する時間
		UINT	shake_value		//この値異常の傾きを振ったものとする
		);
	~JoyPad();
	void operator =(const JoyPad& pd);
private:
	void DebugChangeValue();
	//傾きの値を更新
	void PushNowGyroValue();
public:
	//傾きの判定を更新
	void Update();
public:
	bool ShakeJuge();				//	振ったかどうかを得る
public:
	void SetControllerNumber(int num);	//コントローラ番号を設定
};



#endif