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
	Surface*	 shadow_Z;				//�V���h�E�p�f�v�X�o�b�t�@
	iex2DObj*	m_shdow_tex[2];						//���l�e�N�X�`��
	iex2DObj*	m_shadow_tex_MainStage[2];			//���C���X�e�[�W�p�̃V���h�E�}�b�v
	Matrix shadow_Projection[2];	//�J�X�P�[�h���Ƃ̕��s���e�s��
	float projection_W[2];					//�J�X�P�[�h���Ƃ̓��e��
	int	shadow_Size;
public :
	ShadowMap(int size, float near_w, float far_w);
	~ShadowMap();
	//@!<�J�X�P�[�h���Ƃ̓]���s��X�V
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
