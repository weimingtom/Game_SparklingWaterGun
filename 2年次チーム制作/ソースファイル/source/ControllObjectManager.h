#ifndef __CONTROLL_OBJECT_MANAGER_H__
#define __CONTROLL_OBJECT_MANAGER_H__

#include "ControllObject.h"
#include "CODBO.h"
#include "CODBOForwardDecl.h"
#include "TeamViewData.h"

//********************************************************
//	�Q�[�����̑���ł�����̂̊Ǘ��N���X
//********************************************************

class ControllObjectManager
{
private:
	static ControllObjectManager* m_pInstance;
	bool			m_isDelete[CODBO_CONTROLL_OBJECT_MAX];
	LPCONTROLLOBJCT m_pControllObject_array[CODBO_CONTROLL_OBJECT_MAX];	//�Ǘ�����L�����N�^�̔z��
public:
	ControllObjectManager();
	~ControllObjectManager();
public:
	static void						InstanceCreate();
	static ControllObjectManager&	InstanceGet();
	static void						InstanceRelase();
public:
	template<class _predicate_type>
	inline CONTROLLOBJLIST	GetObjectList(_predicate_type& pred);			//�v���f�B�P�[�g��true��Ԃ��L�����N�^�̃��X�g��Ԃ�
	template<class _function>
	inline void				For_each(_function& func);						//�S�L�����ɑ΂��֐������s����
	LPCONTROLLOBJCT			GetObjectPtr(ENTITY_ID id);		 				//�w�肳�ꂽID�̃L�����N�^�ւ̃|�C���^�𓾂�
	void					AddObject(LPCONTROLLOBJCT addobj);				//�Ǘ�����L�����N�^���X�V
	bool					DeleteObject(ENTITY_ID id);						//�w�肵��ID�̃I�u�W�F�N�g��delete��,����index���ė��p�\�ɂ���
	void					Update(TeamDataManager& teamData);										//�S���̍X�V
	void					Render(ControllObject& viewowner, TeamData& teamdata);										//�S���̕`��
	void					Render_ShadowMap(ControllObject& viewowner, TeamData& teamdata);
};

//	�l�����y�ɂ��邽�߂Ɂc
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
	//�S�L�����ɑ΂��֐������s����
	for (int i = 0; i < CODBO_CONTROLL_OBJECT_MAX; i++)
	{
		if (m_pControllObject_array[i])
			func(m_pControllObject_array[i]);
	}
}

#endif