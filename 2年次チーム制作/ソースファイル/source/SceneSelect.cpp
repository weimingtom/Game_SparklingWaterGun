#include "SceneSelect.h"
#include "sceneMain.h"
#include "system\System.h"
#include "CODBO.h"
#include "system\Framework.h"


SceneSelect::SceneSelect()
{
	for (int i = 0; i < (int)__TEXMAX; ++i)
	{
		m_pTextures[i] = nullptr;
	}
}

SceneSelect::~SceneSelect()
{
	for (int i = 0; i < (int)__TEXMAX; ++i)
	{
		delete	m_pTextures[i];
		m_pTextures[i] = nullptr;
	}
}


bool SceneSelect::Initialize()
{
	m_MaxPlayer = 4;
	m_Cursor = 0;
	m_Pre_stick = 0;
	m_Count = 0;

	m_pTextures[_TITLE] = new iex2DObj("DATA\\TEXTURE\\Title\\title.png");
	m_pTextures[_TITLE_BACK] = new iex2DObj("DATA\\TEXTURE\\Title\\title_back.png");
	m_pTextures[_PLAYER_NUM] = new iex2DObj("DATA\\TEXTURE\\Title\\Player_512x512_white.png");
	m_pTextures[_BUTTON] = new iex2DObj("DATA\\TEXTURE\\Title\\ã‰º‚Æ.png");

	return true;
}

void SceneSelect::Update()
{
	const float stick = KEY(KEY_AXISY, 0)*0.001f;

	if (++m_Count == INT_MAX)m_Count = 0;

	if (m_Pre_stick<0.3f&&stick>0.3f)
	{
		--m_Cursor;
		m_Cursor = max(m_Cursor, 0);
	}
	else  if (m_Pre_stick > -0.3f&&stick < -0.3f)
	{
		++m_Cursor;
		m_Cursor = min(m_Cursor, m_MaxPlayer - 1);
	}

	m_Pre_stick = stick;

	if (KEY(KEY_A, 0) == 3)
	{
		CODMatchData match;
		match.player_num = m_Cursor + 1;

		MainFrame->ChangeScene(new sceneMain(&match));
	}
}

void SceneSelect::Render()
{
	m_View.Set(Vector3(0, 0, -1), Vector3(0, 0, 0));
	m_View.Activate();
	m_View.Clear(0xFFFFFF00);

	//	ŠÂ‹«Ý’è
	iexLight::SetAmbient(0x404040);
	iexLight::SetFog(800, 1000, 0);

	Vector3 dir(1.0f, -1.0f, -0.5f);
	dir.Normalize();
	iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);

	COLORf color;

	m_pTextures[_TITLE_BACK]->Render();
	
	m_pTextures[_TITLE]->Render(
		100,
		50,
		1020,
		270,

		0,
		0,
		2048,
		1024
		);

	m_pTextures[_PLAYER_NUM]->Render(
		620,
		420,
		480,
		140,

		0,
		m_Cursor * 64, 
		256,
		64,
		0,
		0xFFFFFF00
		);

	const int cursor_size = 60;

	color.SetColor(0xFFFFFFFF);
	color.a = sinf((float)m_Count*0.1f)*0.3f + 0.7f;

	
	if (m_Cursor != m_MaxPlayer - 1)
	{
		m_pTextures[_BUTTON]->Render(
			780,
			350,
			cursor_size,
			cursor_size,

			34,
			36,
			64,
			64,
			RS_COPY,
			color.toDWORD()
			);
	}

	if (m_Cursor != 0)
	{
		m_pTextures[_BUTTON]->Render(
			780,
			565,
			cursor_size,
			cursor_size,

			34,
			167,
			64,
			64,
			RS_COPY,
			color.toDWORD()
			);
	}

	m_pTextures[_BUTTON]->Render(
		1122,
		460,
		80,
		80,

		149,
		20,
		90,
		90,
		RS_COPY,
		color.toDWORD()
		);
}