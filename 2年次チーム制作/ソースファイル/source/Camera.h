#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "iextreme.h"
#include "StateMachine\StateTemplate.h"
#include "CODBOForwardDecl.h"

//*********************************************************************
//	カメラ動きクラス
//*********************************************************************

typedef StateMachine<Camera, void> CAMERASTATEMACHINE, *LPCAMERASTATEMACHINE;

class CAMERASTATE:public State<Camera, void>
{
public:
	virtual ~CAMERASTATE(){}
	virtual void Enter(_client_type_ptr) = 0;
	virtual void Execute(_client_type_ptr) = 0;
	virtual void Exit(_client_type_ptr) = 0;
	virtual bool HandleMessage(LPGAMEMESSAGE msg){ return false; }
};

typedef CAMERASTATE* LPCAMERASTATE;

//*********************************************************************
//	カメラクラス
//*********************************************************************
class Camera
{
private:
	LPCAMERASTATEMACHINE	m_pStateMachine;
	Matrix					m_MatView;
	Matrix					m_Client_to_screen;	//ワールドをスクリーンに
	Matrix					m_Screen_to_client;	//スクリーンをワールドに
	DXQuaternion			m_shock_target_qua;
	DXQuaternion			m_shock_now_qua;
	float					m_shock_power;
	int						m_shock_start_count;
	int						m_shock_count;
protected:
	Vector3					m_pos, m_target,m_up;	//座標
public:
	Camera();
	~Camera();
private:
	void ShockAngleUpdate();
public:
	const Vector3&	GetPos()const;
	const Vector3&	GetTarget()const;
	void SetPos	(const Vector3& pos);
	void SetTarget(const Vector3& target);
	const Matrix& GetMatrix()const;
public:
	void SetShockAngle(const DXQuaternion& qua);
	void SetShock(float shock_power,int shock_count);
	void Update();
	void SetNewState(LPCAMERASTATE	pState);
	bool ClientToScreen(Vector3& out,CRVector3 client_pos);
	void ScreenToClient(Vector3& out, CRVector3 scr_pos);
	bool HandleMessage(LPGAMEMESSAGE msg);
};

//*********************************************************************
//	カメラの便利関数たち
//*********************************************************************
class CameraFunction :public Camera
{
private:
	CameraFunction();
	~CameraFunction();
public:
	enum RIGHTorLEFT
	{
		_RIGHT,
		_LEFT
	};
public:
	static Vector3 GetFront(LPCCAMERA pCam);	//カメラの前方向ベクトルを得る
	static Vector3 GetRight(LPCCAMERA pCam);	//カメラの右方向ベクトルを得る
	static Vector3 GetUp(LPCCAMERA pCam);		//カメラの上方向ベクトルを得る
public:
	static LPCAMERASTATE GetCharacterShuffleModeCamera(LPCHARACTER C, LPCAMERA Cam, RIGHTorLEFT pos);
	static LPCAMERASTATE GetCharacterUsualModeCamera(LPCHARACTER C, LPCAMERA Cam, RIGHTorLEFT pos );
	static LPCAMERASTATE GetCharacterAimModeCamera(LPCHARACTER C, LPCAMERA Cam, RIGHTorLEFT pos);
};

#endif