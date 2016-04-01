#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "ControllObject.h"
#include "ControllObjectFunction.h"
#include "GameSystem\GameController.h"


//*****************************************************************//
//	ゲームをプレイする者(プレイヤとコンピュータ両方)のクラス	
//	操作するControllObjectへのポインタを持つ
//*****************************************************************//

class Controller
{
protected:
	PLAYERMATCHDATA				m_chr_match_data;		//このキャラクタのマッチデータ
	LPCONTROLLOBJCT				m_pControllObj;			//自分が操作しているオブジェクトへのポインタ
public:
	Controller(const PLAYERMATCHDATA& chr_match_data) :
		m_pControllObj(nullptr),
		m_chr_match_data(chr_match_data)
	{
	}
	~Controller(){}
public:
	inline ControllObject* GetControllObjectPtr()
	{
		return m_pControllObj;
	}
	inline const PLAYERMATCHDATA& GetMatchData()const
	{
		return m_chr_match_data;
	}
	void SetControllObj(LPCONTROLLOBJCT p)
	{
		m_pControllObj = p;
		if (p)ControllObjFunc::SetMatchDataPtr(p, &m_chr_match_data);
	}
	void Controll()
	{
		//毎フレームのキャラクタ更新
		SAFE_EXE(m_pControllObj, Controll);
	}
};


typedef Controller*	 LPCONTROLLER;

#endif