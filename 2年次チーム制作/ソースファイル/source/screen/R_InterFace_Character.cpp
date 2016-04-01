#include	"..\All.h"


R_InterFace_Character::R_InterFace_Character()
:Rendering_Interface("character", "ShadowBuf")
{
	m_color[outline_color::RED] = .0f;
	m_color[outline_color::GREEN] = .0f;
	m_color[outline_color::BLUE] = 1.0f;
	m_color[outline_color::ALPHA] = 1.0f;
	m_outline_Size = 0.1f;
}

void R_InterFace_Character::Render(CharacterRenderParam* owner)
{
	enum RenderType
	{
		Me,
		TeamMember,
		ViewEnemy,
		Enemy
	};

	RenderType rt;
	TeamViewData::DataParam dp;
	RATIO alpha = 0; 
	const COLORf color = ChrFunc::GetParam(&owner->character).m_OutLineColor; 
	COLORf see_through(CODMgr::GetTeamColor(ControllObjFunc::GetMatchData(&owner->character)->team_type));
	LPCHARACTERMODELBASE pModel = ChrFunc::GetModelPtr(&owner->character);


	m_outline_Size = color.a;

	if ((&owner->character) == (&owner->viewOwner))
	{
		rt = Me;
	}
	else if (
		ControllObjFunc::GetMatchData(&owner->character)->team_type !=
		ControllObjFunc::GetMatchData(&owner->viewOwner)->team_type
		)
	{
		if (owner->OwnerteamData.m_Viewdata.GetViewData(
			owner->character.GetId(),
			dp,
			alpha))
		{
			rt = ViewEnemy;
		}
		else
			rt = Enemy;
	}
	else
	{
		rt = TeamMember;
		alpha = 0.7f;
	}

	switch (rt)
	{
	case Me:

		m_color[outline_color::RED] = color.r;
		m_color[outline_color::GREEN] = color.g;
		m_color[outline_color::BLUE] = color.b;
		m_color[outline_color::ALPHA] = 1.0f;
		break;/*
	case TeamMember:
		m_color[outline_color::RED] = 0.0f;
		m_color[outline_color::GREEN] = 0.0f;
		m_color[outline_color::BLUE] = 1.0f;
		m_color[outline_color::ALPHA] = 1.0f;
		break;
	case ViewEnemy:
	case Enemy:
		m_color[outline_color::RED] = 1.0f;
		m_color[outline_color::GREEN] = .0f;
		m_color[outline_color::BLUE] = .0f;
		m_color[outline_color::ALPHA] = 1.0f;
		break;
		*/
	default:
		m_color[outline_color::RED] = 0.0f;
		m_color[outline_color::GREEN] = 0.0f;
		m_color[outline_color::BLUE] = 0.0f;
		m_color[outline_color::ALPHA] = 1.0f;
		break;
	}
	

	shader->SetValue("outlinecolor_A", alpha);
	
	shader->SetValue("outlinecolor_R", see_through.r);
	shader->SetValue("outlinecolor_G", see_through.g);
	shader->SetValue("outlinecolor_B", see_through.b);

	pModel->Render(&owner->character, "see_through");

	shader->SetValue("outlinecolor_A", m_color[outline_color::ALPHA]);
	shader->SetValue("outlinecolor_R", m_color[outline_color::RED]);
	shader->SetValue("outlinecolor_G", m_color[outline_color::GREEN]);
	shader->SetValue("outlinecolor_B", m_color[outline_color::BLUE]);
	shader->SetValue("OutlineSize", m_outline_Size);

	pModel->Render(&owner->character, m_default_Technique);

	const float PowerUpAlpha = ChrFunc::GetParam(&owner->character).UpValue.PowerUpAlpha;

	if (PowerUpAlpha > 0 && (rt == Me || rt == TeamMember))
	{
		shader->SetValue("outlinecolor_A", ShareRatioObjectBase::GetSeeThroughAlpha()*PowerUpAlpha);

		shader->SetValue("outlinecolor_R", 1);
		shader->SetValue("outlinecolor_G", 1);
		shader->SetValue("outlinecolor_B", 1);

		pModel->Render(&owner->character, "see_through");
	}

}

void	R_InterFace_Character::Render_ShadowMap(CharacterRenderParam* owner)
{
	LPCHARACTERMODELBASE model=ChrFunc::GetModelPtr(&owner->character);

	if (model)
	{
		model->Render(
			&owner->character,
			m_shadowmap_Technique
			);
	}
}


