#include "NewsTelopUpdater.h"
#include "NewsTelopWord.h"
#include "GameSystem\GameIDManager.h"
#include <algorithm>

NewsTelopUpdaterShareMatch::NewsTelopUpdaterShareMatch():
m_RemainingFrame(MAXDWORD)
{

}

NewsTelopUpdaterShareMatch::~NewsTelopUpdaterShareMatch()
{

}

bool NewsTelopUpdaterShareMatch::isNewsValue(RATIO& out_value, RATIO value)
{
	const float news_values[2] = { 0.6f, 0.8f };

	for (int i = 0; i < ARRAYSIZE(news_values); ++i)
	{
		if (news_values[i] < value)
		{
			out_value = news_values[i];
			return true;
		}
	}
	return false;
}

void NewsTelopUpdaterShareMatch::UpdateShareValueNews()
{
	MapObjectManager::SHAREPARAM_ARRAY share_array;
	MAP_MANAGER.Get_ShareValue(share_array);

	bool isfind;
	float news_value;

	for (auto &it : share_array)
	{
		if (isNewsValue(news_value, it.share_power))
		{
			isfind = false;
			for (auto &it2 : m_Share_array)
			{
				if (it.team_type == it2.team_type)
				{
					isfind = true;
					if (news_value > it2.share_power)
					{
						SendShareValueNews(it.share_power, it.team_type);
					}
					break;
				}
			}
			if (isfind == false)
			{
				SendShareValueNews(it.share_power, it.team_type);
			}
		}
	}

	m_Share_array = share_array;
}

void NewsTelopUpdaterShareMatch::UpdateTimeNews(DWORD frame)
{
	if (frame != DWORD(3600))return;

	NewsTelopStr* telop = new NewsTelopStr();
	WordParam wp;

	NewsTelopWord::WORD_TYPE word[] =
	{
		NewsTelopWord::NOKORI_JIKANN,
		NewsTelopWord::_ERROR_TYPE,
		NewsTelopWord::HUN,
		NewsTelopWord::BIKKURI
	};

	for (auto &it : word)
	{
		if (it != NewsTelopWord::_ERROR_TYPE)
		{
			NewsTelopWord::GetTextureRect(
				it,
				wp
				);

			telop->AddWord(
				new NewsTelopWord(wp)
				);
		}
		else{
			NewsTelopWordNumber::AddNumbers(
				telop,
				1,
				COLORf(1, 1, 1, 1)
				);
		}
	}

	GameMessage		msg;

	msg.from = game_id::id_error;
	msg.to = game_id::id_news_telop_manager;
	msg.type = msg_t::send_news_telop;
	msg.exinfo = telop;

	IDMANAGER.Dispatch(
		&msg
		);
}

void NewsTelopUpdaterShareMatch::SendShareValueNews(
	RATIO		value,
	TEAM_TYPE team
	)
{
	NewsTelopStr* telop = new NewsTelopStr();
	WordParam wp;

	NewsTelopWord::WORD_TYPE word[]=
	{
		NewsTelopWord::GetTeamWord(team),
		NewsTelopWord::TI_MUGA,
		NewsTelopWord::SYEA_RITU,

		NewsTelopWord::_ERROR_TYPE,
		NewsTelopWord::PARCENT,

		NewsTelopWord::TOPPA,
		NewsTelopWord::BIKKURI
	};

	for (auto &it : word)
	{
		if (it != NewsTelopWord::_ERROR_TYPE)
		{
			NewsTelopWord::GetTextureRect(
				it,
				wp
				);

			telop->AddWord(
				new NewsTelopWord(wp)
				);
		}
		else{
			NewsTelopWordNumber::AddNumbers(
				telop,
				static_cast<int>(value*100.0f),
				COLORf(1, 1, 1, 1)
				);
		}
	}

	GameMessage		msg;

	msg.from = game_id::id_error;
	msg.to = game_id::id_news_telop_manager;
	msg.type = msg_t::send_news_telop;
	msg.exinfo = telop;

	IDMANAGER.Dispatch(
		&msg
		);
}

void NewsTelopUpdaterShareMatch::SetRemainingFrame(DWORD	Frame)
{
	m_RemainingFrame = Frame;
}

void NewsTelopUpdaterShareMatch::Update()
{
	//シェア率関連のニュース更新
	UpdateShareValueNews();

	//時間関連のニュース更新
	UpdateTimeNews(m_RemainingFrame);
}