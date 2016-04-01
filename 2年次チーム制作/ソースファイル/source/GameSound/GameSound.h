#ifndef __GAME_SOUND_H__
#define __GAME_SOUND_H__

#include "iextreme.h"
#include "../GameSystem/GameBase.h"
#include "../SingletonTemplate.h"
#include <map>

class GameSoundManager
{
public:
	typedef int SoundId;
	typedef int PlayerNum;
private:
	enum SoundPlayType
	{
		_SE,
		_BGM,
	};
	struct SoundData
	{
		SoundPlayType		type;
		iexSoundNumber		use_number;
		int					play_count;
		int					regist_num;
		float				minus_start;
		float				minus_end;
	};
	typedef std::map<SoundId, SoundData> SoundDataMap;
	typedef std::map<PlayerNum, Matrix>	CameraMatMap;
private:
	static GameSoundManager*	m_pInstance;
	iexSoundNumber				m_RegistedIdCount;
	SoundDataMap				m_SoundDataMap;
	CameraMatMap				m_CameraMatMap;
private:
	GameSoundManager();
	~GameSoundManager();
private:
	int GetVolume(RATIO v);
	float GetNearCamera(CRVector3 soundPos);
public:
	static GameSoundManager& GetInstance();
	static void ReleaseInstance();
public:

	RATIO GetSoundVolume(
		CRVector3		soundPos,
		SoundId			id);

	//�J�����X�V
	void UpdateCameraMatrix(PlayerNum num, CRMatrix mat);

	//�o�^
	bool RegistSE(
		SoundId			id,
		char*			file,
		float			minus_start,
		float			minus_end,
		int				multi_regist);

	bool RegistBGM(
		SoundId			id,
		char*			file);


	//�Đ�
	void PlaySE(SoundId id, RATIO volume);
	void PlayBGM(SoundId id);

	//��~
	void StopBGM(SoundId id);
};

#define SOUNDPLAYER (GameSoundManager::GetInstance())




//
//#include "../GameSystem/GameBase.h"
//#include <map>
//#include <vector>
//#include	<array>


//class GameSoundManager :public GameBaseEntity
//{
//public:
//	enum PlayerNumber	//�O�`�R
//	{
//		_error = -1,
//		_MaxPlayer = 4
//	};
//	typedef float	VOLUME;		//0.0f�`1.0f
//	typedef int		SoundId;	//���̎�ނ�����(�����Ƃ������Ƃ�)
//	//enum class SoundType
//	//{
//	//	SE,		//�v���C���[�����炷(�����A�����Ȃǂ��l������)
//	//	BGM,	//�v���C���[���Ɋ֌W�Ȃ����炷(�����A�����͍l�����Ȃ�)
//	//};
//	typedef std::map<PlayerNumber, const Matrix*> CameraMatMap;	//�`�悵�Ă���v���C���[�̃J�����s��
//
//private:
//	class SE_Buffer
//	{
//	public:
//		float						m_hearing_Dist;	//�������鋗��(Sq)
//		int						m_num_SE;			//�ő哯���Đ���(0~MaxPlayer)
//		std::vector<SoundId>	m_id_vector;		//IDvector
//		SE_Buffer(const float& m_hearing_Dist) : m_num_SE(0), m_hearing_Dist(m_hearing_Dist)
//		{}
//		bool	Regist_ID(const SoundId& regist_ID);
//	};
//	struct LoopSE
//	{
//		SoundId	play_ID;
//		se_list		se_type;
//		bool			active;
//		int			player_Num; //�v���C���[�̐��iCOM�܂ށj
//		LoopSE() :
//			play_ID(-1), active(false), player_Num(-1), se_type(se_list::error)
//		{}
//		void			Clear()
//		{
//			play_ID = -1;	se_type = se_list::error; active = false; player_Num = -1;
//		}
//	};
//
//private:
//	static const int													m_max_loopSound = 20;	//�ő僋�[�v�Đ���
//	CameraMatMap												m_CameraMatMap;
//	std::vector<SoundId>										m_already_useIndex;			//���Ɏg�p���ꂽ�ŗLID�R���e�i
//	std::map<se_list, SE_Buffer*>						m_se_Map;							//�e�J�������Ƃɓo�^���ꂽSE��ID���R���e�i						
//	std::array<LoopSE, m_max_loopSound>		m_current_LoopSound;		//���݃��[�v�Đ����Ă�o�b�t�@ID
//
//private:
//	/**
//	*@brief	�Đ�����SE�̉��ʂ����肷��
//	*@return	����
//	*/
//	float	Calculate_Dist(const Vector3& se_Pos, const Matrix& camera_Mat);
//	/**
//	*@brief �Đ�����SE�̉��ʂ����߂�
//	*@param ���̋���
//	*@param ���̕�������ő勗��
//	*@note IDirectSoundBuffer::SetVolume�̉��ʕ���-10000~0
//	*@note DSBVOLUME_MAX�ł��̂܂܂̉��ʂōĐ��ADSBVOLUME_MIN�Ŗ���
//	*/
//	int Calculate_SoundVolume(const float& dist, const float& max_hearing_Dist);
//
//	/**
//	*@brief  �o�^���悤�Ƃ��Ă���T�E���hID���g�p�ς݂��ǂ����`�F�b�N����
//	*@return �g�p�\�Ȃ�true
//	*/
//	bool Can_Use(const SoundId& regist_id);
//	/**
//	*@brief ���[�v�Đ�����SE��o�^
//	*/
//	bool	Register_LoopSE(const int& player_Num, const se_list& regist_se,
//		const SoundId& regist_ID);
//	/**
//	*@brief ���[�v�Đ����Ă���SE��m_current_LoopSound����폜
//	*@param �v���C���[�ԍ��iCOM�܂ށj
//	*@param SE�̎��
//	*@return ���[�v�Đ����Ă���SE�̌ŗLID
//	*/
//	SoundId Remove_LoopSE(const int& player_Num, const se_list& remove_se);
//
//public:
//	GameSoundManager();
//	~GameSoundManager();
//public:
//	/**
//	*@brief �`�悵�Ă���v���C���[�̃J�����s����Z�b�g
//	*@param �J�����������Ă���v���C���[No
//	*@param ViewMatrix
//	*/
//	void SetCameraMatrix(
//		PlayerNumber player_Num,
//		const Matrix& cameraMat
//		);
//
//	/**
//	*@brief SE��o�^����
//	*@param se�̎��
//	*@param �g�p�������ŗLID
//	*@param SE�̕�������ő勗��
//	*@param	�t�@�C���p�X
//	*/
//	bool Regist_SEIndex(
//		se_list					regist_type,
//		SoundId				sound,
//		float						hear_len,	//���̕�������ő勗��
//		char*					file
//		);
//	//bool	RegisetBGMIndex();
//
//	/**
//	*@brief SE�Đ�
//	*@param1 �Đ�����SE
//	*@param2 SE�̍��W
//	*@param3 �v���C���[�ԍ�
//	*@param4 ���[�vflg
//	*/
//	bool PlaySE(const se_list& se_type, CRVector3	pos,
//		const PlayerNumber&	player_number, bool	loop = false);
//
//	/**
//	*@brief	���[�v�Đ����Ă���SE���~����
//	*@param ��~����SE�̎��
//	*@param �v���C���[�ԍ��iCOM�܂ށj
//	*/
//	bool StopSE(
//		const se_list&	stop_se,
//		int	player_number
//		);
//
//	//�Đ�
//	bool PlayBGM(
//		bgm_list		sound,	//�Đ����鉹ID
//		bool			loop
//		);
//
//	//��~
//	bool StopBGM(
//		const bgm_list& sound_no
//		);
//	//���̃��������
//	void Release();
//	bool	HandleMessage(LPGameMessage msg);
//};


#endif


