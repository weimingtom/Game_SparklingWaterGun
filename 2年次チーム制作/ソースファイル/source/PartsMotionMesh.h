#ifndef __PARTS_MOTION_MESH_H__
#define __PARTS_MOTION_MESH_H__

#include <list>
#include "iextreme.h"

class PartsMotionMesh : public iexMesh
{
protected:
	u8				version;
	u8				Param[16];

	u8*				Motion;			//	現在のモーション番号
	u16				M_Offset[256];	//	モーション先頭フレーム

	u32*			dwFrame;		//	現在のフレーム	(変更)
	u32				NumFrame;		//	総フレーム数
	u16*			dwFrameFlag;	//	フレーム情報

	LPIEXANIME2		lpAnime;		//	ボーンアニメーション

	//	頂点情報
	DWORD			NumVertex;
	void*			lpVertex;

	// スキンメッシュ関係
	LPD3DXSKININFO	lpSkinInfo;	// スキン情報

	u32				NumBone;
	LPWORD			BoneParent;
	Matrix*			lpBoneMatrix;
	Matrix*			lpOffsetMatrix;
	Matrix*			lpMatrix;

	Quaternion*		orgPose;
	Vector3*		orgPos;

	Quaternion*		CurPose;
	Vector3*		CurPos;

public:
	void	SetLoadFlag(BOOL bLoad){ this->bLoad = bLoad; }
	PartsMotionMesh(){
		bLoad = FALSE;
		for (int i = 0; i<16; i++) Param[i] = 0;
	}
	PartsMotionMesh(char* filename);
	~PartsMotionMesh();

	PartsMotionMesh*	Clone();

	BOOL LoadObject(char* filename);
	int LoadiEM(LPIEMFILE lpIem, LPSTR filename);
	BOOL CreateFromIEM(char* path, LPIEMFILE lpIem);

	LPD3DXSKININFO	CreateSkinInfo(LPIEMFILE lpIem);
	LPD3DXMESH	CreateMesh(LPIEMFILE lpIem);



	static BOOL SaveObject(LPIEMFILE lpIem, LPSTR filename);

	void Update();
	void SetMotion(int motion, std::list<int> bone_list);
	void SetMotion(int motion, int bone_num);
	inline int GetMotion(int bone_num){ return Motion[bone_num]; }
	inline WORD GetMotionOffset(int m){ return M_Offset[m]; }

	inline void SetFrame(int bone_num, int frame){ dwFrame[bone_num] = frame; }
	inline int GetFrame(int bone_num){ return dwFrame[bone_num]; }

	void Animation();

	void Render();
	void Render(DWORD flag, float alpha = -1);
	void Render(iexShader* shader, char* name);

	inline int GetParam(int n){ return Param[n]; }
	inline void SetParam(int n, int p){ Param[n] = p; }

	inline WORD GetFrameFlag(int frame){ return dwFrameFlag[frame]; }
	inline void SetFrameFlag(int frame, WORD p){ dwFrameFlag[frame] = p; }

	inline int GetNumFrame(){ return NumFrame; }

	inline Quaternion*	GetPose(int n){ return &CurPose[n]; }
	inline Vector3*		GetBonePos(int n){ return &CurPos[n]; }
	inline int	GetNumBone(){ return NumBone; }
	inline Matrix*	GetBone(int n){ return &lpBoneMatrix[n]; }

	void UpdateSkinMeshFrame();
	void UpdateBoneMatrix();
	void UpdateSkinMesh();
public:

	void DoubleMotion(int boneNum, int motion);
};

#endif