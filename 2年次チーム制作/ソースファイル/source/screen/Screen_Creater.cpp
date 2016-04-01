#include	"..\All.h"

ScreenCreater_DebugMatch::ScreenCreater_DebugMatch() :
m_rect(), m_sampler_Tex(nullptr), m_shader_Map(nullptr)
{}

void ScreenCreater_DebugMatch::Initialize(const RECTI& g_bufferRect)
{
	m_rect = g_bufferRect;
	m_sampler_Tex = new iex2DObj*[buffer_index::num_buffer];
	m_num_Player = GAMEMATCHINFO.GetMatchData().player_num;
	Initialize_SamplarTex();
	m_rendering_Count = 0;
}

void	ScreenCreater_DebugMatch::Initialize_SamplarTex()
{
	RECTI rc = GAMEMATCHINFO.GetScreenSize();
	m_shader_Map = new ShadowMap*[m_num_Player];
	m_radiation_Blur = new Radiation_Blur*[m_num_Player];
	m_Screen_Distortion = new Screen_Distortion*[m_num_Player];

	rc.x = 512;
	rc.y = 256;

	for (int i = 0; i < m_num_Player; i++)
	{
#if LIGHTMAP
		m_shader_Map[i] = new ShadowMap(1024, 10, 90);
#else
		m_shader_Map[i]=new ShadowMap(1700, 20, 90);
#endif
		m_radiation_Blur[i] = new Radiation_Blur();

		m_Screen_Distortion[i] = new Screen_Distortion();
		m_Screen_Distortion[i]->Initialize(
			GAMEMATCHINFO.GetTextureMgr().GetTexturePtr(TextureManager::WATER_DROP_ANIMATION),
			rc,
			1
			);
	}
	m_sampler_Tex[buffer_index::toon] = new iex2DObj("DATA\\toon.png");
	m_sampler_Tex[buffer_index::reflect] = new iex2DObj(m_rect.width, m_rect.height, IEX2D_RENDERTARGET);
	m_sampler_Tex[buffer_index::depth] = new iex2DObj(m_rect.width, m_rect.height, IEX2D_RENDERTARGET);
	m_sampler_Tex[buffer_index::water] = new iex2DObj(m_rect.width, m_rect.height, IEX2D_RENDERTARGET);
}

ScreenCreater_DebugMatch::~ScreenCreater_DebugMatch()
{
	for (int i = 0; i < buffer_index::num_buffer; i++)
		SAFE_DELETE(m_sampler_Tex[i]);
	
	delete[] m_sampler_Tex;
	for (int i = 0; i < m_num_Player; i++)
	{
		SAFE_DELETE(m_shader_Map[i]);
		SAFE_DELETE(m_radiation_Blur[i]);
		SAFE_DELETE(m_Screen_Distortion[i]);
	}
	delete[] m_Screen_Distortion;
	delete[] m_radiation_Blur;
	delete[] m_shader_Map;
}

void	ScreenCreater_DebugMatch::Set_ViewPort()
{
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = m_rect.width;
	viewport.Height = m_rect.height;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;
	iexSystem::GetDevice()->SetViewport(&viewport);
}

void	ScreenCreater_DebugMatch::Draw_OpacityObject(ControllObject*owner)
{
	Set_ViewPort();
	//マップオブジェクトを描画
	MAP_MANAGER.Render();
	//コントロール可能なオブジェクトを描画
	CONTROLLOBJMGR.Render(*owner, *ControllObjFunc::GetTeamData(owner));
	//iexParticle::Render();
	
	//field_Debug->Render_GoalNode();
	//field_Debug->Reder_GoalRoute();
	//field_Debug->Render_AllEdge();
//	field_Debug->Render_Edge(ControllObjFunc::GetPos(owner));
//	field_Debug->Render_Division(ControllObjFunc::GetPos(owner));
}

void	ScreenCreater_DebugMatch::Create_Screen(
	iex2DObj* screen_Tex,
	iexView* view,
	ControllObject*owner)
{
	ProgramSpeedCheck::Begin();
	Vector3 owner_Pos = ControllObjFunc::GetPos(owner);
	Vector3 c_pos = ControllObjFunc::GetCamera(owner)->GetPos();
	const ControllObjectParam& cntParam = ControllObjFunc::GetParam(owner);


	m_shader_Map[m_rendering_Count]->Create(
		Vector3(1.0f, -1.0f, -0.5f),
		owner_Pos,
		owner,
		ControllObjFunc::GetTeamData(owner)
		);
	screen_Tex->RenderTarget();
	m_sampler_Tex[buffer_index::depth]->RenderTarget(1);
	
	//値渡し
	view->Set(c_pos, Vector3(ControllObjFunc::GetCamera(owner)->GetTarget()));
	shader->SetValue("Camera_Pos", c_pos);
	shader->SetValue("Toon", m_sampler_Tex[buffer_index::toon]);
	float fa, n, f;
	view->GetViewParameters(n, fa, f);
	shader->SetValue("Zfar", fa);

	view->Activate();
	matView = ControllObjFunc::GetCamera(owner)->GetMatrix();
	view->Clear();
	Draw_OpacityObject(owner);
	shader->SetValue("Depth", m_sampler_Tex[buffer_index::depth]);
	BulletManager* mng = (BulletManager*)IDMANAGER.GetPointer(game_id::id_bullet_manager);
	//shader->SetValue("ScreenBuffer", screen_Tex);

	mng->Render();
	WaterPoolManager* mg = (WaterPoolManager*)IDMANAGER.GetPointer(game_id::id_water_pool_manager);
	mg->Render();

	POLYGONEFFECT.UpdateBillboardVertex();
	POLYGONEFFECT.Render();


	shader->SetValue("ScreenBuffer", screen_Tex);
	owner->RenderUI(*owner, *ControllObjFunc::GetTeamData(owner));

	//爆発ブラー適用
	if (cntParam.doexplode_blur)
	{
		ControllObjFunc::ResetBlurParam(owner);
		m_radiation_Blur[m_rendering_Count]->Begin(cntParam.explode_pos, cntParam.blur_time, cntParam.blur_power);
	}

	//水の滴るエフェクト
	if (cntParam.do_water_drop)
	{
		ControllObjFunc::SetWaterDropParam(owner, false);
		m_Screen_Distortion[m_rendering_Count]->SetColor(CODMgr::GetTeamColor(ControllObjFunc::GetTeamType(owner)));
		m_Screen_Distortion[m_rendering_Count]->Active();
	}

	m_Screen_Distortion[m_rendering_Count]->Render(screen_Tex, view);
	
	m_radiation_Blur[m_rendering_Count]->Render(screen_Tex);
	

	m_rendering_Count = ++m_rendering_Count % m_num_Player;
}

iex2DObj*ScreenCreater_DebugMatch::Get_Texture()
{
	return m_sampler_Tex[buffer_index::depth];
}

void		ScreenCreater_DebugMatch::Render_ShadowMap()
{
	m_shader_Map[0]->Render_Debug(0);
}