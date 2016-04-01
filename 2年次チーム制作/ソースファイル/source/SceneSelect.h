#ifndef __SCENE_SELECT_H__
#define __SCENE_SELECT_H__

#include "system\Scene.h"
#include "iextreme.h"

class SceneSelect :public Scene
{
private:
	enum TEXTURE_TYPE
	{
		_TITLE,
		_TITLE_BACK,
		_PLAYER_NUM,
		_BUTTON,

		__TEXMAX
	};
	iexView			m_View;
	LPIEX2DOBJ		m_pTextures[__TEXMAX];
	int				m_Cursor;
	int				m_MaxPlayer;
	int				m_Count;
	float			m_Pre_stick;
public:
	SceneSelect();
	~SceneSelect();
public:
	
public:
	bool Initialize();
	void Update();
	void Render();
};

#endif