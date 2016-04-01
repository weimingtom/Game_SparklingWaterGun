#include "All.h"


BulletManager::BulletManager():
GameBaseEntity(game_id::id_bullet_manager),
m_BulletSetCount(0)
{
	for (int i = 0; i < BULLETMAX; ++i)
	{
		m_pBulletArray[i] = nullptr;
	}
}

BulletManager::~BulletManager()
{
	for (int i = 0; i < BULLETMAX; ++i)
	{
		SAFE_DELETE(m_pBulletArray[i]);
	}
}

bool BulletManager::HandleMessage(LPGameMessage msg)
{
	if (msg->type == msg_t::add_bullet)
	{
		return AddBullet((LPBULLETBASE)msg->exinfo);
	}

	return false;
}

bool BulletManager::AddBullet(LPBULLETBASE bullet)		
{
	//弾をマネージャにセット(newしたポインタを渡すこと)
	for (int i = 0; i < BULLETMAX; ++i)
	{
		if (m_pBulletArray[m_BulletSetCount])
		{
			++m_BulletSetCount;
			m_BulletSetCount %= BULLETMAX;
			continue;
		}
		m_pBulletArray[m_BulletSetCount] = bullet;
		++m_BulletSetCount;
		m_BulletSetCount %= BULLETMAX;
		return true;
	}

	delete bullet;
	MyDebugString("BulletManager::AddBullet関数で弾の最大数に達しました\n");

	return false;
}


void BulletManager::Update()
{
	ColaWater::AddUpdateTag();

	for (int i = 0; i < BULLETMAX; i++)
	{
		if (m_pBulletArray[i])
		{
			m_pBulletArray[i]->Update();
			if (m_pBulletArray[i]->isDelete())
			{
				delete m_pBulletArray[i];
				m_pBulletArray[i] = nullptr;
			}
		}
	}
}


void BulletManager::Render()
{
	for (int i = 0; i < BULLETMAX; i++)
	{
		if (m_pBulletArray[i])
		{
			m_pBulletArray[i]->Render();
		}
	}
}