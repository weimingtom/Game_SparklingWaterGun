#include "WayPointObject.h"




const MAP_OBJ_TYPE way_point_cource[map_obj::_way_point_obj_end - map_obj::_way_point_obj_begin + 1][2] =
{
	{ (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 1), (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 1) },//0Å®1
	{ (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 2), (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 4) },//1Å®2or4
	{ (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 3), (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 3) },//2Å®3
	{ (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 4), (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 4) },//3Å®4
	{ (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 5), (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 7) },//4Å®5or8
	{ (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 6), (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 6) },//5Å®6
	{ (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 7), (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 7) },//6Å®7
	{ (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 8), (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 8) },//7Å®8
	{ (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 0), (MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + 0) },//8Å®0
};


WayPointObject::WayPointObject(
	ENTITY_ID               id,
	ObjectParam            param,
	MAP_OBJ_TYPE       type
	) :
	MapObjectInterface(id, param, nullptr, type), 
	is_used_(false)
{

}

WayPointObject* WayPointObject::GetNextWayPoint()
{
	const MAP_OBJ_TYPE next_types[2] = {
		way_point_cource[object_type_ - map_obj::_way_point_obj_begin][0],
		way_point_cource[object_type_ - map_obj::_way_point_obj_begin][1]
	};

	MapObjectManager::MAPOBJ_ARRAY way_points;

	MAP_MANAGER.Get_TypeObject(way_points, next_types[0]);
	MAP_MANAGER.Get_TypeObject(way_points, next_types[1]);

	assert((way_points.empty() == false) && "WayPointéÊìæÇ…é∏îsÇµÇ‹ÇµÇΩÅB");

	WayPointObject* next_way_points[2];
	next_way_points[0] = (WayPointObject*)way_points.at(0);
	next_way_points[1] = (WayPointObject*)way_points.at(1);

	int random_next_type = rand() % 2;
	int another_next_type = ((random_next_type == 1) ? 0 : 1);

	if (next_way_points[random_next_type]->IsUsed() == false)
	{
		return next_way_points[random_next_type];
	}

	if (next_way_points[another_next_type]->IsUsed() == false)
	{
		return next_way_points[another_next_type];
	}

	return this;
}


int WayPointFactory::count_way_point = 0;

void WayPointFactory::Initcount_way_point()
{
	WayPointFactory::count_way_point = 0;
}

bool WayPointFactory::CreateNewObject(const ObjectData &data)
{
	// çÏê¨Ç…ïKóvÇ»Ç‡ÇÃÇópà”Ç∑ÇÈ

	ObjectParam parameter;
	parameter.pos = data.pos;
	parameter.angle = data.angle;
	parameter.scale = data.scale;

	ENTITY_ID id = 
		(ENTITY_ID)(game_id::id_map_object_min + count);

	// çÏê¨
	WayPointObject* way_point =
		new WayPointObject(
		id,
		parameter,
		(MAP_OBJ_TYPE)(map_obj::_way_point_obj_begin + count_way_point)
		);
	++count;
	++count_way_point;
	
	if (count_way_point == (int)map_obj::_way_point_obj_end)
	{
		Initcount_way_point();
	}

	// ìoò^
	MAP_MANAGER.AddObject(way_point);

	return true;
}

// EOF