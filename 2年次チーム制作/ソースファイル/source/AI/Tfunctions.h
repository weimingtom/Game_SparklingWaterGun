#ifndef __T_FUNCTIONS_H__
#define __T_FUNCTIONS_H__
#include "iextreme.h"

#define break_point(break_time) if(GAMEMATCHINFO.GetMatchTime() == break_time)


#pragma warning(disable : 4150)

#include	<type_traits>
////enum�N���X�̈��S�ȃL���X�g(�����^�v�ɕϊ�)

#if 0
template<class T>
inline typename std::underlying_type<T>::type enum_cast(T t)
{

	return static_cast<typename std::underlying_type<T>::type>(t);
}
#else
template<class T>
inline int enum_cast(T t)
{

	return static_cast<int>(t);
}

#endif
/**
*@brief �ӂ��̃x�N�g����������ɂ���Ē������ꂽcos�l�𓾂�
*/

inline float Harf_DotCos(const Vector3& v1, const  Vector3& v2, const float& rate, const float& adjust)
{
	float cos = Vector3Dot(v1, v2);
	cos *= rate;
	cos += adjust;
	return cos;
}

inline float Min(const float a, const float b)
{
	return (a < b) ? a : b;
}

inline float Max(const float a, const float b)
{
	return (a > b) ? a : b;
}

inline float Clamp(const float&val, const float& max, const float& min)
{
	return Min(Max(min, val), max);
}

inline float	Cross2D(const Vector3& v1, const Vector3& v2)
{
	return v1.z *  v2.x - v1.x * v2.z;
}


#endif