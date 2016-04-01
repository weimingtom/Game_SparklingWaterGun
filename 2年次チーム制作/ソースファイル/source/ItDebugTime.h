#ifndef __IT_DEBUG_TIME_H__
#define __IT_DEBUG_TIME_H__
#include <cstdio>
#include <time.h>
#include <Windows.h>
#include <MMSystem.h>
#include "itDebugStringManager.h"

//********************************************************
//	�v���O�����o�ߎ��Ԍv�Z
//********************************************************

class ProgramSpeedCheck
{
private:
	static DWORD start_time;
public:
	static void		Begin()	//�v���J�n
	{
		start_time = timeGetTime();
	}
	static DWORD GetTime()	//�o�ߎ��Ԃ𓾂�
	{
		return timeGetTime() - start_time;
	}
};
DECLSPEC_SELECTANY DWORD ProgramSpeedCheck::start_time = 0;

#ifdef _DEBUG

#define FunctionDBGStr(color,str,...)\
{\
	char t[1000]; \
	sprintf_s<1000>(t, str, __VA_ARGS__); \
	itDebugStringManager::GetInstance().SetDebugStr(t, color); \
	itDebugStringManager::GetInstance().NextLine(); \
}

//�֐��̏������Ԃ��v��
#define FUNCTIONSPEEDCHECK(function) \
{\
	DWORD start_time = timeGetTime(); \
	function; \
	start_time = timeGetTime() - start_time; \
	FunctionDBGStr(0xFFFFFF00, "%s�̏������� %d ms ", #function, (int)(start_time)); \
}

//�֐��̏������Ԃ��v��
#define FUNCTIONSPEEDCHECK2(function,ret) \
{\
	DWORD start_time = timeGetTime(); \
	ret = function; \
	start_time = timeGetTime() - start_time; \
	FunctionDBGStr(0xFFFFFF00, "%s�̏������� %d ms ", #function, (int)(start_time)); \
}
#else

#define FUNCTIONSPEEDCHECK(function) function
#define FUNCTIONSPEEDCHECK2(function, ret) ret = function

#endif

#endif