#include	"iextreme.h"
#include"itDebugStringManager.h"




itDebugStringManager::itDebugStringManager()
{
	Clear();
}

itDebugStringManager::~itDebugStringManager()
{

}

itDebugStringManager& itDebugStringManager::GetInstance()
{
	static itDebugStringManager Instance;
	return Instance;
}

#ifdef _DEBUG

void itDebugStringManager::Clear()
{
	m_data.clear();
	m_data.push_back(DEBUGSTRING("DebugDrawText«", 0xFFFFFF00));
	m_data.push_back(DEBUGSTRING("\n", 0xFFFFFF00));
//	NextLine();
}


void itDebugStringManager::SetDebugStr(const char *str, DWORD color)
{
	//(*m_data.rbegin()) += DEBUGSTRING(str,color);
	m_data.push_back(DEBUGSTRING(str, color));
}

void itDebugStringManager::NextLine()
{
	//m_data.push_back(DEBUGSTRING("", 0xFFFFFF00));
}

void itDebugStringManager::Render()
{
	DEBUGSTRINGLIST::iterator it = m_data.begin(), end = m_data.end();

	const int str_size = 15, line_space = str_size + 10;
	int dx = 5, dy = 40;

	while (it != end)
	{
		IEX_DrawText((LPSTR)it->buffer.c_str(), dx, dy, str_size*(int)it->buffer.size(), str_size, it->color);

		dy += line_space;
		if (dy > 680)
		{
			dy = 100;
			dx += 200;
		}
		++it;
	}
}

#else

void itDebugStringManager::Clear()
{

}


void itDebugStringManager::SetDebugStr(const char *str, DWORD color)
{

}

void itDebugStringManager::NextLine()
{

}

void itDebugStringManager::Render()
{

}

#endif