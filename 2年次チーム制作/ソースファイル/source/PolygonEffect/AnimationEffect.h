#ifndef __ANIMATIONEFFECT_H__
#define __ANIMATIONEFFECT_H__
#include    "iextreme.h"

//*********************************************************************
//		エフェクトクラスベース
//*********************************************************************

class AnimationEffect
{
public:
	float m_sort_z;
public:
	AnimationEffect() :m_sort_z(0){}

	virtual void UpdateZ() = 0;
	virtual ~AnimationEffect(){}
	virtual bool Update() = 0;
	virtual void Render(LPLVERTEX) = 0;
};
typedef AnimationEffect* LPANIMEEFFECT;

void GetCameraZ(const Vector3& pos, float& out_z);

//*********************************************************************
//		エフェクトクラスマネージャ
//*********************************************************************
class AnimationEffectManager
{
	typedef int INDEX;
private:
	const INDEX EffectMax;
	INDEX SetIndex;
	LPANIMEEFFECT* Effects;
	LPANIMEEFFECT* RenderEffects;
public:
	AnimationEffectManager(int n);
	~AnimationEffectManager();
public:
	INDEX SetEffect(LPANIMEEFFECT e);
	void EraceEffect(INDEX index);
	void ZSort(int &render_effect_count);
	void Update();
	void Render();
};

#endif