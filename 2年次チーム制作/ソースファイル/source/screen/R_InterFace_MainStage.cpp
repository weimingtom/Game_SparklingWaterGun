#include	"..\All.h"

R_InterFace_MainStage::R_InterFace_MainStage() :
R_InterFace_MapObject("mainstage","ShadowBuf_MainStage")
{}

void	R_InterFace_MainStage::Render(MapObjectInterface* owner)
{

	owner->Get_Mesh()->Render(shader, m_default_Technique);
}

void	R_InterFace_MainStage::Render_ShadowMap(MapObjectInterface* owner)
{
	owner->Get_Mesh()->Render(shader, m_shadowmap_Technique,
													owner->Get_Matrix());
}