#ifndef	__SELFSHADOW_H__
#define __SELFSHADOW_H__

#include "iextreme.h"
#include "CODBOForwardDecl.h"

class ShadowMap
{
	enum
	{
		d_NEAR,
		d_FAR,
	};
private :
	Surface*	 shadow_Z;				//シャドウ用デプスバッファ
	iex2DObj*	m_shdow_tex[2];						//ｚ値テクスチャ
	iex2DObj*	m_shadow_tex_MainStage[2];			//メインステージ用のシャドウマップ
	Matrix shadow_Projection[2];	//カスケードごとの平行投影行列
	float projection_W[2];					//カスケードごとの投影幅
	int	shadow_Size;
public :
	ShadowMap(int size, float near_w, float far_w);
	~ShadowMap();
	//@!<カスケードごとの転送行列更新
	void	Update_ShadowProjecton(Vector3 dir,const Vector3& player_Pos);
	void	Create(Vector3 dir, const Vector3& pos,ControllObject*	pViewOwner,TeamData* teamData);
	//void Render2D(int sc_X, int sc_Y, int sc_W, int sc_H, int te_X, int te_Y)
	//{
	//	tex_Z[0]->Render(sc_X, sc_Y, sc_W, sc_H, te_X, te_Y, shadow_Size, shadow_Size);
	//}
	//void Insert_Object(iexMesh* in){ draw_List.push_back(in); }

	void Render_Debug(int i );

};

#endif
