#pragma once

#include "iextreme.h"

//オブジェクトのデータ構造体
typedef struct ObjectData
{
	Vector3 pos;
	Vector3 angle;
	Vector3 scale;
	int type;            // オブジェクトのタイプ
	int mesh_type;  // メッシュのタイプ
}ObjectData, *LPObjectData;

typedef std::list<ObjectData> ObjectDataList;
class textLoader;

//*************************************************************
//オブジェクトロードクラス
//*************************************************************
class ObjectLoader
{
private:
	ObjectDataList      m_List;                 // 入出力に使用するデータ
	textLoader*          m_pTextLoader;   // テキストローダ
	LPSTR                   filename;              // ファイル名
	FILE*                    fp;                        // ファイルポインタ
public:
	ObjectLoader ( LPSTR filename );
	~ObjectLoader ();
public:
	ObjectDataList& GetList(){ return m_List; }
	// 読み込み
	bool Load();
	// 書き込み
	bool Save ( ObjectDataList& List );
	// オブジェクト単位で書き込み
	void SaveObject ( const ObjectData& data );
	// 読み込んだデータを出力
	void PrintLoadData ();
};



