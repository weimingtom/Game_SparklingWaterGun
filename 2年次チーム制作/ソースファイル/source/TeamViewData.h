#ifndef __TEAM_VIEW_DATA_H__
#define __TEAM_VIEW_DATA_H__

#include "GameSystem\GameIDDefine.h"
#include "iextreme.h"
#include <map>

class TeamViewData
{
public:
	struct DataParam
	{
		ENTITY_ID	id;
		DWORD		color;
		float		time;
		RATIO		alpha;
	};
	typedef const DataParam& CRDataParam;
	typedef std::map<ENTITY_ID, DataParam> VIEWDATAMAP;
private:
	VIEWDATAMAP		m_data;
	float			m_draw_seed;
	RATIO			m_draw_power;
	const float		m_draw_speed;
	const float		m_apeer_speed;
	const float		m_delete_speed;
public:
	TeamViewData();
	~TeamViewData();
public:
	void SetData(CRDataParam data);
	bool GetViewData(ENTITY_ID searchID, DataParam& outData, RATIO& draw_power)const;
	void Update();
};

struct TeamData
{
	TeamViewData m_Viewdata;

};

class TeamDataManager
{
private:
	const int		m_DataMax;
	TeamData*		m_pDatas;
public:
	TeamDataManager(int TeamMax);
	~TeamDataManager();
public:
	TeamData& MyData(int team_type);
	void Update();
};

#endif