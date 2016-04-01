//#ifndef __EFFECT_BASE_H__
//#define __EFFECTBASE_H__
//
///*
//Effekseer�̃��C�u�����[
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
//// �G�t�F�N�g�̓ǂݍ��ݐ� 
//#define EFFECT_READ_MAX 2000
//
////�G�t�F�N�g�̍ő�o����
//#define EFFECT_SIGUP_MAX	1000
//
//#define EFFECT_TYPE_MAX 10
//#define SAFE_RELEASE(x) if(x){(x)->Release();}
//
//// �G�t�F�N�g���f���^�C�v
//enum EFFECT_MODEL_NO
//{
//	EMN_SPHERE,				//	�X�t�B�A
//	EMN_TORNADO			//	����
//};
//
//// ���[�`���^�C�v
//enum EFFECT_ROUTINE_NO
//{
//	ERN_TIMER			//	���Ԃł̏���
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
//	bool Alive;	//	��������
//	int EffectNo;
//	int Time;
//public:
//	EffekseerBase();
//	~EffekseerBase();
//
//	void Update();
//	void Render();
//
//	void PlayEffect(												//	�G�t�F�N�g�Đ�
//		int type,
//		EffekseerDate* edata,
//		const Vector3& pos,
//		const Vector3& scale,
//		const Vector3& angle
//		);				
//	void StopEffect();												//	�G�t�F�N�g��~
//	void SetMove(const Vector3& move);				//	�w�肵���l�Ɉړ�����
//	void SetParam(												//	�p�����[�^�ݒ�
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
//	void SetMove(const Vector3& move, int effectNo);				//	�w�肵���l�Ɉړ�����
//	void SetEffect(WCHAR* filepath,int effectNo);						//	�G�t�F�N�g�̓o�^
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
