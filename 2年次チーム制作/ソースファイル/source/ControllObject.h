#ifndef __CONTROLL_OBJECT_H__
#define __CONTROLL_OBJECT_H__

#include "GameSystem\GameBase.h"
#include "Camera.h"
#include "iextreme.h"
#include "TeamViewData.h"

//ゲーム中の操作できる物の持っている基本的なパラメーター
typedef struct tagControllObjectParam
{
	int			team_type;
	Vector3		pos;	//座標
	bool		doexplode_blur;
	Vector2		explode_pos;
	float		blur_time;
	RATIO		blur_power;
	bool		do_water_drop;
}
ControllObjectParam, *LPCONTROLLOBJECTPARAM;

//ゲーム中の操作できる物の基底クラス
class ControllObject:public GameBaseEntity
{
protected:
	LPPLAYERMATCHDATA		m_pMyMatchData;	//自分のマッチデータへのポインタ
	ControllObjectParam		m_base_param;	//基本的なパラメーター
	Camera					m_camera;		//このオブジェクトを操作しているときのカメラ
	TeamData*				m_pTeamData;	//自分チームデータへのポインタ
public:
	ControllObject(ENTITY_ID id, int team_type, TeamData* tData);
	virtual~ControllObject(){}
public:
	virtual Vector3 GetBodyCenter() = 0;
	virtual void Controll() = 0;	//操作関数
	virtual void Update(TeamData& teamData) = 0;		//毎フレームの更新
//	virtual void Render() = 0;		//描画
	virtual void Render_ShadowMap(ControllObject& viewowner, TeamData& teamdata) = 0;
	virtual void Render(ControllObject& viewowner,TeamData& teamdata) = 0;		//描画
	virtual void RenderUI(ControllObject& viewowner, TeamData& teamdata) = 0;	//UIの描画
	virtual bool HandleMessage(LPGameMessage msg) = 0;	//メッセージ
};

#endif