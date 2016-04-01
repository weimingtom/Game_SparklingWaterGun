#ifndef	__BILLBOARD_H__
#define	__BILLBOARD_H__

#include	"iextreme.h"
class	Billboard
{

public :
	static Matrix mat;	/*とりあえずぶっこむ用*/
								/*画像の中心座標, 画像は別で用意、画像の幅	、画像の高さ、x方向テクスチャ座標（0.0~1.0f)y方向のテクスチャ座標、RS_COPYでそのまま出す*/
	static void	Draw3D(Vector3 pos,iex2DObj* obj,float width,float height,float tu[2],float tv[2],u32 dwFlags);	//完全に追従するやつ
	static void	Draw_MatBoard(Matrix m,Vector3 pos,iex2DObj* obj,float width,float height,float tu[2],float tv[2],u32 dwFlags);	//引数のマトリクスに追従
	static void	Draw_VecBoard(Vector3 up,Vector3 right,Vector3 pos,iex2DObj* obj,float width,float height,float tu[2],float tv[2],u32 dwFlags);	//引数のベクトルの板ポリ
};

#endif
