#ifndef __environmental_sound_h__
#define __environmental_sound_h__

#include	"SoundMessage.h"
#include	"Field_Division.h"

class Env_SoundManager;

class SoundParam
{
public:
	int		sound_No;			//����FileNo
	int		end_Frame;		//�����t���[��
	float		power;				//�����������锼�a
	SoundParam(const int& no, const int& end, const float& po) :
		sound_No(no), end_Frame(end), power(po)
	{	}
};

class Env_Sound
{
	friend class Env_SoundManager;
private :
	std::vector<Character*>		hearing_Entity;	//���ɂ��̉��𕷂���entity
	Tree_Object<Env_Sound>*  division_Ptr;
public :
	SOUND_TYPE					type;
	SoundParam*					param;
	int									event_Frame;	//���̔����t���[��
	int									current_Frame;	//�i�s�t���[��
	Vector3							source_Pos;		//������
	int									team_No;			//�`�[��No

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
	std::list<Env_Sound*>			sound_List;				//���݃t�B�[���h�ɔ������Ă���������X�g
	std::vector<SoundParam*>	param_vector;			//�T�E���h�̎�ނɉ��������i���̋����A�����t���[���Ȃǁj
	std::vector<Env_Sound*>		work_vector;				//sound_Map����E���Ă��������ꎞ�I�Ɋi�[����R���e�i
	FieldDivision_Manager<Env_Sound>	sound_Map;
	int												min_Ret;						//�T�E���h�擾���̊�l�isound_Map����E����sound_List����E�����j
private :
	Env_SoundManager(){}
	~Env_SoundManager();
	bool	Load_SoundParam(char* filename);
	/**
	*@brief sound_Map�S�ĂƂ̒��o��������
	*/
	int		Hearing_SoundList(Character* character, std::vector<Env_Sound*>* ret_Container);
public :
	static Env_SoundManager* Get_Instance()
	{
		static Env_SoundManager I; return &I;
	}
	void	Initialize();
	/**
	*@brief			�������Z�b�g
	*@param1		���̃^�C�v
	*@param2		�����������邷��t���[��
	*@param3		�������̍��W
	*@param4		�����o����entity�̏����`�[��
	*/
	void	Register( const SOUND_TYPE& type,const int& ev_Frame, const Vector3& pos,const int& team_No);

	void	Update();
	/**
	*@brief		�L�����N�^�[���V���ɕ������鉹��ԋp����
	*@param1	�L�����N�^�[�̃|�C���^
	*@param2	�����������̏����i�[����|�C���^
	*@param3 �����J�n���W
	*@param3	�������锼�a
	*/
	int	Hearing(Character* character, std::vector<Env_Sound*>* ret_Container,
		const Vector3& search_Pos, const float& search_Radius);
};
#define env_Sound (Env_SoundManager::Get_Instance())

#endif
