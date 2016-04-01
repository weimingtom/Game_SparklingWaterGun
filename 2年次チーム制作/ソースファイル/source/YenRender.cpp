#include "YenRender.h"

YenRender::YenRender(
	LPIEX2DOBJ	pTexture,
	bool TextureDelete) :
m_pTexture(pTexture),
m_TargetValue(0),
m_NowValue(0),
m_ChangeSpeed(1),
m_TextureDelete(TextureDelete)
{

}

YenRender::~YenRender()
{
	if (m_TextureDelete)
	{
		delete m_pTexture;
	}
}

void YenRender::SetValue(int value)
{
	m_TargetValue = (value);
}

void YenRender::SetChangeSpeed(int speed)
{
	m_ChangeSpeed = (speed);
}

void YenRender::Update()
{
	if (m_NowValue > m_TargetValue + m_ChangeSpeed)
	{
		m_NowValue -= m_ChangeSpeed;
	}
	else if (m_NowValue < m_TargetValue - m_ChangeSpeed)
	{
		m_NowValue += m_ChangeSpeed;
	}
	else{
		m_NowValue = m_TargetValue;
	}
}

void YenRender::Render(
	int x,
	int y,
	int number_sizeX,
	int number_sizeY,
	int number_space)
{
	int dot_count = 0;
	int number;
	int value = m_NowValue;

	do {

		number = value % 10;

		m_pTexture->Render(
			x - number_sizeX,
			y,
			number_sizeX,
			number_sizeY,
			64 * number,
			0,
			64,
			64
			);

		value /= 10, x -= number_space;

	} while (value > 0);

}