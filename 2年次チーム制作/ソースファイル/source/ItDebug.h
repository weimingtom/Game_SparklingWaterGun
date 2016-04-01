#ifndef __IT_DEBUG_H__
#define __IT_DEBUG_H__


//*****************************************************************
//new�I�[�o�[���[�h
//*****************************************************************

#define _CRTDBG_MAP_ALLOC

#include <cstdlib>
#include <cassert>
#include <crtdbg.h>
#include <new>
#include <memory>
#include "ItDebugTime.h"
//*****************************************************************
//�������[���[�N�����`�F�b�N
//*****************************************************************

//*****************************************************************
//		��O�ɂ���đ��o�����N���X
//*****************************************************************

template<class _error_type>
class ErrorHandlerBase
{
	typedef _error_type ERROR_TYPE_VALUE;
private:
	char				str[512];
	ERROR_TYPE_VALUE	data;
public:
	ErrorHandlerBase(
		const char* errstr,				//�G���[���b�Z�[�W
		const ERROR_TYPE_VALUE& data):	//error�l
		data(data)
	{
		str[0] = '\0';
		if (errstr)
		{
			strcpy_s(str, 512, errstr);
		}
	}
	~ErrorHandlerBase()
	{

	}
	ErrorHandlerBase(const ErrorHandlerBase& errh)
	{
		strcpy_s(str, 512, errh.GetStr());
	}
public:
	const char*	GetStr()const
	{
		return str;
	}
	const ERROR_TYPE_VALUE&	GetErrData()const
	{
		return data;
	}
};

//�Q�[�������I��error
typedef ErrorHandlerBase<int> Error_GameExit;


//#define _IT_DEBUG

#ifdef _IT_DEBUG


#define MyMemoryLeakCheck _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)

//*****************************************************************
//printf�Ɠ����`���ŏo�̓E�B���h�E�ɕ\��
//*****************************************************************
#define MyDebugString(str,...)\
{\
	TCHAR temp[512]; \
	sprintf_s<512>(temp, str, __VA_ARGS__); \
	OutputDebugString(temp); \
}

//*****************************************************************
//	���b�Z�[�W�{�b�N�X��printf�݂����ɕ\������
//*****************************************************************
#define MessageBoxPlus(str,...)\
{\
	char t[512]; \
	sprintf_s<512>(t, str, __VA_ARGS__);\
	MessageBox(0, t, "MessageBoxPlus", MB_OK);\
}

#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)

#else

#define MyDebugString(str,...)

#define MessageBoxPlus(str,...)

#define MyMemoryLeakCheck

#endif	//_IT_DEBUG


//*****************************************************************
//printf�Ɠ����`���ŃA�T�[�g�ɕ������\��������
//*****************************************************************

#define __NotMyAssert

#ifndef __NotMyAssert

#define MyAssert(expr,str,...)\
{\
if (!(expr)){\
	\
	\
	char t[256], ti[600]; \
	sprintf_s<256>(t, str, __VA_ARGS__); \
	sprintf_s<600>(ti, "�@%s�@�̍s�ԍ��@%d�@�ŃG���[���������܂��� \n %s \n��(%s) \n���s�𑱂��܂����H", __FILE__, __LINE__, t, #expr); \
	MessageBeep(MB_OK); \
if (MessageBox(0, ti, "MyAssertError!!!", MB_YESNO) == IDNO){\
	Error_GameExit err("GameExitError", 0); \
	throw err; \
}\
}\
}
#else

#define MyAssert(expr,str,...) expr

#endif

#endif	//eof