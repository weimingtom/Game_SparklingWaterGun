//#ifndef __EFFECT_BASE_H__
//#define __EFFECTBASE_H__
//
///*
//Effekseerのライブラリー
//*/
//
//
//#include <Effekseer.h>
//#include <EffekseerRendererDX9.h>
//#include <EffekseerSoundXAudio2.h>
//
//#pragma comment(lib,"EffekseerRendererDX9.lib")
//#pragma comment(lib,"Effekseer.lib")
//
//#include "iextreme.h"
//
//// エフェクトの読み込み数 
//#define EFFECT_READ_MAX 2000
//
////エフェクトの最大出現数
//#define EFFECT_SIGUP_MAX	1000
//
//#define EFFECT_TYPE_MAX 10
//#define SAFE_RELEASE(x) if(x){(x)->Release();}
//
//// エフェクトモデルタイプ
//enum EFFECT_MODEL_NO
//{
//	EMN_SPHERE,				//	スフィア
//	EMN_TORNADO			//	竜巻
//};
//
//// ルーチンタイプ
//enum EFFECT_ROUTINE_NO
//{
//	ERN_TIMER			//	時間での消滅
//};
//
//
//
//
//class EffekseerDate
//{
//public:
//	::Effekseer::Effect*		g_effect;
//	::Effekseer::Manager*	g_manager;
//	::EffekseerRenderer::Renderer	*g_renderer;
//	int count;
//public:
//	EffekseerDate();
//	~EffekseerDate();
//public:
//	void Init(::EffekseerRenderer::Renderer	*renderer);
//	void Update(const Vector3& cpos, const Vector3& ctarget);
//	void Render(::Effekseer::Handle* handle);
//};
//
//
//class EffekseerBase
//{
//private:
//	EffekseerDate* p_effectdata;
//	::EffekseerRenderer::Renderer	*g_renderer;
//	::Effekseer::Handle							g_handle;
//public:
//	bool Alive;	//	生存判定
//	int EffectNo;
//	int Time;
//public:
//	EffekseerBase();
//	~EffekseerBase();
//
//	void Update();
//	void Render();
//
//	void PlayEffect(												//	エフェクト再生
//		int type,
//		EffekseerDate* edata,
//		const Vector3& pos,
//		const Vector3& scale,
//		const Vector3& angle
//		);				
//	void StopEffect();												//	エフェクト停止
//	void SetMove(const Vector3& move);				//	指定した値に移動する
//	void SetParam(												//	パラメータ設定
//		const Vector3& pos,
//		const Vector3& scale,
//		const Vector3& angle
//		);
//
//	inline void SetEffectManager(::Effekseer::Effect*	effect, ::Effekseer::Manager*manager, ::EffekseerRenderer::Renderer	*renderer)
//	{
//		g_renderer = renderer;
//	}
//};
//
//class EffekseerManager
//{
//public:
//	EffekseerBase*				effect[EFFECT_SIGUP_MAX];
//	EffekseerDate*				p_effectdata[EFFECT_TYPE_MAX];
//	::EffekseerRenderer::Renderer	*renderer;
//public:
//	EffekseerManager();
//	~EffekseerManager();
//	void Init();
//public:
//	void Render();
//	void Update(const Vector3& cpos, const Vector3& ctarget);
//public:
//	void SetMove(const Vector3& move, int effectNo);				//	指定した値に移動する
//	void SetEffect(WCHAR* filepath,int effectNo);						//	エフェクトの登録
//public:
//	void PlayEffect(
//		int effectType,
//		const Vector3& pos,
//		const Vector3& scale,
//		const Vector3& angle
//		);
//	void StopEffect(int effectNo);
//
//};
//
//
//#endif
//
