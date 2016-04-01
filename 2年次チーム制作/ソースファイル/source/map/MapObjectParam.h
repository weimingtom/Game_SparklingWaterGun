#ifndef __MAP_OBJECT_PARAM_H__
#define __MAP_OBJECT_PARAM_H__

#include "iextreme.h"
#include "../CODBOForwardDecl.h"

// マップオブジェクトの3Dパラメータ
typedef struct ObjectParam
{
	Vector3 pos;
	Vector3 angle;
	Vector3 scale;
}
OBJECTPARAM, *LPOBJECTPARAM;

// シェアオブジェクトのパラメータ
struct ShareRatioParam
{
	struct Render
	{
		COLORf		NowColor;
		COLORf		DamageTeamColor;
	};
	RATIO            share_power;              //このオブジェクトの影響力
	float               share_value;               //現在の代わり度
	float               max_share_value;      //最大代わり度
	TEAM_TYPE   team_type;                 //チームタイプ
	Render           render_param;
	int                  NotDamageCount;
};

#endif // __MAP_OBJECT_PARAM_H__

// EOF