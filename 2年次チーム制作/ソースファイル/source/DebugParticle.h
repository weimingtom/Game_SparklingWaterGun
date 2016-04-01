#ifndef __DEBUG_PARTICLE_H__
#define __DEBUG_PARTICLE_H__

#include "iextreme.h"

namespace debug_particle
{
	extern LPIEX2DOBJ g_pNumberTexture;

	void point(CRVector3 pos, CRVector3 move, float size, DWORD color, int liveFrame = 60);

	/**
	*@brief		�~��`��
	*@param1 ���S���W
	*@param2	�~�̔��a
	*@param3�~�̖��x�i���0.05���炢���x�X�g�H�j
	*@param4 �F
	*/
	void circle(const Vector3& center_Pos, const float radius, const float&	dencity, const DWORD& color, const float& size);

	void NumberRender(CRVector3 pos, float size, int n, DWORD color = 0xFFFFFFFF);
}
#endif