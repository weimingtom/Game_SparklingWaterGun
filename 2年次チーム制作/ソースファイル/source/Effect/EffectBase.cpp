//#include	"iextreme.h"
//#include	"system/system.h"
//#include "sceneMain.h"
//
//#include <Effekseer.h>
//#include <EffekseerRendererDX9.h>
//#include <EffekseerSoundXAudio2.h>
//
//#pragma comment(lib,"Effekseer.lib")
//#pragma comment(lib,"EffekseerRendererDX9.lib")
//
//#include "EffectBase.h"
//
//
//// �G�t�F�N�g�̃^�C�v
////���f��NO,���[�`��NO,����
//// -1�Ȃ疳��
//int Type[][3] =
//{
//	{ EMN_SPHERE,ERN_TIMER,100},
//	{ EMN_TORNADO, ERN_TIMER,250},
//};
//
//Effekseer::Matrix44 TranslateMatrix44(const Matrix& mat){
//
//	Effekseer::Matrix44 ret;
//
//	for (int i = 0; i < 4; i++){
//		for (int j = 0; j < 4; j++){
//			ret.Values[i][j] = mat.m[i][j];
//		}
//	}
//	return ret;
//}
//
//Effekseer::Matrix43 TranslateMatrix43(const Matrix& mat){
//
//	Effekseer::Matrix43 ret;
//
//	for (int i = 0; i < 4; i++){
//		for (int j = 0; j < 3; j++){
//			ret.Value[i][j] = mat.m[i][j];
//		}
//	}
//	return ret;
//}
//
//
//EffekseerDate::EffekseerDate():
//g_effect(NULL), 
//g_manager(NULL),
//g_renderer(NULL),
//count(0)
//{
//}
//
//EffekseerDate::~EffekseerDate()
//{
//	SAFE_RELEASE(g_effect);
//	if (g_manager)g_manager->Destroy();			
//}
//
//	
//void EffekseerDate::Init(::EffekseerRenderer::Renderer	*renderer)
//{
//	g_renderer = renderer;
//
//	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
//	g_manager= ::Effekseer::Manager::Create(EFFECT_READ_MAX);
//
//	// �`��p�C���X�^���X����`��@�\��ݒ�
//	g_manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
//	g_manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
//	g_manager->SetRingRenderer(renderer->CreateRingRenderer());
//	g_manager->SetModelRenderer(renderer->CreateModelRenderer());
//
//	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
//	//�Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
//	g_manager->SetTextureLoader(renderer->CreateTextureLoader());
//	g_manager->SetModelLoader(renderer->CreateModelLoader());
//	g_manager->SetCoordinateSystem(::Effekseer::COORDINATE_SYSTEM_LH);
//}
//
//
//void EffekseerDate::Update(const Vector3& cpos, const Vector3& ctarget)
//{
//	if (count == 0)return;
//
//	// ���e�s���ݒ�
//	::Effekseer::Matrix44 work;
//	work = (TranslateMatrix44(matView));
//
//	g_renderer->SetCameraMatrix(work);
//
//	work = (TranslateMatrix44(matProjection));
//
//	g_renderer->SetProjectionMatrix(work);
//
//	// �J�����s���ݒ�
//	g_renderer->SetCameraMatrix(
//		::Effekseer::Matrix44().LookAtLH(::Effekseer::Vector3D(cpos.x, cpos.y, cpos.z), ::Effekseer::Vector3D(ctarget.x, ctarget.y, ctarget.z), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
//
//	g_manager->Update();            // �X�V����
//
//}
//
//void EffekseerDate::Render(::Effekseer::Handle* handle)
//{
//	g_manager->DrawHandle(*handle);					// �`��
//}
//
//
//
//EffekseerBase::EffekseerBase():
//g_renderer(NULL),
//g_handle(0),
//Alive(false),
//p_effectdata(NULL),
//EffectNo(-1)
//{
//}
//
//
//EffekseerBase::~EffekseerBase()
//{
//}
//
//// �G�t�F�N�g�̍Đ�
//void EffekseerBase::PlayEffect(int type,EffekseerDate* edata,const Vector3& pos, const Vector3& scale, const Vector3& angle)
//{
//	EffectNo = Type[type][1];
//	Time = Type[type][2];
//	p_effectdata = edata;
//	g_handle = edata->g_manager->Play(edata->g_effect, 0.0f, 0.0f, 0.0f);
//
//	SetParam(pos,scale,angle);
//	Alive = true;
//	edata->count++;
//}
//
//
//// �G�t�F�N�g�̒�~
//void EffekseerBase::StopEffect()
//{
//	p_effectdata->g_manager->StopEffect(g_handle);
//	Alive = false;
//	p_effectdata->count--;
//}
//
//// �p�����[�^�ݒ�
//void EffekseerBase::SetParam(const Vector3& pos, const Vector3& scale, const Vector3& angle)
//{
//	p_effectdata->g_manager->SetLocation(g_handle, pos.x, pos.y, pos.z);//�ʒu
//	p_effectdata->g_manager->SetRotation(g_handle, Effekseer::Vector3D(angle.x, angle.y, angle.z), 0.0f);//��]
//	p_effectdata->g_manager->SetScale(g_handle, scale.x, scale.y, scale.z);//�{��
//}
//
//// �w�肵���l�Ɉړ�����
//void EffekseerBase::SetMove(const Vector3& move)
//{
//	p_effectdata->g_manager->AddLocation(g_handle, Effekseer::Vector3D(move.x, move.y, move.z));
//}
//
//void EffekseerBase::Update()
//{
//
//	switch (EffectNo)
//	{
//	case ERN_TIMER: // ���Ԍ^(-1:����)
//		if (Time == -1)break;
//		Time--;
//		if (Time >= 0)	break;;
//
//		StopEffect();
//		break;
//	}
//}
//
//void EffekseerBase::Render()
//{
//	p_effectdata->Render(&g_handle);
//}
//
//
//// ������
//EffekseerManager::EffekseerManager()
//{
//	for (int i = 0; i < EFFECT_SIGUP_MAX;i++)
//	{
//
//		effect[i] = new EffekseerBase();
//	}
//
//	for (int i = 0; i < EFFECT_TYPE_MAX; i++)
//	{
//
//		p_effectdata[i] = new EffekseerDate();
//	}
//}
//
///*
//*������
//*/
//void EffekseerManager::Init()
//{
//	const int spriteMax = 300;
//	// �`��p�C���X�^���X�̐���
//	renderer = ::EffekseerRendererDX9::Renderer::Create(iexSystem::GetDevice(), spriteMax);
//
//	for (int i = 0; i < EFFECT_TYPE_MAX; i++)
//	{
//		p_effectdata[i]->Init(renderer);
//	}
//}
//
//
//// �f�X�g���N�^
//EffekseerManager::~EffekseerManager()
//{
//	for (int i = 0; i < EFFECT_SIGUP_MAX; i++)
//	{
//		delete effect[i];
//	}
//
//	for (int i = 0; i < EFFECT_TYPE_MAX; i++)
//	{
//		delete p_effectdata[i];
//	}
//	renderer->Destory();			// �`��p�C���X�^���X��j��
//}
//
//// �G�t�F�N�g�ݒ�
//void EffekseerManager::SetEffect(WCHAR* filepath, int effectNo)
//{
//	p_effectdata[effectNo]->g_effect = Effekseer::Effect::Create(p_effectdata[effectNo]->g_manager, (const EFK_CHAR*)filepath);
//}
//
//// �G�t�F�N�g�̍Đ�
//void EffekseerManager::PlayEffect(int effectType, const Vector3& pos, const Vector3& scale, const Vector3& angle)
//{
//	int effectNo = Type[effectType][0];
//	if (p_effectdata[effectNo] == NULL)return;
//
//	for (int i = 0; i < EFFECT_SIGUP_MAX; i++)
//	{
//		if (effect[i]->Alive)continue;
//		effect[i]->SetEffectManager(p_effectdata[effectNo]->g_effect, p_effectdata[effectNo]->g_manager, renderer);
//		effect[i]->PlayEffect(effectType, p_effectdata[effectType], pos, scale, angle);
//		break;
//	}
//}
//
//// �G�t�F�N�g�̒�~
//void EffekseerManager::StopEffect(int effectNo)
//{
//	effect[effectNo]->StopEffect();
//}
//
//
//// �ǂ��ړ����邩
//void EffekseerManager::SetMove(const Vector3& move,int effectNo)
//{
//	effect[effectNo]->SetMove(move);
//}
//
//void EffekseerManager::Update(const Vector3& cpos, const Vector3& ctarget)
//{
//	for (int i = 0; i < EFFECT_SIGUP_MAX; i++)
//	{
//		if (effect[i]->Alive)
//		{
//			effect[i]->Update();
//		}
//	}
//
//	for (int i = 0; i < EFFECT_TYPE_MAX; i++)
//	{
//		p_effectdata[i]->Update(cpos, ctarget);
//	}
//}
//
//void EffekseerManager::Render()
//{
//	renderer->BeginRendering();						// �`��J�n����
//
//	for (int i = 0; i < EFFECT_SIGUP_MAX; i++)
//	{
//		if (effect[i]->Alive)
//		{
//			effect[i]->Render();
//		}
//	}
//	renderer->EndRendering();							// �`��I��
//}
