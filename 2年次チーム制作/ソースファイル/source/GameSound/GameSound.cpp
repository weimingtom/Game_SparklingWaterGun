#include "GameSound.h"

GameSoundManager*	GameSoundManager::m_pInstance = nullptr;

GameSoundManager::GameSoundManager():
m_RegistedIdCount(0)
{

}

GameSoundManager::~GameSoundManager()
{
	
}

int GameSoundManager::GetVolume(RATIO volume)
{
	return (int)((1 - volume) * -10000.0f);
}

float GameSoundManager::GetNearCamera(CRVector3 soundPos)
{
	Vector3 temp;
	float l = 1000000, l2;

	for (auto& it : m_CameraMatMap)
	{
		temp = Vector3MulMatrix(soundPos, it.second);

		l2 = temp.LengthSq();

		if (l > l2)l = l2;
	}

	return sqrtf(l);
}

GameSoundManager& GameSoundManager::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new GameSoundManager();
	return *m_pInstance;
}

void GameSoundManager::ReleaseInstance()
{
	delete m_pInstance;
	m_pInstance = nullptr;
}

RATIO GameSoundManager::GetSoundVolume(
	CRVector3		soundPos,
	SoundId			id
	)
{
	const SoundData& sd = m_SoundDataMap.at(id);
	float v = GetNearCamera(soundPos);

	v -= sd.minus_start;

	if (v<0)return 1;
	v /= sd.minus_end - sd.minus_start;

	return (v > 1) ? (0) : (1 - v);
}

void GameSoundManager::UpdateCameraMatrix(PlayerNum num, CRMatrix mat)
{
	m_CameraMatMap[num] = mat;
}

bool GameSoundManager::RegistSE(
	SoundId			id,
	char *			file, 
	float			minus_start,
	float			minus_end,
	int				multi_regist)
{

	MyAssert((m_SoundDataMap.count(id) <= 0), "‰¹‚Ì“ñd“o˜^‚ªs‚í‚ê‚Ü‚µ‚½");

	SoundData data;

	data.type = _SE;
	data.play_count = 0;
	data.regist_num = multi_regist;
	data.use_number = m_RegistedIdCount;
	data.minus_start = minus_start;
	data.minus_end = minus_end;

	for (int i = 0; i < data.regist_num; ++i)
	{
		IEX_SetWAV(m_RegistedIdCount++, file, false);
	}

	m_SoundDataMap.insert(SoundDataMap::value_type(
		id,
		data
		));

	return true;
}


bool GameSoundManager::RegistBGM(
	SoundId			id,
	char*			file)
{
	MyAssert((m_SoundDataMap.count(id) <= 0), "‰¹‚Ì“ñd“o˜^‚ªs‚í‚ê‚Ü‚µ‚½");
	MyAssert((file), "‰¹‚Ìƒtƒ@ƒCƒ‹–¼‚ªNULL‚Å‚·");

	SoundData data;

	data.type = _BGM;
	data.play_count = 0;
	data.regist_num = 1;
	data.use_number = m_RegistedIdCount;
	data.minus_start = 0;
	data.minus_end = 0;

	IEX_SetWAV(m_RegistedIdCount++, file);

	m_SoundDataMap.insert(SoundDataMap::value_type(
		id,
		data
		));

	return true;
}


void GameSoundManager::PlaySE(SoundId id, RATIO volume)
{
	if (volume <= 0.0f)return;

	MyAssert(m_SoundDataMap.count(id), "Ä¶‚Å‚«‚È‚¢‰¹Id id=%d", id);

	SoundData& sd = m_SoundDataMap.at(id);
	const int v = GetVolume(volume);

	IEX_SetSoundVolume(sd.use_number + sd.play_count, v);
	IEX_PlaySound(sd.use_number + sd.play_count, false);

	sd.play_count = (sd.play_count + 1) % sd.regist_num;
}

void GameSoundManager::PlayBGM(SoundId id)
{
	SoundData& sd = m_SoundDataMap.at(id);
	IEX_PlaySound(sd.use_number, true);
}

void GameSoundManager::StopBGM(SoundId id)
{
	SoundData& sd = m_SoundDataMap.at(id);

	if (IEX_GetSoundStatus(sd.use_number))
		IEX_StopSound(sd.use_number);
}

//#include "../CODPlaySound.h"
//#pragma region SE_Buffer
//
//bool	GameSoundManager::SE_Buffer::Regist_ID(const SoundId& regist_ID)
//{
//	m_num_SE++;
//	if (m_num_SE > _MaxPlayer)
//	{
//		MyAssert(false, "Å‘å“o˜^”‚ğ’´‚¦‚Ä‚¢‚Ü‚·");
//		m_num_SE--;
//		return false;
//	}
//	m_id_vector.push_back(regist_ID);
//	return true;
//}
//
//#pragma endregion
//
//#pragma region Manager
//
//GameSoundManager::GameSoundManager() :
//GameBaseEntity(game_id::id_sound_manager)
//{
//
//}
//
//GameSoundManager::~GameSoundManager()
//{
//	for (auto& it : m_se_Map)
//	{
//		SAFE_DELETE(it.second);
//	}
//}
//
//void	GameSoundManager::SetCameraMatrix(PlayerNumber player_Num, const Matrix& camera_Mat)
//{
//	if (m_CameraMatMap.find(player_Num) == m_CameraMatMap.end())
//	{
//		m_CameraMatMap.insert(std::pair<PlayerNumber, const Matrix*>(player_Num, &camera_Mat));
//	}
//	m_CameraMatMap[player_Num] = &camera_Mat;
//}
//
//bool	 GameSoundManager::Can_Use(const SoundId& regist_id)
//{
//	for (auto& it : m_already_useIndex)
//	{
//		if (it == regist_id)
//		{
//			MyAssert(false, "“o˜^‚µ‚æ‚¤‚Æ‚µ‚½SoundID‚ÍŠù‚Ég—p‚³‚ê‚Ä‚¢‚Ü‚· error_index %d", regist_id);
//			return false;
//		}
//	}
//	m_already_useIndex.push_back(regist_id);
//	return true;
//}
//
//bool	GameSoundManager::Regist_SEIndex(se_list regist_type, SoundId	sound,
//	float	hear_len, char* file)
//{
//	if (!Can_Use(sound))
//		return false;
//
//	if (m_se_Map.find(regist_type) == m_se_Map.end())
//	{
//		m_se_Map.insert(std::pair<se_list, SE_Buffer*>(regist_type, new SE_Buffer(hear_len)));
//	}
//	if (m_se_Map[regist_type]->Regist_ID(sound))
//	{
//		IEX_SetWAV(sound, file);
//		return true;
//	}
//	return false;
//}
//
//bool	GameSoundManager::Register_LoopSE(const int& player_Num, const se_list& regist_se,
//	const SoundId& regist_ID)
//{
//	int register_Index = -1;
//	for (int i = 0; i < m_max_loopSound; i++)
//	{
//		if (m_current_LoopSound[i].active)
//		{
//			if (m_current_LoopSound[i].play_ID == regist_ID)
//				return false;
//
//			continue;
//		}
//		register_Index = i;
//	}
//	m_current_LoopSound[register_Index].active = true;
//	m_current_LoopSound[register_Index].player_Num = player_Num;
//	m_current_LoopSound[register_Index].play_ID = regist_ID;
//	m_current_LoopSound[register_Index].se_type = regist_se;
//
//	return true;
//}
//
//GameSoundManager::SoundId GameSoundManager::Remove_LoopSE(const int& player_Num, const se_list& remove_se)
//{
//	SoundId ret = -1;
//	for (auto& it : m_current_LoopSound)
//	{
//		if (!it.active)
//			continue;
//		if (it.player_Num == player_Num &&
//			it.se_type == remove_se)
//		{
//			ret = it.play_ID;
//			it.Clear();
//			return ret;
//		}
//	}
//	return ret;
//}
//
//float GameSoundManager::Calculate_Dist(const Vector3& se_Pos, const Matrix& camera_Mat)
//{
//	Vector3 viewPos = Vector3MulMatrix(se_Pos, camera_Mat);
//	return viewPos.Length();
//}
//
//int GameSoundManager::Calculate_SoundVolume(const float& dist, const float& max_hearing_Dist)
//{
//	float rate = dist / max_hearing_Dist;
//	//”½“]‚µ‚Ä•â³‚©‚¯‚é
//	rate *= -1.0f;
//	rate *= 10000.0f;
//	return (int)rate;
//}
//
//bool	GameSoundManager::PlaySE(const se_list& se_type,
//	CRVector3 pos, const PlayerNumber& player_Num, bool loop)
//{
//	if (m_se_Map.find(se_type) == m_se_Map.end())
//		return false;
//	SE_Buffer*	play_Buffer = m_se_Map[se_type];
//	PlayerNumber	camera_num = PlayerNumber::_error;
//	float min_dist = play_Buffer->m_hearing_Dist
//		;
//	for (auto& it : m_CameraMatMap)
//	{
//		const Matrix& mat = *it.second;
//		float dist = Calculate_Dist(pos, mat);
//
//		if (dist <= min_dist)
//		{
//			min_dist = dist;
//			camera_num = it.first;
//		}
//	}
//	if (camera_num == PlayerNumber::_error)
//		return false;
//
//	if (play_Buffer->m_num_SE >= camera_num)
//	{
//		if (loop)
//		{
//			Register_LoopSE(player_Num, se_type, play_Buffer->m_id_vector[camera_num]);
//		}
//		//‰¹—ÊŒˆ’è
//		int	volume = Calculate_SoundVolume(sqrtf(min_dist), play_Buffer->m_hearing_Dist);
//		IEX_SetSoundVolume(play_Buffer->m_id_vector[camera_num], volume);
//		IEX_PlaySound(play_Buffer->m_id_vector[camera_num], loop);
//
//		return true;
//	}
//	return false;
//}
//
//bool	GameSoundManager::StopBGM(const bgm_list& sound_no)
//{
//
//	return true;
//}
//
//void	GameSoundManager::Release()
//{
//	for (auto& it : m_already_useIndex)
//	{
//
//	}
//}
//
//bool	GameSoundManager::HandleMessage(LPGameMessage msg)
//{
//	cod_sound::PlaySEParam* pSE;
//
//	switch (msg->type)
//	{
//	case msg_t::play_se_msg:
//		pSE = (cod_sound::PlaySEParam*)msg->exinfo;
//		PlaySE(
//			pSE->se_type,
//			*pSE->pPos,
//			pSE->number,
//			pSE->loop
//			);
//		return true;
//	case msg_t::play_bgm_msg:
//	//	PlayBGM(*(bgm_list*)msg->exinfo, true);
//		return true;
//	default:
//		MyAssert(false, "GameSoundManager::HandleMessage@‘Î‰‚µ‚Ä‚¢‚È‚¢ƒƒbƒZ[ƒW Type=%d", msg->type);
//		break;
//	}
//	return false;
//}
//
//bool	GameSoundManager::StopSE(const se_list&	stop_se, int	player_number)
//{
//	SoundId stop_Index = Remove_LoopSE(player_number, stop_se);
//	if (stop_Index == -1)
//	{
//		//MyAssert(false, "~‚ß‚éSE‚ª‚È‚¢‚æ se_list : %d  player_number : %d", (int)stop_se, player_number);
//		return false;
//	}
//	IEX_StopSound(stop_Index);
//	return true;
//}
//
//#pragma endregion
