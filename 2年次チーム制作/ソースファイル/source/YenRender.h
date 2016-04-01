#ifndef __YEN_RENDER_H__
#define __YEN_RENDER_H__

#include "iextreme.h"

class YenRender
{
private:
	LPIEX2DOBJ	const	m_pTexture;
	bool				m_TextureDelete;
	int					m_TargetValue;
	int					m_NowValue;
	int					m_ChangeSpeed;
public:
	YenRender(LPIEX2DOBJ	pTexture, bool TextureDelete);
	~YenRender();
public:
	void SetValue(int value);
	void SetChangeSpeed(int speed);
	void Update();
	void Render(int x, int y, int number_sizeX, int number_sizeY,int number_space);
};

#endif