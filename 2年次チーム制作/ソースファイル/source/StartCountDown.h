#ifndef __START_COUNT_DOWN_H__
#define __START_COUNT_DOWN_H__

#include "iextreme.h"

class StartCountDown
{
private:
	static const int count_texture_max = 4;
private:
	LPIEX2DOBJ	m_pTexture;
	float		m_count[count_texture_max];
	const float m_countSpeed;
public:
	StartCountDown(
		LPIEX2DOBJ texture
		);
	~StartCountDown();
public:
	void Reset();
	bool isStart()const;
	void Update();
	void Render();
};

#endif