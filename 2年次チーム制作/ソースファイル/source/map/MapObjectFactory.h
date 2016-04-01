#ifndef __MAP_OBJECT_FACTORY_H__
#define __MAP_OBJECT_FACTORY_H__

#include "MapObject.h"
#include "ObjectLoader.h"

//*************************************************************
// �}�b�v�I�u�W�F�N�g�쐬�N���X
//*************************************************************
class MapObjectFractory
{
protected:
	static int count; // �J�E���^ (ENTITY_ID + 1)
public:
	static void InitCount();
	virtual~MapObjectFractory(){}

	virtual bool CreateNewObject(const ObjectData &data);
	virtual int GetCount()const{ return count; }
};

#endif // __MAP_OBJECT_FACTORY_H__

// EOF