#ifndef __MAP_WAYPOINT_OBJECT_H__
#define __MAP_WAYPOINT_OBJECT_H__

#include "ObjectLoader.h"
#include "MapObject.h"
#include <vector>
#include "MapObjectFactory.h"

//*************************************************************
// オブジェクトの動き用オブジェクトクラス
//*************************************************************
class WayPointObject 
	:public MapObjectInterface
{
	bool is_used_; // 現在使われているかどうか
public:
	WayPointObject(
		ENTITY_ID               id,
		ObjectParam            param,
		MAP_OBJ_TYPE       type
		);
	~WayPointObject(){}
public:
	// 更新・描画しない
	void Update()override{}
	void Render()override{}
	void Render(char* shaderName)override{}

	// レイ判定
	// あたらない
	bool RayPick(
		Vector3* out,
		Vector3* pos,
		Vector3*vec,
		float* dist
		)override
	{
		return false;
	}

	// 次の移動目標決定
	WayPointObject* GetNextWayPoint(); 

	void UsePoint(){ is_used_ = true; }
	void UnUsePoint(){ is_used_ = false; }

	bool IsUsed()const{ return is_used_; }
};

//*************************************************************
// WayPointクラス作成者
//*************************************************************
class WayPointFactory :public MapObjectFractory
{
	static int count_way_point;
public:
	static void Initcount_way_point();
	// 作成
	bool CreateNewObject(const ObjectData &data);
};

#endif // __MAP_WAYPOINT_OBJECT_H__

// EOF