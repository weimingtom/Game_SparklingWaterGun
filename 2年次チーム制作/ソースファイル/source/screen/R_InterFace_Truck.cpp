#include	"..\All.h"

R_InterFace_Truck::R_InterFace_Truck() :
R_InterFace_MapObject("character", "ShadowBuf")
{}

void	R_InterFace_Truck::Render(MapObjectInterface* owner)
{

	owner->Get_Mesh()->Render(shader, m_default_Technique, owner->Get_Matrix());
}

void	R_InterFace_Truck::Render_ShadowMap(MapObjectInterface* owner)
{
	owner->Get_Mesh()->Render(shader, m_shadowmap_Technique, owner->Get_Matrix());
}