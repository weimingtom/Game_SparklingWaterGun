#ifndef __MAP_OBJECT_TEST_H__
#define __MAP_OBJECT_TEST_H__

//*************************************************************
//オブジェクト管理ヘッダ
//*************************************************************


#include "iextreme.h"
#include "../ParentAndChild.h"
#include "../GameSystem/GameBase.h"
#include "../ppSpatialPartitionning.h"
#include <vector>
#include "MapObjectParam.h"

class MeshManager;


namespace map_obj
{
	enum _ENUM_MAP_OBJECT
	{
		_main_stage,
		
		_resporn_point_begin,

		_resporn_point_end = _resporn_point_begin + 1,
		_flag,
		
		_share_obj_begin,

		_share_obj_end = _share_obj_begin + 3,

		_way_point_obj_begin = 9,

		_way_point_obj_end = _way_point_obj_begin + 8, 

		_truck,

		_crane_body,
		_crane_steel_frame,

		_sky,
		_signboard,
		_elevator,
		_air_ship,

		_max,
	};
}

typedef map_obj::_ENUM_MAP_OBJECT MAP_OBJ_TYPE;

LPIEXMESH CreateDebugMesh(MAP_OBJ_TYPE type);

//*************************************************************
// オブジェクト基本クラス
//*************************************************************
class MapObjectInterface 
	:public PARENTANDCHILD, 
	public GameBaseEntity
{
protected:
	MAP_OBJ_TYPE				object_type_;                               // オブジェクトの識別に使用する
	ObjectParam                 parameters_;                                // 3Dパラメータ
	Matrix                           difference_prev_frame_matrix_;   // 前フレームとの差分行列
	Matrix                           RT_matrix_;                                  // 回転と平行移動をさせる行列
	Matrix                           RT_matrix_inverse_;
	Matrix                           S_matrix_;                                    // スケーリング行列
	Matrix                           SRT_matrix_;                                // SRT_matrix_ = S_matrix_ * RT_matrix_;
	Matrix                           SRT_matrix_inverse_;
	LPIEXMESH                  mesh_;                                          // メッシュ
public:
	MapObjectInterface(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);	
	virtual~MapObjectInterface();
public:
	// 自分の姿勢を変えたいときはここでする
	virtual void RotateSelf(){}

	// 更新・描画
	virtual void Update();
	virtual void Render();
	virtual void Render_ShadowMap();
	virtual void Render ( char* shaderName );

	//RayPick
	virtual bool RayPick(
		Vector3* out,
		Vector3* pos,
		Vector3*vec,
		float* dist
		);

	// 親の影響を受ける
	virtual void ParentMove(
		LPCMATRIX pParentWorldMat,
		LPCMATRIX pParentWorldMatInv,
		LPCMATRIX pParentMoveMat,
		LPCMATRIX AffineMat
		)override {};

	//メッセージ受け取り関数
	virtual	bool HandleMessage(
		LPGameMessage msg) override
	{
		return false;
	};

	// ゲッタ
	// オブジェクトの移動量取得
	virtual Vector3 Get_MoveParameter()const{ return VECTOR3ZERO; }

	const ObjectParam& Get_Param()const{ return parameters_; }
	MAP_OBJ_TYPE Get_Type()const{ return object_type_; }
	const LPIEXMESH Get_Mesh(){ return mesh_; }
	Matrix& Get_Matrix(){ return SRT_matrix_; }
 };

typedef MapObjectInterface* LPMapObjectInterface;

//************************************************************
//	マップオブジェクトマネージャクラス(Singleton)
//************************************************************
class MapObjectManager
{
public:
	typedef std::vector<LPMapObjectInterface> MAPOBJ_ARRAY;
	typedef std::vector<ShareRatioParam> SHAREPARAM_ARRAY;
private:
	static MapObjectManager* instance_;//インスタンス
private:
	MapObjectManager();
	MapObjectManager(const MapObjectManager& org) {};
	MapObjectManager& operator = (const MapObjectManager& org) { return *this; }
	~MapObjectManager();
public:
	static void CreateInstance();
	static MapObjectManager& getInstance();
	static void Release();
private:
	const Vector3               stage_center_;                    // ステージの中心
	lp_sp_field                    space_partitioning_field_;  // 空間分割のフィールドクラス
	MeshManager*             mesh_manager_;               // メッシュ管理クラス
	MAPOBJ_ARRAY           map_objects_;                   // 登録されているオブジェクトの配列
	sp_field::OBJVECTOR   objects_in_range_;           // 指定範囲内のオブジェクト
	SHAREPARAM_ARRAY  share_parameters_;          // シェアオブジェクトのパラメータ配列
public:
	// 更新・描画
	void Update();
	void UpdateShareValue();
	void Render();
	void Render(char* shaderName);
	void	Render_ShadowMap();

	//オブジェクト作成
	bool LoadObject(char* objfile, char* meshfile);
	// オブジェクト追加
	void AddObject(MapObjectInterface* obj);

	//指定範囲内のRayPick
	MapObjectInterface* RayPick(
		Vector3* out,
		Vector3* pos,
		Vector3*vec,
		float* dist
		);

	// ゲッタ
	float Get_FieldSize()const{ return 2000; }
	// type と一致するオブジェクトを取得
	void Get_TypeObject(MAPOBJ_ARRAY& out, MAP_OBJ_TYPE type);
	// map_obj::_share_obj_begin ~ map_obj::_share_obj_end の間のオブジェクトを取得
	void Get_ShareTypeObject(MAPOBJ_ARRAY& out);
	void Get_ShareValue(SHAREPARAM_ARRAY& out);
	void Get_MeshManager(MeshManager** out);
};



#define MAP_MANAGER (MapObjectManager::getInstance())



#endif //__MAP_OBJECT_TEST_H__

// EOF