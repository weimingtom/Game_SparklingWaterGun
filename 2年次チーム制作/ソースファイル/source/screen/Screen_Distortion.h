#pragma once

struct int2
{
	int x;
	int y;
	int2() : x(0), y(0){}
	int2(const int x, const int y) :x(x), y(y){}
};

class Screen_Distortion
{
private: 
	 struct
	{
		 bool isActive;
		 int current_UVIndex;
		 int progress_Frame;
		 int animation_Speed;
		 int end_UVIndex;
		 int num_Width;
		 int num_Height;
		 COLOR color;
		 void	Initallize(const int animationSpeed,const int endUVIndex,
								const int num_W,const int num_H)
		 {
			 current_UVIndex = 0;	progress_Frame = 0; animation_Speed = animationSpeed;
			 end_UVIndex = endUVIndex; isActive = false; num_Width = num_W;
			 num_Height = num_H;
			 color = 0xFFFFFFFF;
		 }
	} m_param;
private: 
	iex2DObj*	m_sampler_Buffer;		//UVアニメーションさせたコマを張り付けるSamplerBuffer
	iex2DObj*	m_animation_Texture;	//UVアニメーションさせるTexture
	int2				m_sampler_Rect;
	int2				m_screen_Rect;

private :
	bool	Create_SamplerBuffer(iexView* view);
	void	Distortion(iex2DObj* screen_Buffer);
public :
	Screen_Distortion();
	~Screen_Distortion();
	
public :
	/**
	*@brief param samplarBufferSizeのxとyにひとつのコマの大きさ : widthとHeightにスクリーンサイズ
	*/
	void	Initialize(iex2DObj* animeTexture, const RECTI& sampler_BufferSize,
						const int animation_Speed);
	void	SetColor(COLOR color);
	void	Active();
	void	Render(iex2DObj* screen_Buffer, iexView* view);
};
