#include "TeamViewData.h"
#include "ControllObjectFunction.h"
#include "GameSystem\GameIDManager.h"

TeamViewData::TeamViewData():
m_draw_seed(0),
m_apeer_speed(0.05f),
m_delete_speed(1),
m_draw_speed(0.1f),
m_draw_power(0)
{

}

TeamViewData::~TeamViewData()
{

}

void TeamViewData::SetData(CRDataParam data)
{
	VIEWDATAMAP::iterator it = m_data.find(data.id);

	if (it != m_data.end())
	{
		it->second = data;
		return;
	}

	m_data.insert(
		VIEWDATAMAP::value_type(
		data.id,
		data
		)
		);
}

bool TeamViewData::GetViewData(
	ENTITY_ID searchID,
	DataParam& outData,
	RATIO& draw_power)const
{
	VIEWDATAMAP::const_iterator it = m_data.find(searchID);

	if (it == m_data.end())return false;

	outData = it->second;
	draw_power = m_draw_power*0.5f + 0.3f;
	draw_power *= outData.alpha;

	return true;
}

void TeamViewData::Update()
{
	VIEWDATAMAP::iterator it = m_data.begin();
	DataParam* d;

	while (it != m_data.end())
	{
		d = &it->second;

		//時間で消滅
		if (d->time <= 0 ||
			IDMANAGER.GetPointer(d->id) == nullptr)
		{
			it = m_data.erase(it);
			continue;
		}

		d->time -= m_delete_speed;

		if (d->time < 100)
			d->alpha = d->time*0.01f;
		else
			d->alpha = min(d->alpha + m_apeer_speed, 1);
		//d->alpha = 1;

		++it;
	}
	
	m_draw_seed += m_draw_speed;
	if (m_draw_seed>100000)m_draw_seed = 0;

	m_draw_power = sinf(m_draw_seed)*0.5f + 0.5f;
}






//**********************************************************

TeamDataManager::TeamDataManager(int TeamMax) :
m_DataMax(TeamMax)
{
	assert(TeamMax > 0 && "TeamMaxが０以下の状態でViewDataManagerのコンストラクタに入りました");
	m_pDatas = new TeamData[TeamMax];
}

TeamDataManager::~TeamDataManager()
{
	delete[]m_pDatas;
}

TeamData& TeamDataManager::MyData(int team_type)
{
	assert(team_type >= 0 && team_type < m_DataMax&&"存在しないチーム番号が指定されました");
	return m_pDatas[team_type];
}

void TeamDataManager::Update()
{
	for (int i = 0; i < m_DataMax; ++i)
	{
		m_pDatas[i].m_Viewdata.Update();
	}
}