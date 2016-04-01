#ifndef __NEWS_TELOP_WORD_H__
#define __NEWS_TELOP_WORD_H__

#include "NewsTelop.h"

class NewsTelopWord:public NewsTelopWordBase
{
public:
	enum WORD_TYPE
	{
		BURNING_COLA,
		LEMON_LIFE,
		MELON_DUSH,
		GENKI_MIZU,

		NOKORI_JIKANN,

		TI_MUGA,
		SYEA_RITU,
		URIAGE,
		TOPPA,
		ICHII,

		YEN,
		HUN,
		KANMA,
		BIKKURI,
		PARCENT,

		_ERROR_TYPE,
		_FINISH
	};
private:
	WordParam		m_Data;
public:
	NewsTelopWord(const WordParam& data);
	~NewsTelopWord();
public:
	static WORD_TYPE GetTeamWord(int team);
	static void GetTextureRect(
		WORD_TYPE		type,
		WordParam&		outData);

	int GetReadFrame()const;
	int GetWidth()const;

	bool GetWord(
		LPCONTROLLOBJCT		pViewOwner,
		LPTEAMDATA			pTeamData,
		WordParam&			outData
		);
};

class NewsTelopWordNumber :public NewsTelopWordBase
{
private:
	COLORf		m_Color;
	int			m_Number;
public:
	NewsTelopWordNumber(int number,COLORf color);
private:
	bool GetWord(
		LPCONTROLLOBJCT		pViewOwner,
		LPTEAMDATA			pTeamData,
		WordParam&			outData
		);
public:
	int GetReadFrame()const;
	int GetWidth()const;

	static void AddNumbers(
		NewsTelopStr*	add_telop,
		int				number,
		COLORf			color
		);
};


#endif