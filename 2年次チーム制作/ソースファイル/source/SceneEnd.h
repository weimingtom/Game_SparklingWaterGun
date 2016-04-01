#ifndef __SCENE_END_H__
#define __SCENE_END_H__

#include "system\Scene.h"
#include "iextreme.h"

class SceneEnd :public Scene
{
private:
	iexView			m_View;
	LPIEX2DOBJ		m_pScrTex;
	int				m_Cursor;
	int				m_MaxPlayer;
	float			m_Pre_stick;
public:
	SceneEnd();
	~SceneEnd();
public:

public:
	bool Initialize();
	void Update();
	void Render();
};

#endif