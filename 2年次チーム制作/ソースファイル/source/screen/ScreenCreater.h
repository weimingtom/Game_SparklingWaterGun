#ifndef _SCREEN_CREATER_H_
#define _SCREEN_CREATER_H_

class ShadowMap;
class Radiation_Blur;
class Screen_Distortion;

class ScreenCreater_Base
{
public :
	virtual ~ScreenCreater_Base(){}
	virtual void	Initialize(const RECTI& g_bufferRect) = 0;
	virtual void	Create_Screen(iex2DObj* screen_Tex, iexView* view,ControllObject*pOwner) = 0;
	
	//debug
	virtual iex2DObj*	Get_Texture() = 0;
};


class ScreenCreater_DebugMatch : public ScreenCreater_Base
{
	enum buffer_index
	{
		toon,
		depth,
		reflect,
		water,
		num_buffer,
	};
private :
	iex2DObj**			m_sampler_Tex;
	ShadowMap**		m_shader_Map;
	Radiation_Blur**	m_radiation_Blur;
	Screen_Distortion**	m_Screen_Distortion;
	RECTI					m_rect;
	int						m_num_Player;
	int						m_rendering_Count;
private :
	void	Initialize_SamplarTex();
	void	Draw_OpacityObject(ControllObject*owner);
public :
	ScreenCreater_DebugMatch();
	~ScreenCreater_DebugMatch();

public :
	void	Initialize(const RECTI& g_bufferRect);
	void	Create_Screen(iex2DObj* screen_Tex, iexView* view,ControllObject*pOwner);
	void	Set_ViewPort();
	//debug
	iex2DObj*	Get_Texture();
	void				Render_ShadowMap();
};

#endif
