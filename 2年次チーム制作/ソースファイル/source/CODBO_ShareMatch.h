#ifndef __COD_BO_SHARE_MATCH_H__
#define __COD_BO_SHARE_MATCH_H__

#include "CODBO.h"
#include "CODBOForwardDecl.h"

//********************************************************
//	�f�o�b�O(�J��)�p�}�b�`�X�e�[�g
//********************************************************
class ShareMatchState :public MatchTypeStateBase
{
private:
	enum SHARE_MATCH_STATE
	{
		_PRE_PLAY,
		_PLAY,
		_FINISH
	};
private:
	AimTargetManager*					m_pAimTargetManager;//�G�C���ڕW�}�l�[�W��
	ScreenSeparation*					m_pScreenSeparation;//��ʕ����Ǘ��N���X
	std::list<RECTI>					m_ScreenPosList;	//����������ʂ̔z�u�ꏊ
	ControllerManager*					m_pControllerMgr;	//�R���g���[���}�l�[�W���[
	BulletManager*						m_pBulletManager;	//�e�}�l�[�W��
	DamageObjectManager*				m_pDamageObjectManager;	//�_���[�W�}�l�[�W��
	LPWATERPOOLMANAGER					m_pColaWaterMgr;	//�����܂�}�l�[�W��
	LPTEAMDATAMGR						m_pTeamDataMgr;		//�`�[�����Ƃ̃f�[�^
	LPSCORECALCMGR						m_pScoreCalcMgr;	//�X�R�A�J�E���g�N���X
	LPSCRICONMGR						m_pScrIconMgr;		//�X�N���[����̃A�C�R���}�l�[�W��
	LPNEWSTELOPMGR						m_pNewsTelopMgr;	//�j���[�X�e���b�v�}�l�[�W��
	NewsTelopUpdaterShareMatch*			m_pNewsUpdater;		//�j���[�X�X�V�N���X
	TimeRender*							m_pTimeRender;		//���ԃJ�E���g�N���X
	SHARE_MATCH_STATE					m_State;			//�}�b�`���
	int									m_TimeCount;		//���ԃJ�E���g
	bool								m_isGameFinish;		//�Q�[���I���t���O
public:
	ShareMatchState();
	~ShareMatchState();
private:
	void SetSoundCameraMatrix();
	void SetNextState(SHARE_MATCH_STATE	next);
	void MatchState_PrePlay();
	void MatchState_Play();
	void MatchState_Finish();
	void SetTeamShareRatio();
	bool CreateCharacter(ENTITY_ID id, PLAYER_TYPE type, int player_num, TEAM_TYPE team);
	bool CreateMovieModeObj(ENTITY_ID id, PLAYER_TYPE type, int player_num, TEAM_TYPE team);
public:
	void GameRender(LPIEXVIEW pView, ControllObject*pOwner);
public:
	void GameStart();
	bool isGameFinish();
	void GetResultScene(Scene**ppOut);
public:
	void Initialize();
	void Update();
	void Render();

	//test
	void	Water_Render(LPIEXVIEW pView, ControllObject	*pOwner);
};


#endif