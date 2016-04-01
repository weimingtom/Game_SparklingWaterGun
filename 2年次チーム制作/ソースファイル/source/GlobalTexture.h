#ifndef __GLOBAL_TEXTURE_H__
#define __GLOBAL_TEXTURE_H__

#include "iextreme.h"

class GlobalTexture
{
private:
	iex2DObj m_texture;
	iex2DObj m_texture2;
private:
	GlobalTexture() :
		m_texture(iexSystem::ScreenWidth, iexSystem::ScreenHeight,IEX2D_RENDERTARGET),
		m_texture2(iexSystem::ScreenWidth, iexSystem::ScreenHeight, IEX2D_RENDERTARGET)
	{
	}
public:
	static GlobalTexture& GetInstance()
	{
		static GlobalTexture instance;
		return instance;
	}
	iex2DObj& GetScreenSizeTexture()
	{
		return m_texture;
	}
	iex2DObj& GetScreenSizeTexture2()
	{
		return m_texture2;
	}
};

#define GLOBALTEXTURE (GlobalTexture::GetInstance())

#endif