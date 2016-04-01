#include "All.h"

ScreenSeparation::ScreenSeparation(const int& num_P, ScreenCreater_Base* creater)
{
	num_Player = num_P;
	m_screen_texture_size.x = 0;
	m_screen_texture_size.y = 0;
	m_screen_texture_size = GAMEMATCHINFO.GetScreenSize();
	m_screen_create = creater;
	m_screen_create->Initialize(m_screen_texture_size);

	m_pScreenTextures = new iex2DObj*[num_Player];

	for (int i = 0; i < num_Player; i++)
	{
		m_pScreenTextures[i] = new iex2DObj(
			m_screen_texture_size.width,
			m_screen_texture_size.height,
			IEX2D_RENDERTARGET
			);
	}
}

ScreenSeparation::~ScreenSeparation()
{
	for (int i = 0; i < num_Player; i++)
	{
		delete m_pScreenTextures[i];
	}
	delete m_pScreenTextures;
	delete m_screen_create;
}

void ScreenSeparation::SetScreen(
	const  std::list< RECTI>&	screen_positions)
{
	MyAssert(screen_positions.size() <= RENDER_SCREEN_MAX, "描画できる画面の最大数は%dつまでです \n送られてきた数 %d ", (int)RENDER_SCREEN_MAX, (int)screen_positions.size());
	m_ScreenInfo = screen_positions;
}

//描画
void ScreenSeparation::CreateScreenTexture(
	CONTROLLOBJLIST&		obj_list,		//カメラリストここに入っている分だけ画面を表示する
	LPMatchTypeStateBase	pDrawMatchState		//描画する関数の入ったマッチステートクラスのポインター
	)
{
	auto obj_it = obj_list.begin();

	LPDEVICE pDev = iexSystem::GetDevice();
	Surface* pBackBuffer;

	MyAssert(SUCCEEDED(pDev->GetRenderTarget(0, &pBackBuffer)), "バックバッファを所得できませんでした");

	int render_count = 0;
	LPCONTROLLOBJCT pObj;

	while (
		obj_it != obj_list.end() &&
		render_count < RENDER_SCREEN_MAX)
	{
		pObj = *obj_it;
		m_screen_create->Create_Screen(m_pScreenTextures[render_count], &m_view, pObj);
		obj_it++;
		render_count++;
		pDev->SetRenderTarget(0, pBackBuffer);
		pDev->SetRenderTarget(1, NULL);
	}
}

//描画
void ScreenSeparation::DrawScreen()
{
	int screen_count = 0;
	LPRECTI s_size;

	iexSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);

	for (auto it = m_ScreenInfo.begin();
		it != m_ScreenInfo.end() && screen_count<RENDER_SCREEN_MAX;
		++it)
	{
		s_size = &*it;

		m_pScreenTextures[screen_count++]->Render(
			s_size->x,
			s_size->y,
			s_size->width,
			s_size->height,

			m_screen_texture_size.x,
			m_screen_texture_size.y,
			m_screen_texture_size.width,
			m_screen_texture_size.height
			);
	}

	//static bool a = false;
	//if (GetAsyncKeyState('G') & 0x01)
	//{
	//	a ^= 0x01;
	//}

	//if (a)
	//{
	//	ScreenCreater_DebugMatch* a = (ScreenCreater_DebugMatch*)m_screen_create;
	//	//a->Get_Texture()->Render();
	//	a->Render_ShadowMap();
	//}
		//m_screen_create->Get_Texture()->Render();
	
}


void CreateScreenRect(std::list<RECTI>* pOut, int nPlayer)
{

	pOut->clear();

	RECTI screen_info =
	{
		0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight
	};

	RECTI push_rect;
	int screen_num_width, screen_num_height;
	MyAssert(GAMEMATCHINFO.Get_Screen_Division(nPlayer, screen_num_width, screen_num_height),
		"CreateScreenRect関数に予期しないプレイヤー数が送られました\n 送られてきた人数 %d ", nPlayer);

	push_rect.width = screen_info.width / screen_num_width;
	push_rect.height = screen_info.height / screen_num_height;


	for (int i = 0; i < nPlayer; i++)
	{
		push_rect.x = push_rect.width*(i % screen_num_width);
		push_rect.y = push_rect.height*(i / screen_num_width);
		pOut->push_back(push_rect);
	}
}
//
//void ScreenSeparation::Set_ViewPort(int numPlayer)
//{
//	D3DVIEWPORT9 viewport;
//	const RECTI& rc = GAMEMATCHINFO.GetScreenSize();
//	viewport.X = 0;
//	viewport.Y = 0;
//	viewport.Width = rc.width;
//	viewport.Height = rc.height;
//	viewport.MaxZ = 1.0f;
//	viewport.MinZ = 0.0f;
//	iexSystem::GetDevice()->SetViewport(&viewport);
//}