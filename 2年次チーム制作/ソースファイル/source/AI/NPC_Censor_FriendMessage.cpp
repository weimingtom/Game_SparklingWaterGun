#include	"..\FriendMessage.h"
#include	"..\All.h"

Censor_FriendMessage::Censor_FriendMessage(const int reaction_Speed) : 
NPC_Censor(-1, reaction_Speed), m_isActive(false),
m_process_MessageType(FriendMessageIcon::MsgType::_ERROR),
m_send_Entity(game_id::id_error)
{

}

Censor_FriendMessage::~Censor_FriendMessage()
{

}

void Censor_FriendMessage::Update(NPC_Brain* brain, Character* character)
{

}

bool	Censor_FriendMessage::ProcessMessage(NPC_Brain* brain, GameMessage* msg)
{
	//現在メッセージ処理を行ったゴールが適応されている場合は処理しない
	if (m_isActive)
		return true;
	FriendIconMsgInfo* message_Info =static_cast<FriendIconMsgInfo*>(msg->exinfo);

	//（現在HELPとCOME_ONのみ）
	if (message_Info->type == FriendMessageIcon::MsgType::_GO)
		return false;
	if (message_Info->type == FriendMessageIcon::MsgType::_OK)
		return false;

	//メッセージの送信者の記憶があるかどうかをチェックする
	MEMORY* memory = &brain->GetMemory();
	ENTITY_ID source_ID = message_Info->sender;
	memory->Is_Record_Character(source_ID);

	//Character情報を取得
	Character* target_ptr = dynamic_cast<Character*>(IDMANAGER.GetPointer(source_ID));
#if NPC_ASSART
	MyAssert(target_ptr,"character情報がないよ");
#endif
	if (!target_ptr)
		return false;
	
	float reliability;
	Character_Record* target_record = memory->Get_CharacterMemory(source_ID,
		Accesor_Brain::Param(brain).record_character_ReliabilityTime, &reliability);
	if (!target_record)
		return false;
	//情報を更新する
	Vector3 target_pos = ControllObjFunc::GetPos(target_ptr);
	if (!target_record->m_visibilily)
	{
		target_record->m_became_Pos = target_pos;
	}
	target_record->m_sensed_Pos = target_pos;
	target_record->m_hp = ChrFunc::GetParam(target_ptr).m_LifePoint;
	target_record->m_status = arive_status::arive;
	target_record->m_team_Type = ControllObjFunc::GetMatchData(target_ptr)->team_type;
	target_record->m_last_SensedTime = GAMEMATCHINFO.GetMatchTime();

	//自分自身のキャラクター情報取得
	Character* character = dynamic_cast<Character*>(IDMANAGER.GetPointer(msg->to));
#if NPC_ASSART
	MyAssert(character, "character情報がないよ");
#endif
	if (!character)
		return false;

	ai_message->Send_GoalThinkMessage(brain, character, ai_messagetype::set_messsage_bias, &message_Info->type);
	//GoalThinkに評価値補正Messageを送信する
	ai_message->Plannning(brain, character, &std::string("Censor_FriendMessage::ProcessMessage"), m_reaction_Speed);
	m_send_Entity = source_ID;
	m_process_MessageType = message_Info->type;
	m_isActive = true;
	return true;
}

bool	Censor_FriendMessage::Check_ThinkingResult(NPC_Brain* brain, FinalGoal_Evalutor* final_goal)
{
	if (!m_isActive)
		return false;
	
	ENTITY_ID target_ID = final_goal->Get_TargetID();
	if (target_ID != m_send_Entity)
		return false;

	goal_tag tag = final_goal->Get_EvaluteGoal()->Get_Tag();
	bool ret = false;
	switch (m_process_MessageType)
	{
	case FriendMessageIcon::MsgType::_HELP:
		if (tag == goal_tag::medical_friend &&
			m_process_MessageType == FriendMessageIcon::MsgType::_HELP)
		{
			ret = true;
			Accesor_Brain::FriendMessage(brain)->Avtive(FriendMessageIcon::MsgType::_OK);
		}
		break;

	case FriendMessageIcon::MsgType::_COME_ON:
		if (tag == goal_tag::follow_friend &&
			m_process_MessageType == FriendMessageIcon::MsgType::_COME_ON)
		{
			ret = true;
			Accesor_Brain::FriendMessage(brain)->Avtive(FriendMessageIcon::MsgType::_OK);
		}
		break;
	default:
		break;
	}
	if (ret)
	{
		m_isActive = false;
		m_send_Entity = game_id::id_error;
		m_process_MessageType = FriendMessageIcon::MsgType::_ERROR;
		return false;
	}
	return true;
	
}
