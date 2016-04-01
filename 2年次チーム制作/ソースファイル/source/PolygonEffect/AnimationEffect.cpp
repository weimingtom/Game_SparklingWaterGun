#include "AnimationEffect.h"
#include <algorithm>

void GetCameraZ(const Vector3& v, float& out_z)
{
	const Matrix& m = matView;
	out_z = v.x*m._13 + v.y*m._23 + v.z*m._33 + m._43;
}

AnimationEffectManager::AnimationEffectManager(int n):
EffectMax(n), 
SetIndex(0)
{
	Effects = new LPANIMEEFFECT[n];
	for (int i = 0; i < n; ++i)
	{
		Effects[i] = nullptr;
	}
	RenderEffects = new LPANIMEEFFECT[n];
	for (int i = 0; i < n; ++i)
	{
		RenderEffects[i] = nullptr;
	}
}

AnimationEffectManager::~AnimationEffectManager()
{
	for (int i = 0; i < EffectMax; ++i)
	{
		delete Effects[i];
	}
	delete[]Effects;
	delete[]RenderEffects;
	
}

AnimationEffectManager::INDEX AnimationEffectManager::SetEffect(LPANIMEEFFECT e)
{
	for (int i = 0; i < EffectMax; ++i)
	{
		if (Effects[SetIndex])
		{
			SetIndex = (SetIndex + 1) % EffectMax;
			continue;
		}
		Effects[SetIndex] = e;
		return SetIndex;
	}
	MyAssert(false,"エフェクトと弾の総数が最大に達している状態で、追加の要素がいれられようとしました。");

	return (INDEX)-1;
}

void AnimationEffectManager::EraceEffect(INDEX index)
{
	Effects[index] = nullptr;
}

int EffectZsort(const void* p1, const void* p2)
{
	if ((**(LPANIMEEFFECT*)p1).m_sort_z > (**(LPANIMEEFFECT*)p2).m_sort_z)
	{
		return -1;
	}
	return 1;
}

void AnimationEffectManager::ZSort(int &render_effect_count)
{

	ZeroMemory(RenderEffects, sizeof(LPANIMEEFFECT)*EffectMax);

	for (int i = 0; i < EffectMax; ++i)
	{
		if (Effects[i])
		{
			RenderEffects[render_effect_count] = Effects[i];
			RenderEffects[render_effect_count]->UpdateZ();

			++render_effect_count;
		}
	}

	std::qsort(
		&RenderEffects[0],
		render_effect_count,
		sizeof(LPANIMEEFFECT),
		EffectZsort
		);
}

void AnimationEffectManager::Update()
{
	for (int i = 0; i < EffectMax; ++i)
	{
		if (Effects[i] && !Effects[i]->Update())
		{
			delete Effects[i];
			Effects[i] = nullptr;
		}
	}
}

void AnimationEffectManager::Render()
{
	LVERTEX v[4];
	int render_effect = 0;

	ZSort(render_effect);

	LPDEVICE	lpDevice = iexSystem::Device;
	//	ワールド行列設定
	Matrix	mat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	lpDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&mat);

	lpDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < render_effect; ++i)
	{
		RenderEffects[i]->Render(v);
	}

	lpDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}