#ifndef __MAP_OBJECT_TEST_H__
#define __MAP_OBJECT_TEST_H__

//*************************************************************
//�I�u�W�F�N�g�Ǘ��w�b�_
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
// �I�u�W�F�N�g��{�N���X
//*************************************************************
class MapObjectInterface 
	:public PARENTANDCHILD, 
	public GameBaseEntity
{
protected:
	MAP_OBJ_TYPE				object_type_;                               // �I�u�W�F�N�g�̎��ʂɎg�p����
	ObjectParam                 parameters_;                                // 3D�p�����[�^
	Matrix                           difference_prev_frame_matrix_;   // �O�t���[���Ƃ̍����s��
	Matrix                           RT_matrix_;                                  // ��]�ƕ��s�ړ���������s��
	Matrix                           RT_matrix_inverse_;
	Matrix                           S_matrix_;                                    // �X�P�[�����O�s��
	Matrix                           SRT_matrix_;                                // SRT_matrix_ = S_matrix_ * RT_matrix_;
	Matrix                           SRT_matrix_inverse_;
	LPIEXMESH                  mesh_;                                          // ���b�V��
public:
	MapObjectInterface(
		ENTITY_ID               id,
		ObjectParam            param,
		LPIEXMESH              mesh,
		MAP_OBJ_TYPE       type
		);	
	virtual~MapObjectInterface();
public:
	// �����̎p����ς������Ƃ��͂����ł���
	virtual void RotateSelf(){}

	// �X�V�E�`��
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

	// �e�̉e�����󂯂�
	virtual void ParentMove(
		LPCMATRIX pParentWorldMat,
		LPCMATRIX pParentWorldMatInv,
		LPCMATRIX pParentMoveMat,
		LPCMATRIX AffineMat
		)override {};

	//���b�Z�[�W�󂯎��֐�
	virtual	bool HandleMessage(
		LPGameMessage msg) override
	{
		return false;
	};

	// �Q�b�^
	// �I�u�W�F�N�g�̈ړ��ʎ擾
	virtual Vector3 Get_MoveParameter()const{ return VECTOR3ZERO; }

	const ObjectParam& Get_Param()const{ return parameters_; }
	MAP_OBJ_TYPE Get_Type()const{ return object_type_; }
	const LPIEXMESH Get_Mesh(){ return mesh_; }
	Matrix& Get_Matrix(){ return SRT_matrix_; }
 };

typedef MapObjectInterface* LPMapObjectInterface;

//************************************************************
//	�}�b�v�I�u�W�F�N�g�}�l�[�W���N���X(Singleton)
//************************************************************
class MapObjectManager
{
public:
	typedef std::vector<LPMapObjectInterface> MAPOBJ_ARRAY;
	typedef std::vector<ShareRatioParam> SHAREPARAM_ARRAY;
private:
	static MapObjectManager* instance_;//�C���X�^���X
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
	const Vector3               stage_center_;                    // �X�e�[�W�̒��S
	lp_sp_field                    space_partitioning_field_;  // ��ԕ����̃t�B�[���h�N���X
	MeshManager*             mesh_manager_;               // ���b�V���Ǘ��N���X
	MAPOBJ_ARRAY           map_objects_;                   // �o�^����Ă���I�u�W�F�N�g�̔z��
	sp_field::OBJVECTOR   objects_in_range_;           // �w��͈͓��̃I�u�W�F�N�g
	SHAREPARAM_ARRAY  share_parameters_;          // �V�F�A�I�u�W�F�N�g�̃p�����[�^�z��
public:
	// �X�V�E�`��
	void Update();
	void UpdateShareValue();
	void Render();
	void Render(char* shaderName);
	void	Render_ShadowMap();

	//�I�u�W�F�N�g�쐬
	bool LoadObject(char* objfile, char* meshfile);
	// �I�u�W�F�N�g�ǉ�
	void AddObject(MapObjectInterface* obj);

	//�w��͈͓���RayPick
	MapObjectInterface* RayPick(
		Vector3* out,
		Vector3* pos,
		Vector3*vec,
		float* dist
		);

	// �Q�b�^
	float Get_FieldSize()const{ return 2000; }
	// type �ƈ�v����I�u�W�F�N�g���擾
	void Get_TypeObject(MAPOBJ_ARRAY& out, MAP_OBJ_TYPE type);
	// map_obj::_share_obj_begin ~ map_obj::_share_obj_end �̊Ԃ̃I�u�W�F�N�g���擾
	void Get_ShareTypeObject(MAPOBJ_ARRAY& out);
	void Get_ShareValue(SHAREPARAM_ARRAY& out);
	void Get_MeshManager(MeshManager** out);
};



#define MAP_MANAGER (MapObjectManager::getInstance())



#endif //__MAP_OBJECT_TEST_H__

// EOF