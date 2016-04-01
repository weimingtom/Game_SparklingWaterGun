#include "Camera.h"
#include "CameraMove.h"

static const Vector3 CAMERA_UP(0, 1, 0);

Camera::Camera():
m_pos(0,2,-5),
m_target(0,0,0),
m_up(0,1,0),
m_shock_start_count(0),
m_shock_count(0),
m_shock_power(0)
{
	D3DXQuaternionIdentity(&m_shock_now_qua);
	D3DXQuaternionIdentity(&m_shock_target_qua);

	m_pStateMachine = new CAMERASTATEMACHINE(this);
}

Camera::~Camera()
{
	SAFE_DELETE(m_pStateMachine);
}

void Camera::SetShockAngle(const DXQuaternion& qua)
{
	D3DXVECTOR3 axis;
	float angle;

	m_shock_target_qua *= qua;

	D3DXQuaternionToAxisAngle(&m_shock_target_qua, &axis, &angle);
	angle *= PI;

	const RADIAN max_angle = PI_d4*0.30f;

	if (angle > max_angle)
	{
		D3DXQUATERNION temp;
		D3DXQuaternionRotationAxis(&temp, &axis, max_angle - angle);
		m_shock_target_qua *= temp;
	}

}

void Camera::SetShock(float shock_power, int shock_count)
{
	return;

	m_shock_power = shock_power;
	m_shock_count = shock_count;
	m_shock_start_count = shock_count;
}

void Camera::Update()
{
	SAFE_EXE(m_pStateMachine, state_execute);

	ShockAngleUpdate();

	D3DXMatrixLookAtLH(&m_MatView, (D3DXVECTOR3*)&m_pos, (D3DXVECTOR3*)&m_target, (D3DXVECTOR3*)&VECTOR3AXISY);
	m_Client_to_screen = m_MatView*matProjection;
	D3DXMatrixInverse(&m_Screen_to_client, 0, &m_Client_to_screen);
	
	m_shock_count = max(m_shock_count - 1, 0);

	D3DXQuaternionSlerp(&m_shock_now_qua, &m_shock_now_qua, &m_shock_target_qua, 0.4f);
	D3DXQuaternionSlerp(&m_shock_target_qua, &m_shock_target_qua, &DXQuaternion(0, 0, 0, 1), 0.05f);

	if (m_shock_start_count > 0)
	{
		const Vector3 rv = Vector3Rand()* ((float)m_shock_count / (float)m_shock_start_count)*m_shock_power;
		m_pos += rv;
		m_target += rv;
	}
}


void Camera::SetNewState(LPCAMERASTATE	pState)
{
	m_pStateMachine->set_state(pState);
}

bool Camera::ClientToScreen(Vector3& out,CRVector3 client_pos)
{
	float z = client_pos.x*m_MatView._13 + client_pos.y*m_MatView._23 + client_pos.z*m_MatView._33 + m_MatView._43;
	out= Vector3MulMatrixDivW(client_pos, m_Client_to_screen);
	out.z = z;

	return (
		z > 0 &&
		out.x > -1 &&
		out.y > -1 &&
		out.x < 1 &&
		out.y < 1 
		);
}

void Camera::ScreenToClient(Vector3& out, CRVector3 scr_pos)
{
	out = Vector3MulMatrixDivW(scr_pos, m_Screen_to_client);
}

bool Camera::HandleMessage(LPGAMEMESSAGE msg)
{
	return ((LPCAMERASTATE)m_pStateMachine->GetState())->HandleMessage(msg);
}

void Camera::ShockAngleUpdate()
{
	Vector3 v = m_pos - m_target;
	Vector3 pre_v = v;

	v = Vector3RotateQuaternion(m_shock_now_qua, v);
	m_pos = m_target + v;

	pre_v -= v;
	pre_v.Normalize();
	
	m_up.y = cosf(Vector3XZLength(pre_v));
}

const Vector3& Camera::GetPos()const
{
	return m_pos;
}

const Vector3& Camera::GetTarget()const
{
	return m_target;
}

void Camera::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

void Camera::SetTarget(const Vector3& target)
{
	m_target = target;
}

const Matrix& Camera::GetMatrix()const
{
	return m_MatView;
}

//*******************************************************
//	functions
//*******************************************************
#define TOCONTROLL(cam) (*(CameraFunction*)cam)

Vector3 CameraFunction::GetFront(LPCCAMERA pCam)
{
	return TOCONTROLL(pCam).m_target - TOCONTROLL(pCam).m_pos;
}

Vector3 CameraFunction::GetRight(LPCCAMERA pCam)
{
	Vector3 ret;
	Vector3Cross(ret, CAMERA_UP, GetFront(pCam));
	return ret;
}

Vector3 CameraFunction::GetUp(LPCCAMERA pCam)
{
	Vector3 r, f = GetFront(pCam), ret;

	Vector3Cross(r, CAMERA_UP,f);
	Vector3Cross(ret, f, r);

	return ret;
}

LPCAMERASTATE CameraFunction::GetCharacterShuffleModeCamera(LPCHARACTER C, LPCAMERA Cam, RIGHTorLEFT pos)
{
	Vector3 position(0.25f, 0.85f, -2.6f);
	Vector3 target = position;
	target.z = 0.0f;

	if (pos == _LEFT)
	{
		position.x = -position.x;
		target.x = -target.x;
	}
	return new UsualPlayerMoveCamera(Cam, C, 0.15f, 0.05f, position, target);
}

LPCAMERASTATE CameraFunction::GetCharacterUsualModeCamera(LPCHARACTER C, LPCAMERA Cam, RIGHTorLEFT pos)
{
	Vector3 position(0.32f, 1.2f, -3.3f);
	Vector3 target = position;
	target.z = 0.0f;

	if (pos == _LEFT)
	{
		position.x = -position.x;
		target.x = -target.x;
	}
	return new UsualPlayerMoveCamera(Cam, C, 0.15f, 0.2f, position, target);
}

LPCAMERASTATE CameraFunction::GetCharacterAimModeCamera(LPCHARACTER C, LPCAMERA Cam, RIGHTorLEFT pos)
{
	Vector3
		//position(0.45f, 1.0f, -0.8f*2),
		position(0.32f, 1.2f, -1.9f);
	Vector3
		target(position + Vector3(0, 0, 1));

	if (pos == _LEFT)
	{
		position.x = -position.x;
		target.x = -target.x;
	}

	return new UsualPlayerMoveCamera(Cam, C, 0.15f, 0.2f, position, target);
}
