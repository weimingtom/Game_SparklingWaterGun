#ifndef __CONTROLL_OBJECT_MANAGER_H__
#define __CONTROLL_OBJECT_MANAGER_H__

#include "ControllObject.h"
#include "CODBO.h"
#include "CODBOForwardDecl.h"
#include "TeamViewData.h"

//********************************************************
//	ゲーム中の操作できるものの管理クラス
//********************************************************

class ControllObjectManager
{
private:
	static ControllObjectManager* m_pInstance;
	bool			m_isDelete[CODBO_CONTROLL_OBJECT_MAX];
	LPCONTROLLOBJCT m_pControllObject_array[CODBO_CONTROLL_OBJECT_MAX];	//管理するキャラクタの配列
public:
	ControllObjectManager();
	~ControllObjectManager();
public:
	static void						InstanceCreate();
	static ControllObjectManager&	InstanceGet();
	static void						InstanceRelase();
public:
	template<class _predicate_type>
	inline CONTROLLOBJLIST	GetObjectList(_predicate_type& pred);			//プレディケートがtrueを返すキャラクタのリストを返す
	template<class _function>
	inline void				For_each(_function& func);						//全キャラに対し関数を実行する
	LPCONTROLLOBJCT			GetObjectPtr(ENTITY_ID id);		 				//指定されたIDのキャラクタへのポインタを得る
	void					AddObject(LPCONTROLLOBJCT addobj);				//管理するキャラクタを更新
	bool					DeleteObject(ENTITY_ID id);						//指定したIDのオブジェクトをdeleteし,そのindexを再利用可能にする
	void					Update(TeamDataManager& teamData);										//全部の更新
	void					Render(ControllObject& viewowner, TeamData& teamdata);										//全部の描画
	void					Render_ShadowMap(ControllObject& viewowner, TeamData& teamdata);
};

//	人生を楽にするために…
#define CONTROLLOBJMGR (ControllObjectManager::InstanceGet())




template <class _predicate_type>
inline CONTROLLOBJLIST ControllObjectManager::GetObjectList(_predicate_type& pred)
{
	CONTROLLOBJLIST ret;
	for (int i = 0; i < CODBO_CONTROLL_OBJECT_MAX; i++)
	{
		if (m_pControllObject_array[i]&&
			pred(m_pControllObject_array[i]))
			ret.push_back(m_pControllObject_array[i]);
	}
	return ret;
}

template<class _function>
inline void	ControllObjectManager::For_each(_function& func)
{
	//全キャラに対し関数を実行する
	for (int i = 0; i < CODBO_CONTROLL_OBJECT_MAX; i++)
	{
		if (m_pControllObject_array[i])
			func(m_pControllObject_array[i]);
	}
}

#endif