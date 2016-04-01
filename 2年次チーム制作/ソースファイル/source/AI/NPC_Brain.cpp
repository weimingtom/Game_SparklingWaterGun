#include	"..\All.h"
#include	<algorithm>
#define NPC_LOG 0
#define CONSOLE_BRAIN 1

NPC_Brain::NPC_Brain(const char* name, const int Controllder_Num,const int team_type) : m_console(nullptr)
{
	static int	top_X = 1000;
	static int	top_Y = 200;

	//基本パラメータ読み込み
	m_memory.Initialize(this);
	m_symbol.Initialize();
	Set_FinalGoal();
	m_behavior_Manager = new Behavior_Manager(this);
	m_evalutor = new NPC_Evalutor(this);
	m_path_Planner = new PathPlanner(this);
	m_particularGoal_Manager = new ParticularGoal_Manager(this);
	this->character_Name = name;
	m_fuzzy.Initialize();
	m_message_Icon = new NPCFriendMessage(this);
	char work2[128];
	sprintf(work2, "DATA\\NPC\\log\\%d %s.txt", Controllder_Num, character_Name);
	top_X += 400;
#if NPC_LOG
	m_console = new DebugLogOutput(work2);
	m_console->RegisterWindowClass(work2);
	m_console->CreateWnd(work2, 400, 600, top_X, top_Y, NULL, NULL, work2);
#endif

}

void	NPC_Brain::All_Reset(Character* character)
{
	if (!m_goal_Deque.empty())
	{
		m_goal_Deque.front()->Failed(character);
		m_goal_Deque.clear();
	}
	Accesor_Brain::Target_Clear(this);
	m_censor.m_attack_Me->Release();
	m_think->Reset_CurrentTarget_FinalGoal();
}

void	NPC_Brain::Intialize_Censor()
{
	m_censor.m_sight = new Censor_Sight(m_chara_Param.m_response_Sight, m_chara_Param.fov, m_chara_Param.sight, 5);
	m_censor.m_attack_Me = new Censor_AttackMe(m_chara_Param.m_responce_AttackMe);
	m_censor.m_friend_Message = new Censor_FriendMessage(m_chara_Param.m_responce_FriendMessage);
}

void	NPC_Brain::Set_FinalGoal()
{
	m_final_Goallist.reserve(10);

	m_final_Goallist.push_back(new Goal_KnockDownEnemy(this));

	m_final_Goallist.push_back(new Goal_GetAdvertisementPoint
			(this, goalcost_calculater::Test,new Behavior_Director_GetAdvertisementPoint()));

	m_final_Goallist.push_back(new Goal_FollowFriend
		(this, goalcost_calculater::Test, new Behavior_Director_FollowFriend()));

	m_final_Goallist.push_back
		(new Goal_MedicalFriend(this, goalcost_calculater::Test, new Behavior_Director_MedicalFriend()));

	m_final_Goallist.push_back
		(new Goal_CorrespondenceDenger(this, goalcost_calculater::Test, new Behavior_Director_CorrespondenceDenger()));

	m_final_Goallist.push_back
		(new Goal_WanderField(this, goalcost_calculater::Test, new Behavior_Director_WanderField()));

}

NPC_Brain::~NPC_Brain()
{
	SAFE_DELETE(m_message_Icon);
	SAFE_DELETE(m_path_Planner);
	SAFE_DELETE(m_particularGoal_Manager);
	SAFE_DELETE(m_evalutor);
	SAFE_DELETE(m_console);
	SAFE_DELETE(m_behavior_Manager);
	m_goal_Deque.clear();
	for (auto it = m_final_Goallist.begin(); it != m_final_Goallist.end(); it++)
	{
		SAFE_DELETE(*it);
	}
	m_final_Goallist.clear();
	SAFE_DELETE(m_planner);
	SAFE_DELETE(m_think);
	delete m_censor.m_friend_Message;
	m_censor.m_friend_Message = nullptr;
	delete m_censor.m_sight;
	m_censor.m_sight = nullptr;
	delete m_censor.m_attack_Me;
	m_censor.m_attack_Me = nullptr;

}

bool	NPC_Brain::HandleMessage(GameMessage* msg)
{
	switch (msg->type)
	{

	case msg_t::replanning :
		return Planning(static_cast<Character*>(msg->exinfo));
		break;

	case msg_t::damage_hit:
		m_censor.m_attack_Me->Register(this, static_cast<DamageParam*>(msg->exinfo));
		break;

	case msg_t::memory_update :
		return Management_Message_Memory(msg);
		break;

	case msg_t::apper_friend_msg_icon:
		return m_censor.m_friend_Message->ProcessMessage(this,msg);
		return false;
		break;
	default:
		break;
	}
	return true;
}

bool	NPC_Brain::Management_Message_Memory(GameMessage* msg)
{
	//現在ゴールが存在している場合はBehaviorに転送する
	if (!m_goal_Deque.empty())
	{
		return ai_message->Send_BehaviorMessage(this, (Character*)msg->exinfo, ai_messagetype::update_memory, nullptr);
	}
	//ゴールがない場合（リスポーン時）の処理
	if (m_status_Param.m_is_Arive)
	{
		//CONSOLE_PRINT("Management_Message_Memoryで%sがActiveになりました\r\n", character_Name);
		Log(FormatString("%dフレーム,Management_Message_Memoryで%sがActiveになりました\r\n", GAMEMATCHINFO.GetMatchTime(), character_Name));
		m_status_Param.m_is_Active = true;
	}
	return false;
}

void	NPC_Brain::Goal_Completed(LPCHARACTER p)
{
	if (m_goal_Deque.empty())
	{
#if NPC_ASSART
		MyAssert(false, "やばい");
#endif
		Planning(p);
	}
	m_goal_Deque.front()->Completed(p);
	//達成後取り除く
	if (m_goal_Deque.size() == 1)
	{
#if CONSOLE_BRAIN
		CONSOLE_PRINT("%sが達成されたのでプランニングを行います\n", m_goal_Deque.front()->goal_Name);
#endif
		Log(FormatString("%sが達成されたのでプランニングを行います\n", m_goal_Deque.front()->goal_Name));
	}
		
	m_goal_Deque.pop_front();
	if ( m_particularGoal_Manager->Is_Active())
		m_particularGoal_Manager->Exit();
	//取り除いたゴールが特殊ゴールだった場合はそのままreturn
	if (!m_goal_Deque.empty())
		if(m_goal_Deque.front()->status == goal_status::active)
			return;
	
		//ゴールスタックが空になった場合次のプランニングを行う
	if (m_goal_Deque.empty())
	{
		Planning(p);
	}


	m_goal_Deque.front()->status = goal_status::active;
	if (!m_goal_Deque.front()->Active(p))
	{
#if NPC_ASSART
		MyAssert(false, "%sのActivateに失敗", m_goal_Deque.front()->goal_Name);
#endif
		Log(FormatString("%sのActivateに失敗したのでプランニングを行います\r\n", m_goal_Deque.front()->goal_Name));
		Planning(p);
	}
}

void	NPC_Brain::Goal_Failed(Character* character)
{
#if NPC_ASSART
	MyAssert(!m_goal_Deque.empty(), "やばい");
#endif
	m_goal_Deque.front()->Failed(character);
	if (m_particularGoal_Manager->Is_Active())
		m_particularGoal_Manager->Exit();
#if CONSOLE_BRAIN
	CONSOLE_PRINT("%sのProcess失敗したのでプランニングを行います\r\n", m_goal_Deque.front()->goal_Name);
#endif
	Log(FormatString("%sのProcessに失敗したのでプランニングを行います\r\n", m_goal_Deque.front()->goal_Name));
	Planning(character);
}

void	NPC_Brain::Goal_Process(LPCHARACTER p)
{
	
}

bool	NPC_Brain::Is_Arive(Character* character)
{
	int current_Frame = GAMEMATCHINFO.GetMatchTime();


	CHARACTER_STATE_TYPE health = ChrFunc::GetStateType(character);
	if (health == CHARACTER_STATE_TYPE::die_mode && m_status_Param.m_is_Arive)
	{
		CONSOLE_PRINT("\r\n%dフレームで%sが死にました\r\n", current_Frame,character_Name);
		All_Reset(character);
		m_status_Param.m_is_Arive = false;
		m_status_Param.m_is_Active = false;
	}

	if (health == CHARACTER_STATE_TYPE::parachute && !m_status_Param.m_is_Arive)
	{
		m_status_Param.m_is_Arive = true;
		m_censor.m_sight->Look();
		m_status_Param.m_active_Timer = 20;
		if (!m_particularGoal_Manager->Insert_Goal(character, partculargoal_type::airborne))
		{
			MyAssert(false, "AirBorneGoalの挿入に失敗");
		}
	}

	if (health == CHARACTER_STATE_TYPE::usual_mode && !m_status_Param.m_is_Arive)
	{
		CONSOLE_PRINT("\r\n%dフレームで%sがリスポーンしました\r\n", current_Frame,character_Name);
		CONSOLE_PRINT("割り当てられたEntityID%d\n", character->GetId());
		m_status_Param.m_is_Arive = true;
		m_censor.m_sight->Look();
		m_status_Param.m_active_Timer = 20;
	}

	if (m_status_Param.m_active_Timer > 0)
	{
		m_status_Param.m_active_Timer--;
		if (m_status_Param.m_active_Timer <= 0 && !m_status_Param.m_is_Active)
		{
			m_status_Param.m_is_Active = true;
		}
	}

	return (m_status_Param.m_is_Active) ? true : false;
}

void	NPC_Brain::Update(LPCHARACTER character)
{
	int current_Frame = GAMEMATCHINFO.GetMatchTime();
	//記憶更新
	m_memory.Update(current_Frame, character);
	//censor更新
	Update_Censor(character);

	if (!Is_Arive(character))
		return;

	m_message_Icon->Update(character);
	//ゴールスタックが空になってしまっている場合はプランニングを行う
	if (m_goal_Deque.empty())
	{
#if CONSOLE_BRAIN
		CONSOLE_PRINT("%sのgoalDequeが空なのでプランニングを行います\r\n", character_Name);
#endif
		Planning(character);
	}
	m_path_Planner->Update(character);
	goal_status  status = m_goal_Deque.front()->Process(character) ;
	switch (status)
	{
	case goal_status::active:		//このアクティブは現在実行中のことを示す（分かりにくいかも）
		Goal_Process(character);
		break;

	case goal_status::completed:
		Goal_Completed(character);
		break;

	case goal_status::failed:
		Goal_Failed(character);
		break;
	}
}

MapObject_Record*	NPC_Brain::Get_MapObjectMemory(const ENTITY_ID& ID, float* reliability)
{
	if (!IS_MAPOBJ_ID(ID))
	{
#if NPC_ASSART
		MyAssert(false, "Get_CharacterMemoryでMapObject以外の記憶を参照しようとしました ID %d", (int)ID);
#endif		
		return nullptr;
	}

	MapObject_Record*ret = m_memory.memory.Get_MapObjectMemory(ID,
											m_chara_Param.record_mapobject_ReliabilityTime, reliability);
	if (!ret)
		return nullptr;

	if (ret->m_object_type == mapobjcet_type::poster)
		return ret;
	//信頼値計算
	/*ここに*/

	return (*reliability > .0f) ? ret : nullptr;
}

Character_Record*		NPC_Brain::Get_CharacterMemory(const ENTITY_ID& ID, float* reliability)
{
	if (!IS_CONTROLLOBJ_ID(ID))
	{
//#if NPC_ASSART
	//	MyAssert(false, "Get_CharacterMemoryでCharacter以外の記憶を参照しようとしました ID %d", (int)ID);
//#endif
		return nullptr;
	}
	Character_Record*ret = m_memory.memory.Get_CharacterMemory(ID,
		m_chara_Param.record_mapobject_ReliabilityTime, reliability);
	if (*reliability > .0f) return ret;
	return nullptr;
}

void	NPC_Brain::Clear_FinalGoalStatus()
{
	for (auto it : m_final_Goallist)
	{
		it->status = goal_status::inactive;
	}
}

bool	NPC_Brain::Planning(Character* character)
{
	ProgramSpeedCheck::Begin();
	//死亡しているときはプランニングを行わない
	if (!m_status_Param.m_is_Active)
		return false;

	if (!m_goal_Deque.empty())
	{
		m_behavior_Manager->Exit_Current_Behavior(character);
		m_goal_Deque.clear();
	}
	//経路探索申請クラス初期化
	m_path_Planner->Exit();
	//ターゲット情報初期化
	Accesor_Brain::Target_Clear(this);
	m_planner->All_Release();
	m_planner->Clear_GoalStatus();
	Clear_FinalGoalStatus();

	int	game_Time = GAMEMATCHINFO.GetMatchTime();
	//CONSOLE_PRINT("%dフレームで%sのプランニングが行われました\r\n",game_Time, character_Name);
	Log(FormatString("%dフレームで%sのプランニングが行われました\r\n", game_Time, character_Name));

	bool	ret = false;
	//最終ゴールを評価した後、symbolを初期化、更新する（最終ゴール評価後でないと更新できないsymbolメンバがあるため）
	ProgramSpeedCheck::Begin();
	m_symbol.Initialize();
	m_symbol.Update_IsDanger(this, character);
	Action_Node* final_Goal = m_think->Thinking(character);
	DWORD time = ProgramSpeedCheck::GetTime();
	//FriendMessageの評価
	m_censor.m_friend_Message->Check_ThinkingResult(this,m_think->Get_FinalGoal());
	m_symbol.Update(this, character);

	ret = m_planner->Plannning(m_symbol,character, &m_goal_Deque, final_Goal);
	//float  time = (float)ProgramSpeedCheck::GetTime() * 0.001f;
	ENTITY_ID targetID = (m_think->Get_FinalGoal()->Get_TargetID());
	//最終ゴールのMessageBias初期化
	m_think->Reset_MessageBias();

	CONSOLE_PRINT("最終ターゲット%d\n", (int)targetID);
	if (!m_goal_Deque.front()->Active(character))
	{
#if NPC_ASSART
		MyAssert(false, "Planning直後、%sのActive失敗、やばい", m_goal_Deque.front()->goal_Name);
#endif
		//メッセンジャークリア
		m_censor.m_friend_Message->Clear();
		//とりあえず再起的に呼び出してみる
		Planning(character);
	}
	m_censor.m_friend_Message->Clear();
	return ret;
}

void	NPC_Brain::Update_Censor(Character* character)
{
	m_censor.m_attack_Me->Update(this, character);
	m_censor.m_sight->Update(this, character);
}

void	NPC_Brain::Log(const std::string& str)
{
#if NPC_LOG
	m_console->ShowText(str,color::color_type::BLACK);
#endif
}

bool	NPC_Brain::Insert_Goal(Character* character, Goal*insert_Goal)
{
	insert_Goal->status = goal_status::active;
	if (!insert_Goal->Active(character))
	{
#if NPC_ASSART
		MyAssert(false, "やばいっぽい");
#endif
		//とりあえず
		ai_message->Plannning(this, character,
			&std::string("NPC_Brain::InsertGoal"));
		return false;
	}
	m_goal_Deque.push_front(insert_Goal);
	return true;
}
