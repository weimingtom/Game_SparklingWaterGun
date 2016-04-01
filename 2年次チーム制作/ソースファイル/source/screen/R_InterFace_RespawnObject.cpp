#include	"..\All.h"

R_InterFace_RespawnObject::R_InterFace_RespawnObject() :
R_InterFace_MapObject("character", "ShadowBuf")
{}

void	R_InterFace_RespawnObject::Render(MapObjectInterface* owner)
{

	owner->Get_Mesh()->Render(shader, m_default_Technique, owner->Get_Matrix());
}

void	R_InterFace_RespawnObject::Render_ShadowMap(MapObjectInterface* owner)
{
	owner->Get_Mesh()->Render(shader, m_shadowmap_Technique, owner->Get_Matrix());
}