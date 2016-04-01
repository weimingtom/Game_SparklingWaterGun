#include "NewsTelop.h"
#include "GameMatchInfomation.h"
#include "GameSystem\GameIDManager.h"

NewsTelopStr::NewsTelopStr():
m_ReadFrame(0),
m_Length(0)
{

}

NewsTelopStr::~NewsTelopStr()
{
	for (WordList::iterator it = m_WordList.begin();
		it != m_WordList.end();
		++it)
	{
		delete *it;
	}
}

void NewsTelopStr::AddWord(NewsTelopWordBase* addWord)
{
	m_WordList.push_back(addWord);
	m_ReadFrame += addWord->GetReadFrame();
	m_Length += addWord->GetWidth();
}

bool NewsTelopStr::isDelete(float count)const
{
	return ((int)count > m_ReadFrame);
}

int NewsTelopStr::GetLength()const
{
	return m_Length;
}

void NewsTelopStr::Render(
	RenderParam&		render_param,
	LPCONTROLLOBJCT		pViewOwner,
	LPTEAMDATA			pTeamData
	)
{
	int w, h;

	WordParam wp;
	NewsTelopWordBase* pIt;
	float X_count = render_param.RenderStartpos.x;
	COLORf	color;

	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	for (WordList::iterator it = m_WordList.begin();
		it != m_WordList.end();
		++it)
	{
		pIt = *it;

		pIt->GetWord(pViewOwner, pTeamData, wp);
		wp.size.x /= (float)w;
		wp.size.y /= (float)h;
		wp.color.a = min(wp.color.a, render_param.ApperAlpha);

		render_param.pTelopWordTex->Render(
			(int)(X_count),
			(int)(render_param.RenderStartpos.y),
			(int)(wp.size.x),
			(int)(wp.size.y*wp.color.a),

			wp.tex_rc.left,
			wp.tex_rc.top,
			wp.tex_rc.width,
			(int)((float)wp.tex_rc.height*wp.color.a), 
			RS_COPY,
			wp.color.toDWORD()
			);

		X_count += wp.size.x;
	}
}

//************************************************************************

NewsTelopManager::NewsTelopManager() :
m_TelopWordTex("DATA\\TEXTURE\\UI\\News_moji.png"),
m_TelopBackTex("DATA\\TEXTURE\\UI\\moji_bar.png"),
m_BackTelopState(_NONE),
m_TelopCount(0),
GameBaseEntity(game_id::id_news_telop_manager)
{
	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w, h);

	m_TelopSpeed = 2;
	m_TelopRenderHeight = 500;

	m_TelopSpeed /= w;
	m_TelopRenderHeight /= h;

	m_BackTextureSizeY = 0;
	m_BackTextureTargetSizeY = 32; 

	m_StrAlpha = 0;
}

NewsTelopManager::~NewsTelopManager()
{
	Reset();
}

void NewsTelopManager::DeleteNowStr()
{
	if (m_TelopList.empty())return;

	delete *m_TelopList.begin();
	m_TelopList.pop_front();
}

void NewsTelopManager::BackTelopStateUpdate()
{
	const float TelopApperSpeed = 0.2f;
	const float TelopDeleteSpeed = 0.8f;
	const float StrApperSpeed = 0.01f;
	const float StrDeleteSpeed = 0.01f;

	switch (m_BackTelopState)
	{
	case _APPER:
		//出現中の場合はテロップ背景を目標の大きさにする
		m_BackTextureSizeY += (m_BackTextureTargetSizeY - m_BackTextureSizeY)*TelopApperSpeed;
		if (fabsf(m_BackTextureTargetSizeY - m_BackTextureSizeY) < 1)
		{
			m_BackTextureSizeY = m_BackTextureTargetSizeY;
			m_BackTelopState = _USUAL;
			m_TelopCount = 0;
		}
		m_StrAlpha = m_BackTextureSizeY / m_BackTextureTargetSizeY;
		break;
	case _USUAL:
		//文字を出現
		break;
	case _EXIT:
		//消失中の場合はテロップ背景を目標の大きさにする
		m_BackTextureSizeY *= TelopDeleteSpeed;
		m_StrAlpha = m_BackTextureSizeY / m_BackTextureTargetSizeY;
		
		if (m_BackTextureSizeY < 1)
		{
			m_BackTextureSizeY = 0;
			m_BackTelopState = _NONE;
			DeleteNowStr();
		}
		break;
	case _NONE:
		//消えているときは何もしない
		break;
	}

	
}

void NewsTelopManager::Reset()
{
	for (TelopList::iterator it = m_TelopList.begin();
		it != m_TelopList.end();
		++it)
	{
		delete *it;
	}
}

void NewsTelopManager::AddTelop(NewsTelopStr* telop_ptr)
{
	m_TelopList.push_back(telop_ptr);
}

void NewsTelopManager::Update()
{
	BackTelopStateUpdate();

	//テロップデータがない場合はなにもしない
	if (m_TelopList.empty())
	{
		//通常表示状態の場合は消す
		if (m_BackTelopState == _USUAL)
		{
			m_BackTelopState = _EXIT;
		}
		return;
	}

	m_TelopCount++;

	//データがある状態なら出現させる
	if (m_BackTelopState == _NONE)
	{
		m_BackTelopState = _APPER;
		m_TelopCount = 0;
	}

	//もし表示中のテロップの消滅時間が来たら消去する
	if (m_BackTelopState == _USUAL&&
		(*m_TelopList.begin())->isDelete(m_TelopCount))
	{
		m_BackTelopState = _EXIT;
	}
}

void NewsTelopManager::Render(
	LPCONTROLLOBJCT		pViewOwner,
	LPTEAMDATA			pTeamData)
{

	if (m_TelopList.empty() || m_StrAlpha <= 0)return;

	int w, h;
	GAMEMATCHINFO.GetNowGameScrDivision(w, h);
	const int str_len = (*m_TelopList.begin())->GetLength() / w;
	NewsTelopStr::RenderParam rp;
	COLORf back_color;

	back_color.a = min(m_StrAlpha, 0.6f);

	rp.ApperAlpha = m_StrAlpha;
	rp.pTelopWordTex = &m_TelopWordTex;

	rp.RenderStartpos.x = (float)(640 / w);
	rp.RenderStartpos.y = (float)(360 / h);

	rp.RenderStartpos.x -= (float)(str_len / 2);

	if (m_BackTextureSizeY >= 1)
	{
		m_TelopBackTex.Render(
			(int)rp.RenderStartpos.x,
			(int)rp.RenderStartpos.y,
			str_len,
			(int)m_BackTextureSizeY/h,
			
			0,
			0,
			1024,
			(int)(64.0f * (m_BackTextureSizeY / m_BackTextureTargetSizeY)),
			RS_COPY,
			back_color.toDWORD()
			);
	}

	NewsTelopStr* pBegin = *m_TelopList.begin();

	pBegin->Render(
		rp,
		pViewOwner,
		pTeamData
		);
}


bool NewsTelopManager::HandleMessage(LPGameMessage msg)
{
	if (msg->type == msg_t::send_news_telop)
	{
		AddTelop((NewsTelopStr*)msg->exinfo);
		return true;
	}
	else if (msg->type == msg_t::news_telop_render_msg)
	{
		RenderOwnerMsgExinfo* pInfo = (RenderOwnerMsgExinfo*)msg->exinfo;
		Render(pInfo->pViewOwner, pInfo->pTeamData);
		return true;
	}

	return false;
}