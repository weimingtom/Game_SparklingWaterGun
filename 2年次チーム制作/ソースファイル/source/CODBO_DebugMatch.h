#ifndef __COD_BO_DEBUG_MATCH_STATE__
#define __COD_BO_DEBUG_MATCH_STATE__

#include "CODBO.h"
#include "CODBOForwardDecl.h"

//********************************************************
//	�f�o�b�O(�J��)�p�}�b�`�X�e�[�g
//********************************************************
class DebugMatchState:public MatchTypeStateBase
{
private:
	AimTargetManager*					m_pAimTargetManager;//�G�C���ڕW�}�l�[�W��
	ScreenSeparation*					m_pScreenSeparation;//��ʕ����Ǘ��N���X
	std::list<RECTI>					m_ScreenPosList;	//����������ʂ̔z�u�ꏊ
	Camera*								m_pCamera;			//�Ƃ肠�����J����
	ControllerManager*					m_pControllerMgr;	//�R���g���[���}�l�[�W���[
	BulletManager*						m_pBulletManager;	//�e�}�l�[�W��
	DamageObjectManager*				m_pDamageObjectManager;	//�_���[�W�}�l�[�W��
	LPWATERPOOLMANAGER					m_pColaWaterMgr;	//�����܂�}�l�[�W��

public:
	DebugMatchState();
	~DebugMatchState();
public:
	void GameRender(LPIEXVIEW pView, ControllObject*pOwner);
public:
	void GameStart();
	bool isGameFinish();
public:
	void Initialize();
	void Update();
	void Render();

	//test
	void	Water_Render(LPIEXVIEW pView, ControllObject	*pOwner);
};


#endif