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

	//カメラ更新
	void UpdateCameraMatrix(PlayerNum num, CRMatrix mat);

	//登録
	bool RegistSE(
		SoundId			id,
		char*			file,
		float			minus_start,
		float			minus_end,
		int				multi_regist);

	bool RegistBGM(
		SoundId			id,
		char*			file);


	//再生
	void PlaySE(SoundId id, RATIO volume);
	void PlayBGM(SoundId id);

	//停止
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
//	enum PlayerNumber	//０～３
//	{
//		_error = -1,
//		_MaxPlayer = 4
//	};
//	typedef float	VOLUME;		//0.0f～1.0f
//	typedef int		SoundId;	//音の種類を識別(足音とか爆発とか)
//	//enum class SoundType
//	//{
//	//	SE,		//プレイヤー数分鳴らす(距離、方向などを考慮して)
//	//	BGM,	//プレイヤー数に関係なく一回鳴らす(距離、方向は考慮しない)
//	//};
//	typedef std::map<PlayerNumber, const Matrix*> CameraMatMap;	//描画しているプレイヤーのカメラ行列
//
//private:
//	class SE_Buffer
//	{
//	public:
//		float						m_hearing_Dist;	//聞こえる距離(Sq)
//		int						m_num_SE;			//最大同時再生数(0~MaxPlayer)
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
//		int			player_Num; //プレイヤーの数（COM含む）
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
//	static const int													m_max_loopSound = 20;	//最大ループ再生数
//	CameraMatMap												m_CameraMatMap;
//	std::vector<SoundId>										m_already_useIndex;			//既に使用された固有IDコンテナ
//	std::map<se_list, SE_Buffer*>						m_se_Map;							//各カメラごとに登録されたSEのID情報コンテナ						
//	std::array<LoopSE, m_max_loopSound>		m_current_LoopSound;		//現在ループ再生してるバッファID
//
//private:
//	/**
//	*@brief	再生するSEの音量を決定する
//	*@return	距離
//	*/
//	float	Calculate_Dist(const Vector3& se_Pos, const Matrix& camera_Mat);
//	/**
//	*@brief 再生するSEの音量を決める
//	*@param 音の距離
//	*@param 音の聞こえる最大距離
//	*@note IDirectSoundBuffer::SetVolumeの音量幅は-10000~0
//	*@note DSBVOLUME_MAXでそのままの音量で再生、DSBVOLUME_MINで無音
//	*/
//	int Calculate_SoundVolume(const float& dist, const float& max_hearing_Dist);
//
//	/**
//	*@brief  登録しようとしているサウンドIDが使用済みかどうかチェックする
//	*@return 使用可能ならtrue
//	*/
//	bool Can_Use(const SoundId& regist_id);
//	/**
//	*@brief ループ再生するSEを登録
//	*/
//	bool	Register_LoopSE(const int& player_Num, const se_list& regist_se,
//		const SoundId& regist_ID);
//	/**
//	*@brief ループ再生しているSEをm_current_LoopSoundから削除
//	*@param プレイヤー番号（COM含む）
//	*@param SEの種類
//	*@return ループ再生しているSEの固有ID
//	*/
//	SoundId Remove_LoopSE(const int& player_Num, const se_list& remove_se);
//
//public:
//	GameSoundManager();
//	~GameSoundManager();
//public:
//	/**
//	*@brief 描画しているプレイヤーのカメラ行列をセット
//	*@param カメラを持っているプレイヤーNo
//	*@param ViewMatrix
//	*/
//	void SetCameraMatrix(
//		PlayerNumber player_Num,
//		const Matrix& cameraMat
//		);
//
//	/**
//	*@brief SEを登録する
//	*@param seの種類
//	*@param 使用したい固有ID
//	*@param SEの聞こえる最大距離
//	*@param	ファイルパス
//	*/
//	bool Regist_SEIndex(
//		se_list					regist_type,
//		SoundId				sound,
//		float						hear_len,	//音の聞こえる最大距離
//		char*					file
//		);
//	//bool	RegisetBGMIndex();
//
//	/**
//	*@brief SE再生
//	*@param1 再生するSE
//	*@param2 SEの座標
//	*@param3 プレイヤー番号
//	*@param4 ループflg
//	*/
//	bool PlaySE(const se_list& se_type, CRVector3	pos,
//		const PlayerNumber&	player_number, bool	loop = false);
//
//	/**
//	*@brief	ループ再生しているSEを停止する
//	*@param 停止するSEの種類
//	*@param プレイヤー番号（COM含む）
//	*/
//	bool StopSE(
//		const se_list&	stop_se,
//		int	player_number
//		);
//
//	//再生
//	bool PlayBGM(
//		bgm_list		sound,	//再生する音ID
//		bool			loop
//		);
//
//	//停止
//	bool StopBGM(
//		const bgm_list& sound_no
//		);
//	//音のメモリ解放
//	void Release();
//	bool	HandleMessage(LPGameMessage msg);
//};


#endif


