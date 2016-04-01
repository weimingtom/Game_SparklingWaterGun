#ifndef	__BILLBOARD_H__
#define	__BILLBOARD_H__

#include	"iextreme.h"
class	Billboard
{

public :
	static Matrix mat;	/*�Ƃ肠�����Ԃ����ޗp*/
								/*�摜�̒��S���W, �摜�͕ʂŗp�ӁA�摜�̕�	�A�摜�̍����Ax�����e�N�X�`�����W�i0.0~1.0f)y�����̃e�N�X�`�����W�ARS_COPY�ł��̂܂܏o��*/
	static void	Draw3D(Vector3 pos,iex2DObj* obj,float width,float height,float tu[2],float tv[2],u32 dwFlags);	//���S�ɒǏ]������
	static void	Draw_MatBoard(Matrix m,Vector3 pos,iex2DObj* obj,float width,float height,float tu[2],float tv[2],u32 dwFlags);	//�����̃}�g���N�X�ɒǏ]
	static void	Draw_VecBoard(Vector3 up,Vector3 right,Vector3 pos,iex2DObj* obj,float width,float height,float tu[2],float tv[2],u32 dwFlags);	//�����̃x�N�g���̔|��
};

#endif
