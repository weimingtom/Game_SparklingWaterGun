#include	"..\All.h"

R_InterFace_Poster::R_InterFace_Poster() :
RBase_InterFace_ShareObject("character", "ShadowBuf")
{}

void	R_InterFace_Poster::Render(MapObjectInterface* owner)
{
	owner->Get_Mesh()->Render(shader, m_default_Technique,owner->Get_Matrix());
}

void	R_InterFace_Poster::Render_ShadowMap(MapObjectInterface* owner)
{
	owner->Get_Mesh()->Render(shader, m_shadowmap_Technique, owner->Get_Matrix());
}