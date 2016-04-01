#include "StartCountDown.h"
#include "GameMatchInfomation.h"

StartCountDown::StartCountDown(LPIEX2DOBJ texture):
m_pTexture(texture),
m_countSpeed(0.015f)
{
	Reset();
}

StartCountDown::~StartCountDown()
{
	delete m_pTexture;
}

void StartCountDown::Reset()
{
	for (int i = 0; i < count_texture_max; i++)
		m_count[i] = i*-1.0f;
}

bool StartCountDown::isStart()const
{
	return m_count[count_texture_max - 2] >= 1.0f;
}

void StartCountDown::Update()
{
	if (isStart())return;
	for (int i = 0; i < count_texture_max; i++)
	{
		m_count[i] += m_countSpeed;
	}
}

void StartCountDown::Render()
{
	if (isStart())return;


	static const TextureRect tex_rect[count_texture_max] =
	{
		{
			20, 10, 85, 110
		},
		{
			150, 10, 80, 110
		},
		{
			270, 10, 80, 110
		},
		{
			10, 130, 390, 130
		},
	};

	TextureRenderRect tex_render_rect;
	int w, h;

	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	tex_render_rect.render_rect.left = 0;
	tex_render_rect.render_rect.top = 240/h;
	tex_render_rect.render_rect.width = 200/w;
	tex_render_rect.render_rect.height = 200/h;



	float r;

	for (int i = 0; i < count_texture_max; i++)
	{
		tex_render_rect.read_tex_rect = tex_rect[i];
		if (i == 3)tex_render_rect.render_rect.width = 800/w;

		if (m_count[i]<0 || m_count[i]>1)continue;

		r = (m_count[i]<0.5f) ?
			(sinf(m_count[i] * PI)*0.5f) : (1-(sinf(m_count[i] * PI)*0.5f));

		tex_render_rect.render_rect.left = 
			
			-500+(int)(r*((float)iexSystem::ScreenWidth+1000));

		tex_render_rect.render_rect.left /= w;

		tex_render_rect.render_rect.left =
			tex_render_rect.render_rect.left - tex_render_rect.render_rect.width / 2;

		m_pTexture->Render(tex_render_rect);
	}
}