#ifndef __NEWS_TELOP_UPDATER_H__
#define __NEWS_TELOP_UPDATER_H__

#include "iextreme.h"
#include "map\MapObject.h"

class NewsTelopUpdaterShareMatch
{
private:
	MapObjectManager::SHAREPARAM_ARRAY		m_Share_array;
	DWORD	m_RemainingFrame;
public:
	NewsTelopUpdaterShareMatch();
	~NewsTelopUpdaterShareMatch();
private:
	bool isNewsValue(RATIO& out_value, RATIO value);
	void UpdateShareValueNews();
	void UpdateTimeNews(DWORD frame);
	void SendShareValueNews(RATIO	value, int team);
public:
	void SetRemainingFrame(DWORD	Frame);
	void Update();
};

#endif