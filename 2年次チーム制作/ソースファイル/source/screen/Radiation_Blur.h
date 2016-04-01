#ifndef __Radiation_BLUR_H_
#define __Radiation_BLUR_H_



class Radiation_Blur
{
private :
	typedef float RATIO;
	typedef struct tag_BlurParam
	{
		Vector2				centor_Pos;			//ブラーの中心座標
		bool				active;				//ブラーされているかどうかのフラグ
		float				set_frame;
		float				now_frame;
		RATIO				set_blur_power;		//ブラーの強さ
		const float			blur_power;			//このクラスのブラーの強さ
		/**
		*@param 1 最大補間速度
		*@param 2 ブラーの強さ
		*@param 3 ブラー最大強さ
		*@param 4 ブラーをかけ始める距離
		*@param 5	ブラーをかけない距離
		*/
		tag_BlurParam(float blur_power) :
			blur_power(blur_power),
			set_frame(0),
			now_frame(0),
			set_blur_power(0),
			active(false),
			centor_Pos(.5f, .5f){}
	}BlurParam;

private :
	iex2DObj*	m_tex;							//ブラー用テクスチャ
	BlurParam	m_param;			

private :
	void	Create_Blur_Tex(iex2DObj* screen, RATIO t);
	void	Blend_Screen(iex2DObj* screen, RATIO t);
public :
	Radiation_Blur();
	~Radiation_Blur();
	
public :
	void	Render(iex2DObj* main_Buffer);
	void	Begin(CRVector2 pos, float time, RATIO power);
};

#endif
