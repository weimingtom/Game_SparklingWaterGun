#ifndef __CODBO_FLAG_MATCH_H__
#define __CODBO_FLAG_MATCH_H__

#include"CODBO.h"
#include "CODBOForwardDecl.h"

//********************************************************
//	�t���b�O�Q�[���p�}�b�`�X�e�[�g
//********************************************************
class FlagMatchState :public MatchTypeStateBase
{
private:
	enum MATCHSTATE
	{

	};
private:
	const CODMatchData					m_MatchData;		//�v���C���[
	AimTargetManager*					m_pAimTargetManager;//�G�C���ڕW�}�l�[�W��
	ScreenSeparation*					m_pScreenSeparation;//��ʕ����Ǘ��N���X
	std::list<RECTI>					m_ScreenPosList;	//����������ʂ̔z�u�ꏊ
	Camera*								m_pCamera;			//�Ƃ肠�����J����
	ControllerManager*					m_pControllerMgr;	//�R���g���[���}�l�[�W���[
	BulletManager*						m_pBulletManager;	//�e�}�l�[�W��
	DamageObjectManager*				m_pDamageObjectManager;	//�_���[�W�}�l�[�W��
public:
	FlagMatchState(const CODMatchData& match_data);
	~FlagMatchState();
private:
	bool CreateCharacters();
public:
	void GameRender(LPIEXVIEW pView, ControllObject*pOwner);
public:
	void GameStart();
	bool isGameFinish();
public:
	void Initialize();
	void Update();
	void Render();
};
#endif