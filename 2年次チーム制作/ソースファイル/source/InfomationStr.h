#ifndef __INFOMATION_STR_H__
#define __INFOMATION_STR_H__

#include <map>
#include "CODBO.h"
#include "GameSystem\GameBase.h"

class InfomationStrMgr
{
public:
	enum INFOMATION_TYPE
	{
		_FLAG_GET,
		_FLAG_TOUCH_DOWN,
		_FLAG_DROP,
	};
	typedef std::pair<INFOMATION_TYPE, LPPLAYERMATCHDATA> STRINFO;
public:
	void SetInfo(const STRINFO& info);
	void Update();
	void Render(LPPLAYERMATCHDATA me);
};

#endif