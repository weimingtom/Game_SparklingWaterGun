#pragma once

#ifdef _CP11

#define _ENUM_BEGIN( enum_name, temp_name)		\
enum class temp_name :int{

#define _ENUM_END( enum_name, temp_name)			\
};	typedef temp_name enum_name;

#else

#pragma warning(disable:4482)

#define _ENUM_BEGIN( enum_name, temp_name)		\
namespace temp_name {							\
enum enum_name{

#define _ENUM_END( enum_name, temp_name)		\
};												\
};	typedef temp_name::enum_name enum_name;

#endif
