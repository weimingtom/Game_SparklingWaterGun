#ifndef __environmental_sound_h__
#define __environmental_sound_h__

#include	"SoundMessage.h"
#include	"Field_Division.h"

class Env_SoundManager;

class SoundParam
{
public:
	int		sound_No;			//環境音FileNo
	int		end_Frame;		//消失フレーム
	float		power;				//音が聞こえる半径
	SoundParam(const int& no, const int& end, const float& po) :
		sound_No(no), end_Frame(end), power(po)
	{	}
};

class Env_Sound
{
	friend class Env_SoundManager;
private :
	std::vector<Character*>		hearing_Entity;	//既にこの音を聞いたentity
	Tree_Object<Env_Sound>*  division_Ptr;
public :
	SOUND_TYPE					type;
	SoundParam*					param;
	int									event_Frame;	//音の発生フレーム
	int									current_Frame;	//進行フレーム
	Vector3							source_Pos;		//発生源
	int									team_No;			//チームNo

public :
	Env_Sound(const SOUND_TYPE& type,const int& ev_Frame, const Vector3& pos,
							SoundParam* param,const int& team_No);
	~Env_Sound();
	bool	Update();
	bool	Hearing(Character* chara);
};

class Env_SoundManager
{
private :
	std::list<Env_Sound*>			sound_List;				//現在フィールドに発生している環境音リスト
	std::vector<SoundParam*>	param_vector;			//サウンドの種類に応じた情報（音の強さ、消失フレームなど）
	std::vector<Env_Sound*>		work_vector;				//sound_Mapから拾ってきた音を一時的に格納するコンテナ
	FieldDivision_Manager<Env_Sound>	sound_Map;
	int												min_Ret;						//サウンド取得時の基準値（sound_Mapから拾うかsound_Listから拾うか）
private :
	Env_SoundManager(){}
	~Env_SoundManager();
	bool	Load_SoundParam(char* filename);
	/**
	*@brief sound_Map全てとの聴覚判定を取る
	*/
	int		Hearing_SoundList(Character* character, std::vector<Env_Sound*>* ret_Container);
public :
	static Env_SoundManager* Get_Instance()
	{
		static Env_SoundManager I; return &I;
	}
	void	Initialize();
	/**
	*@brief			環境音をセット
	*@param1		音のタイプ
	*@param2		音が発生するするフレーム
	*@param3		発生源の座標
	*@param4		音を出したentityの所属チーム
	*/
	void	Register( const SOUND_TYPE& type,const int& ev_Frame, const Vector3& pos,const int& team_No);

	void	Update();
	/**
	*@brief		キャラクターが新たに聞こえる音を返却する
	*@param1	キャラクターのポインタ
	*@param2	聞こえた音の情報を格納するポインタ
	*@param3 検索開始座標
	*@param3	検索する半径
	*/
	int	Hearing(Character* character, std::vector<Env_Sound*>* ret_Container,
		const Vector3& search_Pos, const float& search_Radius);
};
#define env_Sound (Env_SoundManager::Get_Instance())

#endif
