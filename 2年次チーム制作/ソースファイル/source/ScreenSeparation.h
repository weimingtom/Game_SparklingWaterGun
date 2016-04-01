#ifndef __SCREEN_SEPARATION_H__
#define __SCREEN_SEPARATION_H__

#include <list>
#include "Camera.h"
#include "CODBOForwardDecl.h"

class MatchTypeStateBase;
typedef MatchTypeStateBase *LPMatchTypeStateBase;
class ShadowMap;
class ControllObject;
class ScreenCreater_Base;
//**********************************************************************
//	画面分割描画を担当するクラス(画面一つでも描画するけども)
//**********************************************************************

class ScreenSeparation
{
public:
private:
	enum{
		RENDER_SCREEN_MAX = 9		//同時に描画できる画面の最大数
	};
private:
	int										num_Player;
	iex2DObj**							m_pScreenTextures;	//画面テクスチャたち
	std::list<RECTI>					m_ScreenInfo;															//各スクリーンをどこに描画するか
	iexView									m_view;																		//matViewとかを設定するので
	RECTI									m_screen_texture_size;											//一つ一つのスクリーンのサイズ
	ScreenCreater_Base*			m_screen_create;

public:
	ScreenSeparation(const int& num_P, ScreenCreater_Base* creater);
	~ScreenSeparation();
public:
	//描画するスクリーンを設定する
	void SetScreen(const  std::list<RECTI>&	screen_positions);

	//画面テクスチャ作成
	void CreateScreenTexture(
		CONTROLLOBJLIST&		obj_list,				//カメラリストここに入っている分だけ画面を表示する)
		LPMatchTypeStateBase		pDrawMatchState		//描画する関数の入ったマッチステートクラスのポインター
		);

	//画面を描画
	void DrawScreen();

};
void CreateScreenRect(std::list<RECTI>* pOut, int nPlayer);

#endif