#ifndef __TRIGGER_MANAGER_H__
#define __TRIGGER_MANAGER_H__

#define TRIGGER_MAX 512


//*******************************************************
//	�g���K�[�I�u�W�F�N�g
//*******************************************************
class TriggerBase
{
public:
	inline TriggerBase(){}
	virtual ~TriggerBase(){}
public:
	virtual bool Update() = 0;		//���t���[���̍X�V�i�߂�l��true�̏ꍇ���s���I���j
};

//*******************************************************
//	�g���K�[�}�l�[�W��
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