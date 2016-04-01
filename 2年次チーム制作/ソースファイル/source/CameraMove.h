#ifndef __CAMERA_MOVE_H__
#define __CAMERA_MOVE_H__

#include "iextreme.h"
#include "Camera.h"
#include "Character.h"

//****************************************************************
//	指定した場所でスティックで視点を自由に回せるカメラ
//****************************************************************
class  RunAfterPlayerCamera :public CAMERASTATE
{
private:
	static const float up_angle_max, down_angle_max;
	const float				rotate_speed;				//カメラ回転スピード
	D3DXQUATERNION			rotq, targetrotq;			//現在の回転クォータニオン　と　回転目標クォータニオン
	const Vector3			rotatePos, upvec, adjust;	//回転させる初期位置 と　Y回転軸
	const Vector3&			FollowPos;
public:
	RunAfterPlayerCamera(
		_client_type_ptr p,
		float rotate_speed,
		const Vector3&		FollowPos,
		const Vector3&		adjust);
	~RunAfterPlayerCamera();
private:
	//引数のクォータニオンによって回転した後のカメラの位置を得る
	Vector3 GetCameraQuaternionPos(const D3DXQUATERNION& q);
	//カメラの右方向ベクトル
	Vector3 GetRightVec();
	//カメラのキー回転をする
	void CameraRotate();
	//カメラの位置を補正する
	void CameraAdjust();
public:
	void Enter(_client_type_ptr p);			//	初期化
	void Execute(_client_type_ptr p);		//	実行
	void Exit(_client_type_ptr p);			//	終了
};

//****************************************************************
//	何もしないカメラ
//****************************************************************
class FreezeCamera :public CAMERASTATE
{
public:
	void Enter(_client_type_ptr p);			//	初期化
	void Execute(_client_type_ptr p);	//	実行
	void Exit(_client_type_ptr p);			//	終了
};

//****************************************************************
//	デバッグ用？移動カメラ
//****************************************************************
class FreeMoveCamera :public CAMERASTATE
{
private:

public:
	void Enter(_client_type_ptr p);			//	初期化
	void Execute(_client_type_ptr p);	//	実行
	void Exit(_client_type_ptr p);			//	終了
};

//****************************************************************
//	プレイヤー通常状態でのカメラ
//****************************************************************
class UsualPlayerMoveCamera :public CAMERASTATE
{
private:
	LPCHARACTER		m_pFollowCharacter;			//追尾するキャラクタ
	DXQuaternion	m_camera_pose;				//カメラ方向クォータニオン
	const float		m_rotate_speed;				//プレイヤーについていくスピード
	const float		m_local_pos_move_speed;		//ローカル位置の補間スピード
	const Vector3	m_finish_rotate_position;	//回転させる初期座標
	const Vector3	m_finish_rotate_target;		//回転させるターゲット位置
	Vector3			m_rotate_position;			//現在のローカル位置の座標
	Vector3			m_rotate_target;			//現在のローカル位置のターゲット
public:
	UsualPlayerMoveCamera(
		LPCAMERA	pCamera,				//カメラへのポインタ
		LPCHARACTER	pFollowCharacter,		//追尾するキャラクタ
		float		m_local_pos_move_speed,	//ローカル位置の補間スピード
		float		rotate_speed,			//プレイヤーについていくスピード
		CRVector3	local_pos,				//カメラのローカル位置
		CRVector3	local_target			//カメラのローカルターゲット
		);
	~UsualPlayerMoveCamera();
private:
	bool isEquQuaternion(const DXQuaternion& q1, const DXQuaternion& q2);
	DXQuaternion GetTargetQuaternion(LPCHARACTER pFollowCharacter);
public:
	void Enter(_client_type_ptr p);			//	初期化
	void Execute(_client_type_ptr p);	//	実行
	void Exit(_client_type_ptr p);			//	終了
};


//****************************************************************
//	リスポーン時カメラ
//****************************************************************
class RespornCamera :public CAMERASTATE
{
private:
	LPCAMERASTATE		pNextState;
	const int			max_time;
	int					now_time;
public:
	RespornCamera(int time, LPCAMERASTATE pNext);
	~RespornCamera();
public:
	void Enter(_client_type_ptr p);			//	初期化
	void Execute(_client_type_ptr p);	//	実行
	void Exit(_client_type_ptr p);			//	終了
};

//****************************************************************
//	スタート時カメラ
//****************************************************************
class StartCamera :public CAMERASTATE
{
private:
	typedef struct 
	{
		Vector3 pos, target;
		int		move_frame;

	}PositionData;

	Vector3			m_pre_pos, m_pre_target;
	PositionData*	m_pMove_data;
	int				m_MaxData;
	int				m_NowIndex;
	int				m_frame_count;
	bool*			m_pIsFinish;
public:
	StartCamera(const char* posFile, bool* pIsFinish);
	~StartCamera();
private:
	bool LoadData(const char* posFile);
public:
	void Enter(_client_type_ptr p);			//	初期化
	void Execute(_client_type_ptr p);	//	実行
	void Exit(_client_type_ptr p);			//	終了
};

//****************************************************************
//	リスポーン選択時カメラ
//****************************************************************
class RespornSelectCamera :public CAMERASTATE
{
private:
	const Vector3 m_TargetPos, m_TargetTarget;
	const Vector3 m_PrePos, m_PreTarget;
	const RATIO m_Speed;
	bool& isMove;
	RATIO m_T;
public:
	RespornSelectCamera(
		CRVector3	TargetPos,
		CRVector3	TargetTarget,
		CRVector3	PrePos,
		CRVector3	PreTarget,
		float		speed,
		bool&		isMove
		);
	~RespornSelectCamera();
public:
	void Enter(_client_type_ptr p);			//	初期化
	void Execute(_client_type_ptr p);	//	実行
	void Exit(_client_type_ptr p);			//	終了
};

#endif