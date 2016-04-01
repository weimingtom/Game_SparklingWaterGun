#ifndef __DEBUG_PARTICLE_H__
#define __DEBUG_PARTICLE_H__

#include "iextreme.h"

namespace debug_particle
{
	extern LPIEX2DOBJ g_pNumberTexture;

	void point(CRVector3 pos, CRVector3 move, float size, DWORD color, int liveFrame = 60);

	/**
	*@brief		円を描画
	*@param1 中心座標
	*@param2	円の半径
	*@param3円の密度（大体0.05くらいがベスト？）
	*@param4 色
	*/
	void circle(const Vector3& center_Pos, const float radius, const float&	dencity, const DWORD& color, const float& size);

	void NumberRender(CRVector3 pos, float size, int n, DWORD color = 0xFFFFFFFF);
}
#endif