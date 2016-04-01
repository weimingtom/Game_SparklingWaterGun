#ifndef __Radiation_BLUR_H_
#define __Radiation_BLUR_H_



class Radiation_Blur
{
private :
	typedef float RATIO;
	typedef struct tag_BlurParam
	{
		Vector2				centor_Pos;			//�u���[�̒��S���W
		bool				active;				//�u���[����Ă��邩�ǂ����̃t���O
		float				set_frame;
		float				now_frame;
		RATIO				set_blur_power;		//�u���[�̋���
		const float			blur_power;			//���̃N���X�̃u���[�̋���
		/**
		*@param 1 �ő��ԑ��x
		*@param 2 �u���[�̋���
		*@param 3 �u���[�ő勭��
		*@param 4 �u���[�������n�߂鋗��
		*@param 5	�u���[�������Ȃ�����
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
	iex2DObj*	m_tex;							//�u���[�p�e�N�X�`��
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
