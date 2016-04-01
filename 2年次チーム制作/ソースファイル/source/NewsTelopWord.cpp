#include "NewsTelopWord.h"

NewsTelopWord::NewsTelopWord(const WordParam& data):
m_Data(data)
{

}

NewsTelopWord::~NewsTelopWord()
{

}

int NewsTelopWord::GetReadFrame()const
{
	return 30;
}

int NewsTelopWord::GetWidth()const
{
	return (int)m_Data.size.x;
}

NewsTelopWord::WORD_TYPE NewsTelopWord::GetTeamWord(int team)
{
	NewsTelopWord::WORD_TYPE team_word[4]=
	{
		GENKI_MIZU,
		LEMON_LIFE,
		MELON_DUSH,
		BURNING_COLA
	};

	MyAssert(team >= 0 && team < 4, "対応しないチーム番号　%d が指定されました", team);

	return team_word[team];
}

void NewsTelopWord::GetTextureRect(
	WORD_TYPE	type,
	WordParam&	outData)
{
	static const int widthDatas[]=
	{
		8,
		6,
		7,
		3,
		4,
		4,
		4,
		2,
		2,
		2
	};

	outData.color.SetColor(1, 1, 1, 1);
	outData.size.y = 64/2;

	if ((int)type < (int)YEN)
	{
		outData.size.x = (float)(widthDatas[type] * 64 / 2);
		outData.tex_rc.width = widthDatas[type] * 64;
		outData.tex_rc.left = (int)type / 5 * 512;
		outData.tex_rc.top = (int)type % 5 * 64;
		outData.tex_rc.height = 64;
	}
	else{
		outData.size.x = 64 / 2;
		outData.tex_rc.left = ((int)type - (int)YEN) * 64 + 640;
		outData.tex_rc.top = 320;
		outData.tex_rc.width = 64;
		outData.tex_rc.height = 64;
	}
	
}

bool NewsTelopWord::GetWord(
	LPCONTROLLOBJCT		pViewOwner,
	LPTEAMDATA			pTeamData,
	WordParam&			outData
	)
{
	outData = m_Data;

	return true;
}

NewsTelopWordNumber::NewsTelopWordNumber(int number, COLORf color):
m_Number(number),
m_Color(color)
{

}

bool NewsTelopWordNumber::GetWord(
	LPCONTROLLOBJCT		pViewOwner,
	LPTEAMDATA			pTeamData,
	WordParam&			outData
	)
{
	outData.color = m_Color;

	outData.size.x = (float)GetWidth();
	outData.size.y = 32;

	outData.tex_rc.left = 64 * m_Number;
	outData.tex_rc.top = 320;
	outData.tex_rc.width = 64;
	outData.tex_rc.height = 64;

	return true;
}

int NewsTelopWordNumber::GetReadFrame()const
{
	return 10;
}

int NewsTelopWordNumber::GetWidth()const
{
	return 32;
}

void NewsTelopWordNumber::AddNumbers(
	NewsTelopStr*	add_telop,
	int				value,
	COLORf			color
	)
{
	int number;
	int keta = 0;

	for (int v = value; v > 9; v /= 10)++keta;

	do
	{
		number = value;

		for (int i = 0; i < keta; ++i)
		{
			number /= 10;
		}
		number %= 10;

		add_telop->AddWord(
			new NewsTelopWordNumber(number, color)
			);

	} while (keta-- > 0);

}