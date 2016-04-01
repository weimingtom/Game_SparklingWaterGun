#ifndef __COD_BO_H__
#define __COD_BO_H__

#include "ScreenSeparation.h"
#include "CODBOForwardDecl.h"
#include "GameSystem\GameController.h"
#include <list>

//********************************************************
//	ColaOfDissension_BloodyVsOcean�@
//		���X�V����N���X(SceneMain�݂����Ȃ���)�̃w�b�_�[
//********************************************************


//********************************************************
//	�Q�[����̒萔����
//********************************************************

#define CODBO_GAME_PLAYER_MAX 10		//�Q�[�����v���C�����(COM�܂߂�)�̍ő吔
#define CODBO_CONTROLL_OBJECT_MAX (35)	//�Q�[�����̑���ł�����̂̍ő吔
#define CODBO_MAP_OBJECT_MAX 100

//********************************************************
//	�Q�[���^�C�v
//********************************************************
namespace game_match_type
{
	enum _ENUM_GAME_MATCH_TYPE
	{
		FLAG_GAME,	//���Ƃ荇��
		
	};
}

//���̃Q�[���^�C�v��typedef�i�ł���΁��ł͂Ȃ���������g���Ăق����j
typedef game_match_type::_ENUM_GAME_MATCH_TYPE		GAME_MATCH_TYPE;

//********************************************************
//	�Q�[���p�L�����N�^�[�p�����[�^�[
//********************************************************

namespace player_type
{
	enum _ENUM_PLAYER_TYPE
	{
		PLAYER,		//�v���C���[(�R���g���[������)
		COMPUTER,	//�R���s���[�^
		SPECTATOR,	//�ϐ��
	};
}

typedef player_type::_ENUM_PLAYER_TYPE PLAYER_TYPE;


typedef struct PLAYERMATCHDATA
{
	PLAYER_TYPE				player_type;	//�v���C���[�^�C�v
	int						player_num;		//�v���C���[�ԍ�
	TEAM_TYPE				team_type;		//�������Ă���`�[���ԍ�
	ControllerButtonSet		button_set;		//�{�^���z�u���

} *LPPLAYERMATCHDATA;

typedef std::list<PLAYERMATCHDATA> PLAYER_MATCHDATA_LIST;

void InitPlayerMatchData(LPPLAYERMATCHDATA pOut);

//********************************************************
//	�}�b�`�^�C�v�ʃQ�[���X�e�[�g
//********************************************************
class MatchTypeStateBase
{
public:
	virtual ~MatchTypeStateBase(){}
public:
	virtual void GameStart() = 0;			//�Q�[���J�n���ɌĂ΂��֐�
	virtual bool isGameFinish() = 0;		//�Q�[�����I����Ă��邩(�I����Ă����烊�U���g�Ɉڍs)
	virtual void GetResultScene(Scene**ppOut) = 0;
public:
	virtual void Initialize() = 0;			//������
	virtual void Update() = 0;				//���t���[���̍X�V
	virtual void GameRender(LPIEXVIEW, ControllObject*) = 0;	//�����̃J�����ŃQ�[���I�u�W�F�N�g��`��
	virtual void Render() = 0;				//�`��֐�
};

typedef MatchTypeStateBase*		LPMatchTypeStateBase;


//********************************************************
//�Q�[�����n�߂�̂ɕK�v�ȃf�[�^
//********************************************************
struct CODMatchData
{
	GAME_MATCH_TYPE			match_type;		//�}�b�`�^�C�v
	LPMatchTypeStateBase	pMatch_state;	//�}�b�`�^�C�v�X�e�[�g
	PLAYER_MATCHDATA_LIST	player_data;	//�v���C�����
	int						player_num;		//�v���C���[��
	int						computer_num;	//�R���s���[�^�[��
	int						all_player_num;	//�L�����N�^����(�v���C���[���ƃR���s���[�^�[���̍��v)
};

//********************************************************
//	ColaOfDissension_BloodyVsOcean�@���X�V����N���X
//********************************************************
class ColaOfDessensionManager
{
private:
	CODMatchData						m_MatchData;		//�}�b�`�f�[�^
	LPMatchTypeStateBase				m_pState;			//�}�b�`�X�e�[�g(����pMatch_state�����o��������)
public:
	//�Q�[�����\�z����f�[�^�\���̂����ƂɃQ�[�����\�z
	ColaOfDessensionManager(LPCODMatchData pMatchData);
	//�}�b�`�X�e�[�g��delete���܂�
	~ColaOfDessensionManager();
public:
	static COLOR GetTeamColor(int team);
	static COLOR GetTeamColaWaterColor(int team);
public:
	bool isResult(Scene**ppOut);
	void Initialize();
	void Update();
	void Render();
};

typedef ColaOfDessensionManager CODMgr;

#endif