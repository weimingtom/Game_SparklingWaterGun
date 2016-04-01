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
//// エフェクトのタイプ
////モデルNO,ルーチンNO,時間
//// -1なら無限
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
//	// エフェクト管理用インスタンスの生成
//	g_manager= ::Effekseer::Manager::Create(EFFECT_READ_MAX);
//
//	// 描画用インスタンスから描画機能を設定
//	g_manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
//	g_manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
//	g_manager->SetRingRenderer(renderer->CreateRingRenderer());
//	g_manager->SetModelRenderer(renderer->CreateModelRenderer());
//
//	// 描画用インスタンスからテクスチャの読込機能を設定
//	//独自拡張可能、現在はファイルから読み込んでいる。
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
//	// 投影行列を設定
//	::Effekseer::Matrix44 work;
//	work = (TranslateMatrix44(matView));
//
//	g_renderer->SetCameraMatrix(work);
//
//	work = (TranslateMatrix44(matProjection));
//
//	g_renderer->SetProjectionMatrix(work);
//
//	// カメラ行列を設定
//	g_renderer->SetCameraMatrix(
//		::Effekseer::Matrix44().LookAtLH(::Effekseer::Vector3D(cpos.x, cpos.y, cpos.z), ::Effekseer::Vector3D(ctarget.x, ctarget.y, ctarget.z), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
//
//	g_manager->Update();            // 更新処理
//
//}
//
//void EffekseerDate::Render(::Effekseer::Handle* handle)
//{
//	g_manager->DrawHandle(*handle);					// 描画
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
//// エフェクトの再生
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
//// エフェクトの停止
//void EffekseerBase::StopEffect()
//{
//	p_effectdata->g_manager->StopEffect(g_handle);
//	Alive = false;
//	p_effectdata->count--;
//}
//
//// パラメータ設定
//void EffekseerBase::SetParam(const Vector3& pos, const Vector3& scale, const Vector3& angle)
//{
//	p_effectdata->g_manager->SetLocation(g_handle, pos.x, pos.y, pos.z);//位置
//	p_effectdata->g_manager->SetRotation(g_handle, Effekseer::Vector3D(angle.x, angle.y, angle.z), 0.0f);//回転
//	p_effectdata->g_manager->SetScale(g_handle, scale.x, scale.y, scale.z);//倍率
//}
//
//// 指定した値に移動する
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
//	case ERN_TIMER: // 時間型(-1:無限)
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
//// 初期化
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
//*初期化
//*/
//void EffekseerManager::Init()
//{
//	const int spriteMax = 300;
//	// 描画用インスタンスの生成
//	renderer = ::EffekseerRendererDX9::Renderer::Create(iexSystem::GetDevice(), spriteMax);
//
//	for (int i = 0; i < EFFECT_TYPE_MAX; i++)
//	{
//		p_effectdata[i]->Init(renderer);
//	}
//}
//
//
//// デストラクタ
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
//	renderer->Destory();			// 描画用インスタンスを破棄
//}
//
//// エフェクト設定
//void EffekseerManager::SetEffect(WCHAR* filepath, int effectNo)
//{
//	p_effectdata[effectNo]->g_effect = Effekseer::Effect::Create(p_effectdata[effectNo]->g_manager, (const EFK_CHAR*)filepath);
//}
//
//// エフェクトの再生
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
//// エフェクトの停止
//void EffekseerManager::StopEffect(int effectNo)
//{
//	effect[effectNo]->StopEffect();
//}
//
//
//// どう移動するか
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
//	renderer->BeginRendering();						// 描画開始処理
//
//	for (int i = 0; i < EFFECT_SIGUP_MAX; i++)
//	{
//		if (effect[i]->Alive)
//		{
//			effect[i]->Render();
//		}
//	}
//	renderer->EndRendering();							// 描画終了
//}
