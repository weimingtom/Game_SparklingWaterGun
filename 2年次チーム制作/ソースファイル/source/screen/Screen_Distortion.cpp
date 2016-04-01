#include	"iextreme.h"
#include	"Screen_Distortion.h"
#include	"../GameMatchInfomation.h"
#include	"../system\System.h"


Screen_Distortion::Screen_Distortion() : m_animation_Texture(nullptr),
m_sampler_Buffer(nullptr), m_sampler_Rect(), m_screen_Rect()
{

}

Screen_Distortion::~Screen_Distortion()
{
//	SAFE_DELETE(m_animation_Texture);
	SAFE_DELETE(m_sampler_Buffer);
}

void Screen_Distortion::Initialize(iex2DObj* animeTexture, const RECTI& sampler_BufferSize,
																const int animation_Speed)
{
	m_sampler_Rect.x = sampler_BufferSize.x;
	m_sampler_Rect .y= sampler_BufferSize.y;
	m_sampler_Buffer = new iex2DObj(m_sampler_Rect.x, m_sampler_Rect.y, IEX2D_RENDERTARGET);
	m_animation_Texture = animeTexture;
	
	m_screen_Rect.x = sampler_BufferSize.width;
	m_screen_Rect.y = sampler_BufferSize.height;

	//総UVIndex計算
	int num_U = m_animation_Texture->GetWidth() / sampler_BufferSize.x;
	int num_V = m_animation_Texture->GetHeight() / sampler_BufferSize.y;
	int num_Index = num_U * num_V;
	m_param.Initallize(animation_Speed, num_Index,num_U,num_V);
}

void Screen_Distortion::SetColor(COLOR color)
{
	m_param.color = color;
}

void Screen_Distortion::Active()
{
	m_param.isActive = true;
	m_param.progress_Frame = 0;
	m_param.current_UVIndex = 0;
}

void Screen_Distortion::Render(iex2DObj* screen_Buffer, iexView* view)
{
	if (!m_param.isActive)
		return;
#if 1
	
	if (!Create_SamplerBuffer(view))
		return;
	screen_Buffer->RenderTarget();
	Distortion(screen_Buffer);
#else

	m_param.progress_Frame++;
	m_param.current_UVIndex = m_param.progress_Frame;

	if (m_param.end_UVIndex <= m_param.current_UVIndex)
	{
		m_param.isActive = false;
		return;
	}

	RECTI rect = GAMEMATCHINFO.GetScreenSize();
	LVERTEX VertexArray[4];

	VertexArray[0].x = 0;
	VertexArray[0].y = 0;

	VertexArray[1].x = 1;
	VertexArray[1].y = 0;

	VertexArray[2].x = 0;
	VertexArray[2].y = 1;

	VertexArray[3].x = 1;
	VertexArray[3].y = 1;


	for (int i = 0; i < 4; ++i)
	{
		VertexArray[i].color = m_param.color;
		VertexArray[i].z = 0;
	}

	const int sell_num = m_param.current_UVIndex;
	const int w = 16;
	const int h = 16;
	const Vector2 sell_size(1.0f / 16.0f, 1.0f / 16.0f);

	VertexArray[0].tu = sell_size.x*(float)(sell_num % w);
	VertexArray[0].tv = sell_size.y*(float)(sell_num / w);

	VertexArray[1].tu = VertexArray[0].tu + sell_size.x;
	VertexArray[1].tv = VertexArray[0].tv;

	VertexArray[2].tu = VertexArray[0].tu;
	VertexArray[2].tv = VertexArray[0].tv + sell_size.y;

	VertexArray[3].tu = VertexArray[1].tu;
	VertexArray[3].tv = VertexArray[2].tv;


	iexPolygon::Render3D(VertexArray, 2, m_animation_Texture, shader, "screenposition_effect");
#endif
}

void	Screen_Distortion::Distortion(iex2DObj* screen_Buffer)
{
	shader2D->SetValue("AnimeDistortion", m_sampler_Buffer);

	screen_Buffer->Render(0, 0, m_screen_Rect.x, m_screen_Rect.y,
											0, 0, m_screen_Rect.x, m_screen_Rect.y,
												shader2D, "distortion");

}

bool	Screen_Distortion::Create_SamplerBuffer(iexView* view)
{
	m_param.progress_Frame++;
	m_param.current_UVIndex = m_param.progress_Frame / m_param.animation_Speed;
	
	if (m_param.end_UVIndex <= m_param.current_UVIndex)
	{
		m_param.isActive = false;
		return false;
	}

	m_sampler_Buffer->RenderTarget();
	
	view->SetViewport(0, 0, m_sampler_Rect.x, m_sampler_Rect.x);

	m_animation_Texture->Render(0, 0, m_sampler_Rect.x, m_sampler_Rect.y,		//画面計算
		m_sampler_Rect.x * (m_param.current_UVIndex % m_param.num_Width),	//ここからテクスチャ座標計算
		m_sampler_Rect.y * (m_param.current_UVIndex / m_param.num_Height),
		m_sampler_Rect.x, m_sampler_Rect.y);

	view->SetViewport(0, 0, m_screen_Rect.x, m_screen_Rect.y);

	return true;
}

