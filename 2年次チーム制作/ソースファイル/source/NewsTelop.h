#ifndef __NEWS_TELOP_H__
#define __NEWS_TELOP_H__

#include "CODBOForwardDecl.h"
#include "iextreme.h"
#include "GameSystem\GameBase.h"
#include <list>

struct WordParam
{
	TextureRect		tex_rc;
	COLORf			color;
	Vector2			size;
};

class NewsTelopWordBase
{
public:
	virtual ~NewsTelopWordBase(){}

	virtual int GetWidth()const = 0;
	virtual int GetReadFrame()const = 0;
	virtual bool GetWord(
		LPCONTROLLOBJCT		pViewOwner,
		LPTEAMDATA			pTeamData,
		WordParam&			outData
		) = 0;
};

class NewsTelopStr
{
private:
	typedef std::list<NewsTelopWordBase*> WordList;
public:
	struct RenderParam
	{
		LPIEX2DOBJ			pTelopWordTex;
		Vector2				RenderStartpos;
		RATIO				ApperAlpha;
	};
private:
	WordList		m_WordList;
	int				m_ReadFrame;
	int				m_Length;
public:
	NewsTelopStr();
	~NewsTelopStr();
public:
	void AddWord(NewsTelopWordBase* addWord);
	bool isDelete(float count)const;
	int GetLength()const;
	void Render(
		RenderParam&		render_param,
		LPCONTROLLOBJCT		pViewOwner,
		LPTEAMDATA			pTeamData
		);
};

class NewsTelopManager :public GameBaseEntity
{
private:
	typedef std::list<NewsTelopStr*> TelopList;
	enum TelopBackState
	{
		_APPER,			//èoåªíÜ
		_USUAL,			//ï\é¶ÇµÇƒÇ¢ÇÈ
		_EXIT,			//è¡Ç¶ÇƒÇ¢ÇÈìríÜ
		_NONE,			//äÆëSÇ…è¡Ç¶ÇƒÇ¢ÇÈ
	};
private:
	TelopList			m_TelopList;
	float				m_TelopSpeed;
	float				m_TelopCount;
	float				m_TelopRenderHeight;
	TelopBackState		m_BackTelopState;
	float				m_BackTextureSizeY;
	float				m_BackTextureTargetSizeY;
	IEX2DOBJ			m_TelopWordTex;
	IEX2DOBJ			m_TelopBackTex;
	RATIO				m_StrAlpha;
public:
	NewsTelopManager();
	~NewsTelopManager();
private:
	void DeleteNowStr();
	void BackTelopStateUpdate();
public:
	void Reset();
	void AddTelop(NewsTelopStr* telop_ptr);
	void Update();
	void Render(
		LPCONTROLLOBJCT		pViewOwner,
		LPTEAMDATA			pTeamData);

	bool HandleMessage(LPGameMessage msg);
};

#endif