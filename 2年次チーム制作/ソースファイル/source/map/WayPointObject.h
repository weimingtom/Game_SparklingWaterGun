#ifndef __MAP_WAYPOINT_OBJECT_H__
#define __MAP_WAYPOINT_OBJECT_H__

#include "ObjectLoader.h"
#include "MapObject.h"
#include <vector>
#include "MapObjectFactory.h"

//*************************************************************
// �I�u�W�F�N�g�̓����p�I�u�W�F�N�g�N���X
//*************************************************************
class WayPointObject 
	:public MapObjectInterface
{
	bool is_used_; // ���ݎg���Ă��邩�ǂ���
public:
	WayPointObject(
		ENTITY_ID               id,
		ObjectParam            param,
		MAP_OBJ_TYPE       type
		);
	~WayPointObject(){}
public:
	// �X�V�E�`�悵�Ȃ�
	void Update()override{}
	void Render()override{}
	void Render(char* shaderName)override{}

	// ���C����
	// ������Ȃ�
	bool RayPick(
		Vector3* out,
		Vector3* pos,
		Vector3*vec,
		float* dist
		)override
	{
		return false;
	}

	// ���̈ړ��ڕW����
	WayPointObject* GetNextWayPoint(); 

	void UsePoint(){ is_used_ = true; }
	void UnUsePoint(){ is_used_ = false; }

	bool IsUsed()const{ return is_used_; }
};

//*************************************************************
// WayPoint�N���X�쐬��
//*************************************************************
class WayPointFactory :public MapObjectFractory
{
	static int count_way_point;
public:
	static void Initcount_way_point();
	// �쐬
	bool CreateNewObject(const ObjectData &data);
};

#endif // __MAP_WAYPOINT_OBJECT_H__

// EOF