#include "All.h"


//****************************************************//
//�@������
//****************************************************//

static int numPlayer;

DebugMatchState::DebugMatchState():
m_pScreenSeparation(nullptr),
m_pCamera(nullptr),
m_pControllerMgr(nullptr)
{

}

void DebugMatchState::Initialize()
{
	debug_particle::g_pNumberTexture = new iex2DObj("DATA\\number.png");

	numPlayer = GAMEMATCHINFO.GetMatchData().player_num;

	//Lua�̃L�[�쐬
	CreateLuaState();

	//�Q�[���̒萔�p�����[�^�[��ǂݍ���
	MyAssert(LoadLuaGameParam("DATA\\ScriptFile\\GameSystemConstParam.txt"), "�L�����N�^�̒萔�p�����[�^�[�ǂݍ��݂Ɏ��s���܂����B");

	//�L�����N�^�̒萔�p�����[�^�[��ǂݍ���
	MyAssert(LoadLuaCharacterParam("DATA\\ScriptFile\\CharacterConstParam.txt"), "�L�����N�^�̒萔�p�����[�^�[�ǂݍ��݂Ɏ��s���܂����B");

	//	���ݒ�
	iexLight::SetAmbient(0x404040);
	iexLight::SetFog(800, 1000, 0);

	Vector3 dir(1.0f, -1.0f, -0.5f);
	dir.Normalize();
	iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);

	//�f�o�b�N�p�̃m�[�h�}�l�[�W���쐬
	Field_Debugger::Create_Instance();
	field_Debug->Initialize(GAMEMATCHINFO.GetMatchData().computer_num, 1.0, 1.0f);

	//���b�V���}�l�[�W���쐬
	GAMEMATCHINFO.GetMeshMgr().MeshRegister(new MYMESH("DATA\\OBJECT\\ColaWater\\ColaWater.imo"), (MeshManager::MESH_ID)GameMatchInfomation::_COLA_WATER);
	GAMEMATCHINFO.GetMeshMgr().MeshRegister(new MYMESH("DATA\\OBJECT\\ColaWater\\ColaWater.imo"), (MeshManager::MESH_ID)GameMatchInfomation::_COLA_GUN);


	//�G�C���^�[�Q�b�g�}�l�[�W��
	m_pAimTargetManager = new AimTargetManager;

	//�_���[�W�}�l�[�W��
	m_pDamageObjectManager = new DamageObjectManager;

	//�X�e�[�W�}�l�[�W���̎��̂��쐬
	MAP_MANAGER.CreateInstance();
	MyAssert(MAP_MANAGER.LoadObject("DATA\\stage_mesh\\test2.cod","DATA\\stage_mesh\\MeshList.txt"), "�}�b�v�I�u�W�F�N�g���[�h�Ɏ��s���܂���");



	//��ʕ����ݒ�
	m_pScreenSeparation = new ScreenSeparation(numPlayer,new ScreenCreater_DebugMatch());

	//�R���g���[���}�l�[�W���̎��̂��쐬
	m_pControllerMgr = new ControllerManager;

	//AI�̃t�B�[���h�쐬
	navigation_system->Initialize("DATA\\NPC\\StageNodeData\\node.txt", "DATA\\NPC\\StageNodeData\\waypointinfo.txt");
	//field_Info->Load_Node();
	//field_Info->Load_Visibility("DATA\\NPC\\StageNodeData\\visibility.txt");
	//�����ǂݍ���
	env_Sound->Initialize();
	Rand::Initialize();
	ai_message->Create_Insatnce();

	//�R���g���[���I�u�W�F�N�g�}�l�[�W���̎��̂��쐬
	CONTROLLOBJMGR.InstanceCreate();

	static char* name[4] =
	{
		"jack",
		"devid",
		"snake",
		"hal",
	};
	DWORD color[2] =
	{
		dword_color::red,
		dword_color::white,
	};

	int nPlayer = GAMEMATCHINFO.GetMatchData().player_num;

	//�L�����N�^�Ƃ肠��������Ă݂�
	for (int i = game_id::id_controll_obj_first, controller_cnt = 0; i < game_id::id_controll_obj_first + GAMEMATCHINFO.GetMatchData().all_player_num; i++, nPlayer--)
	{
		//�R���g���[���{�^���z�u�\����
		ControllerButtonSet button_set;

		//�f�t�H���gButton�z�u���Z�b�g
		SetUsualButtonSet(button_set, controller_cnt++);

		CharacterUI* chr_ui = new CharacterUI;

		ENTITY_ID id;
		MyAssert(IDMANAGER.GetEmptyControllObjID(&id), "ID�ɋ󂫂�����܂���");

		CharacterBrainBase* pBRAIN;
		if (nPlayer > 0)
			pBRAIN = new CharacterBrainRealPlayer(&button_set);
		else
			int a = 0;
			/*pBRAIN = new NPC_Brain(
			name[i - game_id::id_controll_obj_first],
			i - game_id::id_controll_obj_first);*/

		LPCHARACTER pCharacter = new Character(
			id,												
			pBRAIN,
			new CharacterModelResouce("DATA\\CHR\\COLA_KUN\\cola_kun.iem",0),	//�e�X�g�p���f���`��N���X�������
			//new TestCharacterModel("DATA\\CHR\\cola_test.IEM"),
			chr_ui,
			0,//////,
			nullptr///////
			);

		chr_ui->SetOwner(pCharacter);

		//�J�����X�e�[�g���Z�b�g
		LPCAMERA cam = ControllObjFunc::GetCamera(pCharacter);
		cam->SetPos(Vector3(0, 40, -10));
		cam->SetTarget(Vector3(0, 35, 0));
		cam->SetNewState(CameraFunction::GetCharacterUsualModeCamera(pCharacter, cam,CameraFunction::_RIGHT));

		//�R���g���[���쐬
		PLAYERMATCHDATA match_data;
		InitPlayerMatchData(&match_data);

		match_data.team_type = (i - game_id::id_controll_obj_first) % 2;	//�Ƃ肠�����`�[�����Q���
		match_data.button_set = button_set;
		match_data.player_num = i - game_id::id_controll_obj_first;
		match_data.player_type = (nPlayer > 0) ? (player_type::PLAYER) : (player_type::COMPUTER);

		Controller* pController = new Controller(match_data);

		//�쐬�����R���g���[���̑��삷��I�u�W�F�N�g�ɃL�����N�^���Z�b�g
		pController->SetControllObj(pCharacter);
		m_pControllerMgr->AddController(pController);

		//�R���g���[���}�l�[�W���ɒǉ�
		CONTROLLOBJMGR.AddObject(pCharacter);
		//�f�o�b�O�p�̌o�H�N���X�ɓo�^
		field_Debug->Register((ENTITY_ID)i, dword_color::red);
	}

	//�e�}�l�[�W�����쐬
	m_pBulletManager = new BulletManager;

	//�����܂�}�l�[�W�����쐬
	m_pColaWaterMgr = new WaterPoolManager;

	//�J�����ݒ�i�Ƃ肠�����f�邩�ǂ����j
	m_pCamera = new Camera;
	m_pCamera->SetPos(Vector3(10, 7, -20));
	m_pCamera->SetTarget(Vector3(0, 5, 0));
	m_pCamera->SetNewState(new FreezeCamera);

	POLYGONEFFECT.LoadEffectTextureData();
}



//****************************************************//
//�@���
//****************************************************//

DebugMatchState::~DebugMatchState()
{
	//���
	navigation_system->Release();
	field_Debug->Release();
	ai_message->Release_Instance();
	SAFE_DELETE(m_pColaWaterMgr);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pScreenSeparation);
	SAFE_DELETE(m_pControllerMgr);
	SAFE_DELETE(m_pBulletManager);
	CONTROLLOBJMGR.InstanceRelase();
	SAFE_DELETE(m_pDamageObjectManager);
	Field_Debugger::Release();
	MAP_MANAGER.Release();
	SAFE_DELETE(m_pAimTargetManager);	//�G�C���^�[�Q�b�g��delete�𒼑O�Ɏ��s����K�v������
	POLYGONEFFECT.Release();
	IDMANAGER.Release();	//����͍Ō��

	delete debug_particle::g_pNumberTexture;
}

//****************************************************//
//�@���t���[���̍X�V
//****************************************************//
void DebugMatchState::Update()
{
	//�f�o�b�O�p������������
	ITDEBUGSTRMGR.Clear();

	//�}�b�v�I�u�W�F�N�g��`��
	FUNCTIONSPEEDCHECK(MAP_MANAGER.Update());

	//AI��HandleMessage�X�V
	ai_message->Update();


	//�R���g���[���ł���I�u�W�F�N�g�̍X�V
//	FUNCTIONSPEEDCHECK(CONTROLLOBJMGR.Update());

	//�R���g���[���[�Ǘ��N���X�̃A�b�v�f�[�g
	FUNCTIONSPEEDCHECK(m_pControllerMgr->Update());

	//�e�ۂ̍X�V
	FUNCTIONSPEEDCHECK(m_pBulletManager->Update());

	navigation_system->Update();
	env_Sound->Update();

	if (GetAsyncKeyState('F') & 0x01)
	{
		env_Sound->Register(sound_type::gun, (int)GAMEMATCHINFO.GetMatchTime(), Vector3(-3.014f, -.0f, 0.623f), 1);
	}
	m_pColaWaterMgr->Update();

	iexParticle::Update();
	POLYGONEFFECT.Update();

	//MyDebugMgrStr(color_smp::RED, "Update�o�ߎ��� %d ms", (int)ProgramSpeedCheck::GetTime());
}

//****************************************************//
//�@�`��
//****************************************************//

//�v���C���[�Ȃ�true
static bool select_player_pred(LPCONTROLLER c)
{
	return c->GetMatchData().player_type == player_type::PLAYER;
}

void DebugMatchState::Render()
{

	//�J�������X�g�쐬
	CONTROLLOBJLIST obj_list;

	//�Ƃ肠�����f�o�b�O�悤�J��������荞��
	//	camera_list.push_back(m_pCamera);

	//�L�����N�^�̃J����������Ă݂�
	CONTROLLERLIST controller_list = m_pControllerMgr->GetController(select_player_pred);

	for (CONTROLLERLIST::iterator it = controller_list.begin();
		it != controller_list.end();
		++it)
		obj_list.push_back((*it)->GetControllObjectPtr());

	//�X�N���[���z�u���W���쐬
	std::list<RECTI>	screen_positions;
	CreateScreenRect(&screen_positions, GAMEMATCHINFO.GetMatchData().player_num);

	//�z�u
	m_pScreenSeparation->SetScreen(screen_positions);

	//��ʃe�N�X�`�����쐬
	m_pScreenSeparation->CreateScreenTexture(obj_list, this);

	//�`��
	m_pScreenSeparation->DrawScreen();

//	MyDebugMgrStr(color_smp::BLACK, "Render�o�ߎ��� %d ms", (int)ProgramSpeedCheck::GetTime());

	static bool dbgDraw=true;
	//�f�o�b�O�p������`��
	if (dbgDraw)ITDEBUGSTRMGR.Render();
	if (KEY(KEY_ENTER, 0) == 3)dbgDraw = !dbgDraw;

}

void DebugMatchState::GameRender(LPIEXVIEW pView, ControllObject	*pOwner)
{
	////�r���[�s��ݒ�
	//pView->Activate();

	////�r���[�|�[�g�Z�b�g
	////m_pScreenSeparation->Set_ViewPort(numPlayer);

	////�N���A
	//pView->Clear();
	//
	////�}�b�v�I�u�W�F�N�g��`��
	////MAP_MANAGER.Render("static_object");
	//MAP_MANAGER.Render();

	////�R���g���[���\�ȃI�u�W�F�N�g��`��
	////CONTROLLOBJMGR.Render("toon");
	//CONTROLLOBJMGR.Render();



	////�e�`��
	//m_pBulletManager->Render();
	////�����܂�`��
	//m_pColaWaterMgr->Render();
	//iexParticle::Render();
	//

	////field_Debug->Render_GoalNode();
	////field_Debug->Reder_GoalRoute();
	////field_Debug->Render_AllEdge();
	//field_Debug->Render_Edge(ControllObjFunc::GetPos(pOwner));
	//pOwner->RenderUI();
}

void	DebugMatchState::Water_Render(LPIEXVIEW pView, ControllObject*pOwner)
{
	m_pColaWaterMgr->Render();
}

//****************************************************//
//�@�Q�[���J�n
//****************************************************//
void DebugMatchState::GameStart()
{

}

//****************************************************//
//�@�Q�[���I���m�F
//****************************************************//
bool DebugMatchState::isGameFinish()
{
	return false;
}

