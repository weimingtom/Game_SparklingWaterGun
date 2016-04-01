#include <list>
#include "ObjectLoader.h"
#include "../textLoader.h"
#include "MyFunction.h"
#include <stdio.h>

//*************************************************************
//オブジェクトロードクラス
//*************************************************************
ObjectLoader::ObjectLoader ( LPSTR file ) :
filename ( file ),
m_pTextLoader ( nullptr )
{
	m_pTextLoader = new textLoader ( filename );
}

ObjectLoader::~ObjectLoader ()
{
	safe_delete ( m_pTextLoader );
}

bool ObjectLoader::Load ()
{
	ObjectData work;

	// リストに読み込んだデータをプッシュしていく
	while ( m_pTextLoader->Search ( "Object" ) )
	{
		m_pTextLoader->In ();

		//pos
		m_pTextLoader->Search ( "pos" );
		work.pos.x = m_pTextLoader->LoadFloat ();
		work.pos.y = m_pTextLoader->LoadFloat ();
		work.pos.z = m_pTextLoader->LoadFloat ();
		//angle
		m_pTextLoader->Search ( "angle" );
		work.angle.x = m_pTextLoader->LoadFloat ();
		work.angle.y = m_pTextLoader->LoadFloat ();
		work.angle.z = m_pTextLoader->LoadFloat ();
		//scale
		m_pTextLoader->Search ( "scale" );
		work.scale.x = m_pTextLoader->LoadFloat ();
		work.scale.y = m_pTextLoader->LoadFloat ();
		work.scale.z = m_pTextLoader->LoadFloat ();
		//type
		m_pTextLoader->Search ( "type" );
		work.type = m_pTextLoader->LoadInt ();
		//mesh_type
		m_pTextLoader->Search ( "mesh_type" );
		work.mesh_type = m_pTextLoader->LoadInt ();

		m_pTextLoader->Out ();

		m_List.push_back ( work );
	}

	return true;
}

bool ObjectLoader::Save ( ObjectDataList& List )
{
	// リストにあるデータを書き込む
	fopen_s ( &fp, filename, "w" );
	for (auto& it : List)
	{
		SaveObject(it);
	}
	fclose(fp);
	return true;
}

void ObjectLoader::SaveObject ( const ObjectData& data )
{
	// オブジェクト単位で書き込んでいく
	fprintf_s ( fp, "Object {\n" );
	fprintf_s ( fp, "pos(%f,%f,%f)\n", data.pos.x, data.pos.y, data.pos.z );
	fprintf_s ( fp, "angle(%f,%f,%f)\n", data.angle.x, data.angle.y, data.angle.z );
	fprintf_s ( fp, "scale(%f,%f,%f)\n", data.scale.x, data.scale.y, data.scale.z );
	fprintf_s ( fp, "type %d\n", data.type );
	fprintf_s ( fp, "mesh_type %d\n", data.mesh_type );
	fprintf_s ( fp, "}\n" );
}

void ObjectLoader::PrintLoadData ()
{
	ObjectData data;
	// リストにあるデータを出力
	for (auto& it : m_List)
	{
		data = it;
		printf_s ("Object {\n" );
		printf_s ("pos(%f,%f,%f)\n", data.pos.x, data.pos.y, data.pos.z );
		printf_s ("angle(%f,%f,%f)\n", data.angle.x, data.angle.y, data.angle.z );
		printf_s ("scale(%f,%f,%f)\n", data.scale.x, data.scale.y, data.scale.z );
		printf_s ("type %d\n", data.type );
		printf_s ("mesh_type %d\n", data.mesh_type );
		printf_s ("}\n" );
	}
}