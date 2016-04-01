#ifndef __MAP_OBJECT_FACTORY_H__
#define __MAP_OBJECT_FACTORY_H__

#include "MapObject.h"
#include "ObjectLoader.h"

//*************************************************************
// マップオブジェクト作成クラス
//*************************************************************
class MapObjectFractory
{
protected:
	static int count; // カウンタ (ENTITY_ID + 1)
public:
	static void InitCount();
	virtual~MapObjectFractory(){}

	virtual bool CreateNewObject(const ObjectData &data);
	virtual int GetCount()const{ return count; }
};

#endif // __MAP_OBJECT_FACTORY_H__

// EOF