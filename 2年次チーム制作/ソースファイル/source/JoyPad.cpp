#include "JoyPad.h"
#include <algorithm>

JoyPad::JoyPad(
	UINT	search_flame,	//Œ»Ý‚ÌƒtƒŒ[ƒ€‚©‚çU“®‚ðŒŸ’m‚·‚éŽžŠÔ
	UINT	shake_value	):	//‚±‚Ì’lˆÙí‚ÌŒX‚«‚ðU‚Á‚½‚à‚Ì‚Æ‚·‚é
	m_Search_flame(search_flame),
	m_Shake_value(shake_value),
	m_Controller_num(0),
	m_ShakeFlg(false)
{

	m_Search_flame = 30;
	m_Shake_value = 100;

}


void JoyPad::operator =(const JoyPad& pd)
{
	m_Controller_num = pd.m_Controller_num;
	m_List = pd.m_List;
	m_Search_flame = pd.m_Search_flame;
	m_Shake_value = pd.m_Shake_value;
	m_Controller_num = pd.m_Controller_num;
	m_ShakeFlg = pd.m_ShakeFlg;
}

JoyPad::~JoyPad()
{
}

bool JoyPad::ShakeJuge()
{
	return m_ShakeFlg;
}

void JoyPad::DebugChangeValue()
{
	m_Search_flame += KEY(KEY_AXISX, 0) / 1000;
	m_Shake_value += KEY(KEY_AXISY, 0) / 1000;

	MyDebugMgrStr2(0xFFFF0000,"Search_flame=%d Shake_value=%d \n",
		m_Search_flame,
		m_Shake_value
		);
}

void JoyPad::PushNowGyroValue()
{
	GyroValue gv =
	{
		KEY(KEY_AXISX3, m_Controller_num),
		KEY(KEY_AXISY3, m_Controller_num),
	};

	m_List.push_back(gv);
}

void JoyPad::Update()
{
//	DebugChangeValue();
	PushNowGyroValue();

	while (m_List.size()>(UINT)m_Search_flame)
	{
		m_List.pop_front();
	}

	if (m_List.size() <= 2)
	{
		return;
	}

	GyroValue temp;
	ValueList::iterator it, preit;

	it = preit = m_List.begin();
	++it;

	int total = 0;

	while (it != m_List.end())
	{
		const GyroValue& pre = *preit;
		const GyroValue& i = *it;

		temp.X = i.X - pre.X;
		temp.X = ABS(temp.X);

		temp.Y = i.Y - pre.Y;
		temp.Y = ABS(temp.Y);

		total += max(temp.X, temp.Y);

		++it;
		++preit;
	}

	//•½‹Ï‚ÌŒX‚«‚ðŽZo
	total /= static_cast<int>(m_List.size()) - 1;

	//ƒtƒ‰ƒO‚ðXV
	m_ShakeFlg = total > m_Shake_value;

	return;
}

void JoyPad::SetControllerNumber(int num)
{
	m_Controller_num = num;
}