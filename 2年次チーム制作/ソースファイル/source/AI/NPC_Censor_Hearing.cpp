//#include	"../All.h"
//
//Censor_Hearing::Censor_Hearing(const int& update_Interval, const int& reaction_Speed):
//NPC_Censor(update_Interval, reaction_Speed), m_most_ImportantSound(), m_activate(false),
//m_dispatch_Timer(-1)
//{
//	m_transact_vector.reserve(100);
//	
//}
//
//Censor_Hearing::~Censor_Hearing()
//{
//	m_transact_vector.clear();
//}
//
//void	Censor_Hearing::Hearing(NPC_Brain* brain, Character* character)
//{
//	//プランニング待機中は音を取得しない
//	if (m_activate)
//		return;
//
//	//音を取得する
//	Vector3 my_Pos = ControllObjFunc::GetPos(character);
//	env_Sound->Hearing(character, &m_transact_vector, my_Pos, Accesor_Brain::Param(brain).max_HearingRange);
//	//新規の音がない場合
//	if (m_transact_vector.empty())
//		return;
//
//	if (Transact(brain, character,my_Pos))
//	{
//		m_activate = true;
//		m_dispatch_Timer = m_reaction_Speed;
//		brain->Log(FormatString("%dフレームでCensor_Hearingがプランニング待機状態になりました\r\n name : type %s ",
//			GAMEMATCHINFO.GetMatchTime(), brain->character_Name,Convert_SoundType(m_most_ImportantSound.m_type)));
//	}
//	//更新後はcontainerを空に
//	m_transact_vector.clear();
//}
//
//float Censor_Hearing::Get_Range(const SOUND_TYPE& type,const NPCParam& param)
//{
//	float ret = .0f;
//
//	switch (type)
//	{
//	case SOUND_TYPE::foot :
//		ret = param.reaction_FootSound;
//		break;
//	case SOUND_TYPE::gun : 
//		ret = param.reaction_GunSound;
//		break;
//	default :
//#if NPC_ASSART
//		MyAssert(false, "Censor_Hearing::Get_Rangeに送られてきたTypeがおかしい", enum_cast(type));
//#endif
//		break;
//	}
//	return ret;
//}
//
//float Censor_Hearing::Get_Rate(const SOUND_TYPE& type, NPC_Brain* brain)
//{
//	static float gun_rate = 0.3f;
//	static float foot_rate = .5f;
//	switch (type)
//	{
//	case SOUND_TYPE::foot:
//		return foot_rate;
//			break;
//	case SOUND_TYPE::gun:
//		return gun_rate;
//		break;
//	default:
//#if NPC_ASSART
//		MyAssert(false, "Censor_Hearing::Get_Rateに送られてきたTypeがおかしい", enum_cast(type));
//#endif
//		break;
//	}
//	return .0f;
//}
//
//bool	Censor_Hearing::Transact(NPC_Brain* brain, Character* character, const Vector3& my_Pos)
//{
//	float max_Score = .0f;
//	bool ret = false;
//	Vector3 my_Front = ChrFunc::GetFront(character);
//	for (auto it = m_transact_vector.begin(); it != m_transact_vector.end(); it++)
//	{
//		float hearing_Range = Get_Range((*it)->type, Accesor_Brain::Param(brain));
//		//距離判定
//		Vector3 vec = ((*it)->source_Pos - my_Pos);
//		float distsq = vec.LengthSq();
//		if (hearing_Range < distsq)
//			continue;
//		//画面外（視界）判定
//		vec.Normalize();
//		float cos = Vector3Dot(my_Front, vec);
//		if (cos >Accesor_Brain::Param(brain).fov)
//			continue;
//
//		//スコア評価
//		ret = true;
//		float rate = Get_Rate((*it)->type, brain);
//		float score = distsq * rate;
//
//		if (max_Score < score)
//		{
//			m_most_ImportantSound.Set((*it)->type, (*it)->source_Pos, (*it)->param->power);
//			max_Score = score;
//		}
//	}
//	return ret;
//}
//
//void	Censor_Hearing::Update(NPC_Brain* brain, Character* character)
//{
//	int current_Frame = GAMEMATCHINFO.GetMatchTime();
//	if (current_Frame % m_update_Interval == 0)
//	{
//		Hearing(brain, character);
//	}
//
//	if (m_activate)
//	{
//		m_dispatch_Timer--;
//		if (m_dispatch_Timer <= 0)
//		{
//			ai_message->Plannning(brain, character, &std::string("Censor_Hearing"));
//		}
//	}
//}
//
//char* Censor_Hearing::Convert_SoundType(const SOUND_TYPE& type)
//{
//	static char* convert[] =
//	{
//		"foot",
//		"gun",
//	};
//	if (type == SOUND_TYPE::error)
//#if NPC_ASSART
//		MyAssert(false, "Convert_SoundTypeに想定外のメッセージtype%d", enum_cast(type));
//#endif
//	return convert[enum_cast(type)];
//}