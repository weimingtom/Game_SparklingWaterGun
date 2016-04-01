#include	"..\All.h"


Env_Sound::Env_Sound(const SOUND_TYPE& type,
											const int& ev_Frame, const Vector3& pos,SoundParam* param,const int& team_No)
{
	this->param = param;
	this->event_Frame = ev_Frame;
	source_Pos = pos; 
	this->type = type;
	current_Frame = 0;
	hearing_Entity.clear();
	division_Ptr = new Tree_Object<Env_Sound>(this);
	this->team_No = team_No;
}

Env_Sound::~Env_Sound()
{
	division_Ptr->Remove_Cell();
	SAFE_DELETE(division_Ptr);
	
}

bool	Env_Sound::Update()
{
	if (current_Frame == event_Frame)
	{
		IEX_PlaySound(param->sound_No, false);
	}
	current_Frame++;

	if (current_Frame >= param->end_Frame)
		return true;
	
	return false;
}

bool	Env_Sound::Hearing(Character* chara)
{
	//既に聞いたことのあるおとの場合は飛ばす
	auto  entitry = std::find(hearing_Entity.begin(), hearing_Entity.end(), chara);
	if (entitry != hearing_Entity.end())
		return false;

	Vector3 pos = ControllObjFunc::GetPos(chara);
	float dist = (pos - source_Pos).LengthSq();
	
	if (dist < param->power)
	{
		hearing_Entity.push_back(chara);
		return true;
	}
	return false;
}

//****************************************************//
//Manager
//****************************************************//

void	Env_SoundManager::Initialize()
{
	//とりあえず音を適当に登録
	sound_List.clear();
	param_vector.reserve(100);
	sound_Map.Initialize(Vector3(.0f, .0f, .0f), 300, 3);
	Load_SoundParam("DATA\\SOUNDDATA\\EnvSoundParam.txt");
	work_vector.reserve(100);
	min_Ret = 30;
}

Env_SoundManager::~Env_SoundManager()
{
	for (auto it = param_vector.begin(); it != param_vector.end();it++)
	{
		SAFE_DELETE(*it);
	}
}

void	Env_SoundManager::Update()
{
	for (auto it = sound_List.begin(); it != sound_List.end();)
	{
		if ((*it)->Update())
		{
			SAFE_DELETE(*it);
			it = sound_List.erase(it);
			continue;
		}
		it++;
	}
	/*MyDebugMgrStr(dword_color::red, "現在発生している環境音");
	MyDebugMgrStr(dword_color::white, "%d", sound_List.size());

	MyDebugMgrStr(dword_color::red, "sound_Mapに登録している数");
	MyDebugMgrStr(dword_color::white, "%d", sound_Map.AllObjectSize());
*/
}

int		Env_SoundManager::Hearing_SoundList(Character* character, std::vector<Env_Sound*>* ret_Container)
{
	int ret = 0;
	for (auto it = sound_List.begin(); it != sound_List.end(); it++)
	{
		if ((*it)->Hearing(character))
		{
			ret_Container->push_back((*it));
			ret++;
		}
	}
	return ret;
}

int		Env_SoundManager::Hearing(Character* character, std::vector<Env_Sound*>* ret_Container,
																const Vector3& search_Pos, const float& search_Radius)
{
	//現在のフィールドに発生している音が一定値以下の場合全ての音との判定をとる
	if (sound_List.size() <= (unsigned int)min_Ret)
		return Hearing_SoundList(character, ret_Container);

	int ret = 0;
	work_vector.clear();
	sound_Map.Search_Object_Near(search_Pos.x, search_Pos.z, &work_vector, search_Radius);
	for (auto it = work_vector.begin(); it != work_vector.end(); it++)
	{
		if ((*it)->Hearing(character))
		{
			ret_Container->push_back(*it);
			ret++;
		}
	}
	return ret;
}

void	Env_SoundManager::Register(const SOUND_TYPE& type, const int& ev_Frame, const Vector3& pos,const int& team_No)
{
	Env_Sound* ptr = new Env_Sound(type, ev_Frame, pos, param_vector[(int)type],team_No);
	ptr->division_Ptr->Registar_Field(ptr->source_Pos.x, ptr->source_Pos.z, &sound_Map);
	sound_List.push_back(ptr);
	
}

bool	Env_SoundManager::Load_SoundParam(char* filename)
{
	static char* sound_name[2] =
	{
		"FOOTSOUND",
		"GUNSOUND"
	};
	textLoader loader(filename);
	MyAssert(loader.Be_Init(), "環境音読み込み失敗 filename: %s", filename);
	int i = 0;
	loader.In();
	char buffer[64];
	for (;;)
	{
		loader._Loadstring(buffer);
		if (strcmp("}", buffer) == 0)
			break;
		
		if (strcmp(sound_name[i], buffer) != 0)
		{
			MyAssert(false, "サウンドの並びがおかしい");
		}
		int		no = loader.LoadInt();
		int		end_F = loader.LoadInt();
		float	power = loader.LoadFloat();
		param_vector.push_back(new SoundParam(no, end_F, power));
		i++;
	}
	int a = 0;
	return true;
}