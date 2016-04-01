#include "SceneEnd.h"
#include "sceneMain.h"
#include "system\System.h"
#include "CODBO.h"
#include "system\Framework.h"
#include "SceneSelect.h"

SceneEnd::SceneEnd()
{

}

SceneEnd::~SceneEnd()
{
	delete m_pScrTex;
}


bool SceneEnd::Initialize()
{
	m_MaxPlayer = 4;
	m_Cursor = 0;
	m_Pre_stick = 0;

	m_pScrTex = new iex2DObj("DATA\\TEXTURE\\End.png");

	return true;
}

void SceneEnd::Update()
{
	for (int i = 0; i < 4; ++i)
	{
		if (KEY(KEY_A, i))
		{
			MainFrame->ChangeScene(new SceneSelect());
		}
	}
//	MainFrame->ChangeScene(new SceneSelect());
}

void SceneEnd::Render()
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

	m_pScrTex->Render();
}