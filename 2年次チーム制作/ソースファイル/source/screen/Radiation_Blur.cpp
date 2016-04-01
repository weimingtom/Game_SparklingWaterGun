#include	"..\All.h"

Radiation_Blur::Radiation_Blur()
//:m_param(.02f,0.0f,15.0f,0.5f,30.0f) 
:m_param(18.0f)
{
	//Rect取得
	RECTI rect = GAMEMATCHINFO.GetScreenSize();
	m_tex = new iex2DObj(rect.width,rect.height, IEX2D_RENDERTARGET);
}

Radiation_Blur::~Radiation_Blur()
{
	delete m_tex;
}

void	Radiation_Blur::Create_Blur_Tex(iex2DObj* screen, RATIO t)
{
	//ターゲット切り替え
	m_tex->RenderTarget();
	RECTI rect = GAMEMATCHINFO.GetScreenSize();
	//値渡し
	float x = 1.0f / rect.width;
	float y = 1.0f / rect.height;

	shader2D->SetValue("center_pos_x", m_param.centor_Pos.x);
	shader2D->SetValue("center_pos_y", m_param.centor_Pos.y);
	shader2D->SetValue("tex_offset_x", x);
	shader2D->SetValue("tex_offset_y", y);
	shader2D->SetValue("blur_power", t*m_param.blur_power);
	//ブラーをかける
	screen->Render(0,0,rect.width,rect.height,0,0,rect.width,rect.height,shader2D, "radiation_blur");
	screen->RenderTarget();
}

void	Radiation_Blur::Blend_Screen(iex2DObj* screen, RATIO t)
{
	shader2D->SetValue("blend_rate", t);
	shader2D->SetValue("Blend_Tex", m_tex);
	RECTI rect = GAMEMATCHINFO.GetScreenSize();
	screen->Render(0, 0, rect.width, rect.height, 0, 0, rect.width, rect.height,shader2D, "blend");
}

void	Radiation_Blur::Render(iex2DObj* screen)
{
	if (!m_param.active || m_param.set_frame <= 0)
		return;

	RATIO t = m_param.now_frame / m_param.set_frame;
	
	Create_Blur_Tex(screen, t);
	Blend_Screen(screen, t);

	m_param.now_frame -= 1;

	if (m_param.now_frame <= 0)
	{
		m_param.now_frame = .0f;
		m_param.active = false;
	}
}

void	Radiation_Blur::Begin(CRVector2 pos, float time, RATIO power)
{
	m_param.active = true;
	m_param.centor_Pos = pos;
	m_param.set_blur_power = power;
	m_param.set_frame = m_param.now_frame = time;

}