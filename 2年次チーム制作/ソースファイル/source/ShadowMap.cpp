#include	"All.h"


ShadowMap::ShadowMap(int size, float near_w, float far_w) :shadow_Size(size)
{
	//サーフェイス取得
//	iexSystem::GetDevice()->GetRenderTarget(0, &main_Buckbuffer);
	iexSystem::GetDevice()->CreateDepthStencilSurface(shadow_Size, shadow_Size, D3DFMT_D24S8,
												D3DMULTISAMPLE_NONE, 0, FALSE, &shadow_Z, NULL);
	projection_W[0] = near_w;
	projection_W[1] = far_w;
	for (int i = 0; i < 2; i++)
	{
		m_shdow_tex[i] = new iex2DObj(shadow_Size, shadow_Size, IEX2D_FLOAT2);
		m_shadow_tex_MainStage[i] = new iex2DObj(shadow_Size, shadow_Size, IEX2D_FLOAT2);
	}

}

ShadowMap::~ShadowMap()
{
	//draw_List.clear();
	for (int i = 0; i < 2; i++){
		delete m_shdow_tex[i];
		delete m_shadow_tex_MainStage[i];
	}
	shadow_Z->Release();
}

void ShadowMap::Create(
	Vector3 dir, const Vector3& player_Pos,
	ControllObject*	pViewOwner, TeamData* teamData)
{
	//デプスバッファ保存
	Surface* orgZ;
	iexSystem::GetDevice()->GetDepthStencilSurface(&orgZ);
	iexSystem::GetDevice()->SetDepthStencilSurface(shadow_Z);


	//ビューポート設定
	D3DVIEWPORT9 vp = { 0, 0, shadow_Size, shadow_Size, 0, 1.0f };
	iexSystem::GetDevice()->SetViewport(&vp);
	Matrix  work_Projecton;
	Vector3 up(0, 1, 0);
	float dist = 30.0f;
	dir.Normalize();
	float near_p = 0;
	float far_p = 40.0f;
	for (int i = 0; i < 2; i++){
		D3DXMatrixIdentity(&work_Projecton);
		Vector3 pos = player_Pos - dir * dist;
		//ビュー行列
		Vector3 workpos = player_Pos;
		LookAtLH(shadow_Projection[i], pos, workpos, up);
		//平行投影行列作成
		D3DXMatrixOrthoLH(&work_Projecton, projection_W[i], projection_W[i], near_p, far_p);
		near_p += 10;
		far_p *= 4;
		dist *= 4.0f;
		//work *= 2;
		//合成する
		shadow_Projection[i] *= work_Projecton;

		//テクスチャ切り替え
		shader->SetValue("ShadowProjection", shadow_Projection[i]);
		m_shdow_tex[i]->RenderTarget(0);
		m_shadow_tex_MainStage[i]->RenderTarget(1);
		//レンダリング
		iexSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0xFFFFFFFF, 1.0f, 0);	

		//マップオブジェクトを描画
		MAP_MANAGER.Render_ShadowMap();
		//コントロール可能なオブジェクトを描画
		CONTROLLOBJMGR.Render_ShadowMap(*pViewOwner,*teamData);

		float offset_X = 1.0f / (float)shadow_Size;
		float offset_Y = 1.0f / (float)shadow_Size;
		shader2D->SetValue("offset_X", offset_X);
		shader2D->SetValue("offset_Y", offset_Y);
		//ブラーをかける
		//for (int k = 0; k < 1; k++){
		//	m_shdow_tex[i]->Render(0, 0, shadow_Size, shadow_Size, 0, 0, shadow_Size, shadow_Size, shader2D, "gaussX");
		//	m_shdow_tex[i]->Render(0, 0, shadow_Size, shadow_Size, 0, 0, shadow_Size, shadow_Size, shader2D, "gaussY");
		//}
	}
	iexSystem::Device->SetRenderTarget(0, NULL);
	iexSystem::Device->SetRenderTarget(1, NULL);
	//デプスバッファ復元
	iexSystem::GetDevice()->SetDepthStencilSurface(orgZ);
	
	shader->SetValue("Near_ShadowM", m_shdow_tex[0]);
	shader->SetValue("Far_ShadowM", m_shdow_tex[1]);
	shader->SetValue("Near_ShadowM_Stage", m_shadow_tex_MainStage[0]);
	shader->SetValue("Far_ShadowM_Stage", m_shadow_tex_MainStage[1]);


	shader->SetValue("n_ShadowProjection", shadow_Projection[0]);
	shader->SetValue("f_ShadowProjection", shadow_Projection[1]);
}

void ShadowMap::Render_Debug(int i)
{
	static bool flg = false;
	if (KEY_Get(KEY_SPACE,0) == 3)
	{
		flg ^= 0x01;
	}
	if (flg)
	{
		m_shdow_tex[0]->Render(515, 0, shadow_Size / 2, shadow_Size / 2, 0, 0, shadow_Size, shadow_Size);
		m_shdow_tex[1]->Render(0, 0, shadow_Size / 2, shadow_Size / 2, 0, 0, shadow_Size, shadow_Size);
	}
	else
	{
		m_shadow_tex_MainStage[0]->Render(515, 0, shadow_Size / 2, shadow_Size / 2, 0, 0, shadow_Size, shadow_Size);
		m_shadow_tex_MainStage[1]->Render(0, 0, shadow_Size / 2, shadow_Size / 2, 0, 0, shadow_Size, shadow_Size);
	}

}