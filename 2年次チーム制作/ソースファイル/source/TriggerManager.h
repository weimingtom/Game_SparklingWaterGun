#ifndef __TRIGGER_MANAGER_H__
#define __TRIGGER_MANAGER_H__

#define TRIGGER_MAX 512


//*******************************************************
//	トリガーオブジェクト
//*******************************************************
class TriggerBase
{
public:
	inline TriggerBase(){}
	virtual ~TriggerBase(){}
public:
	virtual bool Update() = 0;		//毎フレームの更新（戻り値がtrueの場合実行を終了）
};

//*******************************************************
//	トリガーマネージャ
//*******************************************************
class TriggerManager
{
private:
	int				m_Trigger_count;
	TriggerBase*	m_pTrigger_array[TRIGGER_MAX];
public:
	TriggerManager();
	~TriggerManager();
public:
	void AddTrigger(TriggerBase* trigger);
	void Update();
};

#endif