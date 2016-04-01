//#include "All.h"
//
//
////****************************************************//
////�@������
////****************************************************//
//
//FlagMatchState::FlagMatchState(const CODMatchData& match_data) :
//m_pScreenSeparation(nullptr),
//m_pCamera(nullptr),
//m_pControllerMgr(nullptr),
//m_MatchData(match_data)
//{
//
//}
//
//void FlagMatchState::Initialize()
//{
//
//	//Lua�̃L�[�쐬
//	CreateLuaState();
//
//	//�L�����N�^�̒萔�p�����[�^�[��ǂݍ���
//	MyAssert(LoadLuaCharacterParam("DATA\\ScriptFile\\CharacterConstParam.txt"), "�L�����N�^�̒萔�p�����[�^�[�ǂݍ��݂Ɏ��s���܂����B");
//
//	//	���ݒ�
//	iexLight::SetAmbient(0x404040);
//	iexLight::SetFog(800, 1000, 0);
//
//	Vector3 dir(1.0f, -1.0f, -0.5f);
//	dir.Normalize();
//	iexLight::DirLight(shader, 0, &dir, 0.8f, 0.8f, 0.8f);
//
//	//�f�o�b�N�p�̃m�[�h�}�l�[�W���쐬
//	Field_Debugger::Create_Instance();
//	field_Debug->Initialize(m_MatchData.computer_num, 1.0, 1.0f);
//
//	//�X�e�[�W�}�l�[�W���̎��̂��쐬
//	MAP_MANAGER.CreateInstance();
//	MyAssert(MAP_MANAGER.LoadObject("DATA\\stage_mesh\\test.cod","DATA\\stage_mesh\\MeshList.txt"), "�}�b�v�I�u�W�F�N�g���[�h�Ɏ��s���܂���");
//
//	//�_���[�W�}�l�[�W��
//	m_pDamageObjectManager = new DamageObjectManager;
//
//	//�G�C���^�[�Q�b�g�}�l�[�W��
//	m_pAimTargetManager = new AimTargetManager;
//
//	//��ʕ����ݒ�
//	m_pScreenSeparation = new ScreenSeparation(m_MatchData.player_num,);
//
//	//�R���g���[���}�l�[�W���̎��̂��쐬
//	m_pControllerMgr = new ControllerManager;
//
//	//�R���g���[���I�u�W�F�N�g�}�l�[�W���̎��̂��쐬
//	CONTROLLOBJMGR.InstanceCreate();
//
//	//�e�}�l�[�W�����쐬
//	m_pBulletManager = new BulletManager;
//
//	//�L�����N�^�[�쐬
//	MyAssert(CreateCharacters(), "�L�����N�^�쐬�Ɏ��s���܂���");
//}
//
//
//
////****************************************************//
////�@���
////****************************************************//
//
//FlagMatchState::~FlagMatchState()
//{
//	//���
//	SAFE_DELETE(m_pCamera);
//	SAFE_DELETE(m_pScreenSeparation);
//	SAFE_DELETE(m_pControllerMgr);
//	SAFE_DELETE(m_pBulletManager);
//	SAFE_DELETE(m_pDamageObjectManager);
//	CONTROLLOBJMGR.InstanceRelase();
//	Field_Debugger::Release();
//	MAP_MANAGER.Release();
//	SAFE_DELETE(m_pAimTargetManager);	//�G�C���^�[�Q�b�g��delete�𒼑O�Ɏ��s����K�v������
//	
//	IDMANAGER.Release();	//����͍Ō��
//}
//
////****************************************************//
////�@���t���[���̍X�V
////****************************************************//
//void FlagMatchState::Update()
//{
//
//	ProgramSpeedCheck::Begin();
//
//	//�f�o�b�O�p������������
//	ITDEBUGSTRMGR.Clear();
//
//	//�}�b�v�I�u�W�F�N�g��`��
//	FUNCTIONSPEEDCHECK(MAP_MANAGER.Update());
//
//	//�R���g���[���ł���I�u�W�F�N�g�̍X�V
//	FUNCTIONSPEEDCHECK(CONTROLLOBJMGR.Update());
//
//	//�R���g���[���[�Ǘ��N���X�̃A�b�v�f�[�g
//	FUNCTIONSPEEDCHECK(m_pControllerMgr->Update());
//
//	//�e�ۂ̍X�V
//	FUNCTIONSPEEDCHECK(m_pBulletManager->Update());
//
//	iexParticle::Update();
//
//	MyDebugMgrStr(color_smp::RED, "Update�o�ߎ��� %d ms", (int)ProgramSpeedCheck::GetTime());
//}
//
////****************************************************//
////�@�`��
////****************************************************//
////�v���C���[�Ȃ�true
//static bool select_player_pred(LPCONTROLLER c)
//{
//	return c->GetMatchData().player_type == player_type::PLAYER;
//}
//
//void FlagMatchState::Render()
//{
//	ProgramSpeedCheck::Begin();
//
//	//�J�������X�g�쐬
//	CONTROLLOBJLIST obj_list;
//
//	//�Ƃ肠�����f�o�b�O�悤�J��������荞��
//	//	camera_list.push_back(m_pCamera);
//
//	//�L�����N�^�̃J����������Ă݂�
//	CONTROLLERLIST controller_list = m_pControllerMgr->GetController(select_player_pred);
//
//	for (CONTROLLERLIST::iterator it = controller_list.begin();
//		it != controller_list.end();
//		++it)
//		obj_list.push_back((*it)->GetControllObjectPtr());
//
//	//�X�N���[���z�u���W���쐬
//	std::list<RECTI>	screen_positions;
//	CreateScreenRect(&screen_positions, m_MatchData.player_num);
//
//	//�z�u
//	m_pScreenSeparation->SetScreen(screen_positions);
//
//	//��ʃe�N�X�`�����쐬
//	m_pScreenSeparation->CreateScreenTexture(obj_list, this);
//
//	//�`��
//	m_pScreenSeparation->DrawScreen();
//
//	MyDebugMgrStr(color_smp::BLACK, "Render�o�ߎ��� %d ms", (int)ProgramSpeedCheck::GetTime());
//
//	//�f�o�b�O�p������`��
//	ITDEBUGSTRMGR.Render();
//}
//
//bool FlagMatchState::CreateCharacters()
//{
//	//�L�����N�^���쐬
//	auto it = m_MatchData.player_data.begin();
//	controller::CONTROLLER_NUM controller_num = 0;
//	LPCHARACTER	pCharacter;
//
//	while (it != m_MatchData.player_data.end())
//	{
//		const PLAYERMATCHDATA& data = *it;
//
//		switch (data.player_type)
//		{
//		case player_type::PLAYER:
//			ChrFunc::GetNewPlayerCharacter(&pCharacter, &data.button_set);
//			break;
//		case player_type::COMPUTER:
//
//		case player_type::SPECTATOR:
//		default:
//			MyAssert(false, "�v���C���[�^�C�v���Ԉ���Ă��܂�");
//			break;
//		}
//
//		//�J�����X�e�[�g���Z�b�g
//		LPCAMERA cam = ControllObjFunc::GetCamera(pCharacter);
//		cam->SetPos(Vector3(0, 40, -10));
//		cam->SetTarget(Vector3(0, 35, 0));
//		cam->SetNewState(new UsualPlayerMoveCamera(cam, pCharacter, 0.15f, 0.09f, Vector3(0.6f, 0.9f, -2.4f), Vector3(0.6f, 0.9f, 1)));
//
//		//�R���g���[���쐬
//		Controller* pController = new Controller(data);
//
//		//�쐬�����R���g���[���̑��삷��I�u�W�F�N�g�ɃL�����N�^���Z�b�g
//		pController->SetControllObj(pCharacter);
//		m_pControllerMgr->AddController(pController);
//
//		//�R���g���[���}�l�[�W���ɒǉ�
//		CONTROLLOBJMGR.AddObject(pCharacter);
//	}
//
//	return true;
//}
//
//void FlagMatchState::GameRender(LPIEXVIEW pView, ControllObject	*pOwner)
//{
//	//�r���[�s��ݒ�
//	pView->Activate();
//
//	//�r���[�|�[�g�Z�b�g
//	m_pScreenSeparation->Set_ViewPort(m_MatchData.player_num);
//
//	//�N���A
//	pView->Clear();
//
//	//�}�b�v�I�u�W�F�N�g��`��
//	MAP_MANAGER.Render();
//
//	//�R���g���[���\�ȃI�u�W�F�N�g��`��
//	CONTROLLOBJMGR.Render();
//
//	//�e�`��
//	m_pBulletManager->Render();
//
//	iexParticle::Render();
//
//	pOwner->RenderUI();
//}
//
////****************************************************//
////�@�Q�[���J�n
////****************************************************//
//void FlagMatchState::GameStart()
//{
//
//}
//
////****************************************************//
////�@�Q�[���I���m�F
////****************************************************//
//bool FlagMatchState::isGameFinish()
//{
//	return false;
//}
//
