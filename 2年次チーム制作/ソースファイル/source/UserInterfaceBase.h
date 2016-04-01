#ifndef __USER_INTERFACE_BASE_H__
#define __USER_INTERFACE_BASE_H__

//**************************************************************************
//	ユーザーインターフェースの基底クラス
//**************************************************************************
#include "CODBOForwardDecl.h"

class UserInterfaceBase
{
public:
	virtual	~UserInterfaceBase(){}
	virtual void Update() = 0;
	virtual void Render(ControllObject& viewowner, TeamData& teamdata) = 0;
};

#endif