#include "CameraMove.h"
#include "All.h"
#include "textLoader.h"

const float RunAfterPlayerCamera::up_angle_max = 20 * (PI / 180.0f);
const float RunAfterPlayerCamera::down_angle_max = 120 * (PI / 180.0f);
static const float QuaExp = 0.001f;

RunAfterPlayerCamera::RunAfterPlayerCamera(
	_client_type_ptr p,
	float rotate_speed,
	const Vector3&		FollowPos,
	const Vector3&		adjust
	) :
	adjust(adjust),
	rotate_speed(rotate_speed),
	rotatePos(p->GetPos() - p->GetTarget()),
	upvec(0, 1, 0),
	FollowPos(FollowPos)
{
}

RunAfterPlayerCamera::~RunAfterPlayerCamera()
{
}

Vector3 RunAfterPlayerCamera::GetRightVec()
{
	//右方向を返す
	Vector3 r(matView._11, matView._21, matView._31);
	r.Normalize();
	return r;
}

void RunAfterPlayerCamera::CameraRotate()
{
	D3DXQUATERNION temp;	//計算用
	Vector3 stick_value(KEY(KEY_AXISX2,0)*0.001f, -KEY(KEY_AXISY2,0)*0.001f, 0);	//キー入力を入れておく
	const float stick_len = stick_value.Length();

	// 傾きの補正
	if (stick_len > 1)stick_value.Normalize();
	if (stick_len<0.3f){ return; }

	const Vector2 rotate_val(stick_value.x*rotate_speed, stick_value.y*rotate_speed);

	//軸と回転角度から回転クォータニオンを作る
	if (ABS(rotate_val.x) > QuaExp)
	{
		D3DXQuaternionRotationAxis(&temp, &Vector3Trans<Vector3, D3DXVECTOR3>(upvec), rotate_val.x);
		//上で作った回転するクォータニオンと現在の回転クォータニオンを合成する
		targetrotq *= temp;
	}

	if (ABS(rotate_val.y) > QuaExp)
	{
		D3DXQuaternionRotationAxis(&temp, &Vector3Trans<Vector3, D3DXVECTOR3>(GetRightVec()), rotate_val.y);
		targetrotq *= temp;
	}
}

void RunAfterPlayerCamera::CameraAdjust()
{
	D3DXQUATERNION tmpq;

	//現在のカメラ前方方向
	Vector3 cameraFront = -GetCameraQuaternionPos(targetrotq);
	Vector3 cameraRight = GetRightVec();

	//正規化
	cameraFront.Normalize();

	//内積から真上とカメラの前の逆の角度を得る
	float angle = acosf(Vector3Dot((Vector3)upvec, -cameraFront));

	//もしその角度が真上の角度制限の角度よりも小さかった場合
	if (angle < up_angle_max)
	{
		const float rotate_val = -(up_angle_max - angle);
		if (ABS(rotate_val)>QuaExp)
		{
			D3DXQuaternionRotationAxis(&tmpq, &Vector3Trans<Vector3, D3DXVECTOR3>(cameraRight), rotate_val);
			targetrotq *= tmpq;
		}
	}

	if (angle > down_angle_max)
	{
		const float rotate_val = angle - down_angle_max;
		if (ABS(rotate_val) > QuaExp)
		{
			D3DXQuaternionRotationAxis(&tmpq, &Vector3Trans<Vector3, D3DXVECTOR3>(cameraRight), (angle - down_angle_max));
			targetrotq *= tmpq;
		}
	}
}

Vector3 RunAfterPlayerCamera::GetCameraQuaternionPos(
	const D3DXQUATERNION& q)
{
	//クォータニオンでカメラのいちを回す関数

	D3DXQUATERNION rotate_pos_q(rotatePos.x, rotatePos.y, rotatePos.z, 0);	//計算の都合上クォータニオンに変換
	D3DXQUATERNION	conq;

	D3DXQuaternionConjugate(&conq, &q);	//qの共役クォータニオンの作成
	rotate_pos_q = conq*rotate_pos_q*q;	

	//回転後の位置を返す
	return Vector3(rotate_pos_q.x, rotate_pos_q.y, rotate_pos_q.z);
}


void RunAfterPlayerCamera::Enter(_client_type_ptr p)
{
	//初期化
	D3DXQuaternionIdentity(&rotq);
	D3DXQuaternionIdentity(&targetrotq);
}

#define NANCHECK(ctarget) if(isinf(ctarget.x) || isnan(ctarget.x) || isinf(ctarget.y) || isnan(ctarget.y) || isinf(ctarget.z) || isnan(ctarget.z))

void RunAfterPlayerCamera::Execute(_client_type_ptr p)
{
	Vector3 cpos, ctarget;

	//見る位置はターゲット(プレイヤー)
	ctarget = FollowPos;

	CameraRotate();	//キー入力によってクォータニオンを回転させる
	CameraAdjust();	//キー入力によってはみでた位置を補正する

	//現在のいちをクォータニオンから得る
	cpos = GetCameraQuaternionPos(rotq);

	//目標のクォータニオンと現在のクォータニオンを補完する
	D3DXQuaternionSlerp(&rotq, &rotq, &targetrotq, 0.1f);

	//足元を見ているので少し上に補正
	cpos += FollowPos + adjust;
	ctarget += adjust;

	//カメラの位置をセット
	p->SetPos(cpos);
	p->SetTarget(ctarget);
}

void RunAfterPlayerCamera::Exit(_client_type_ptr p)
{

}

//****************************************************************
//	何もしないカメラ
//****************************************************************

void FreezeCamera::Enter(_client_type_ptr p)
{

}
void FreezeCamera::Execute(_client_type_ptr p)
{

}
void FreezeCamera::Exit(_client_type_ptr p)
{

}
//****************************************************************
//	デバッグ用？移動カメラ
//****************************************************************

void FreeMoveCamera::Enter(_client_type_ptr p)
{

}
void FreeMoveCamera::Execute(_client_type_ptr p)
{
	Vector2 R(
		(float)KEY(KEY_AXISX2, 0)*0.001f, 
		(float)KEY(KEY_AXISY2, 0)*0.001f
		);
	Vector2 L(
		(float)KEY(KEY_AXISX, 0)*0.001f,
		(float)KEY(KEY_AXISY, 0)*-0.001f
		);

	Vector3 f, r, m;

	R *= 0.02f;
	L *= 0.15f;

	f = p->GetTarget() - p->GetPos();
	Vector3Cross(r, VECTOR3AXISY, f);

	f = Vector3RotateAxis(VECTOR3AXISY, R.x, f);
	f = Vector3RotateAxis(Vector3Normalize(r), R.y, f);

	m += Vector3Normalize(r)*L.x;
	m += Vector3Normalize(f)*L.y;

	if (isErrorVec(m) ||
		isErrorVec(f)
		)
	{
		int a = 0;
	}

	p->SetPos(p->GetPos() + m);
	p->SetTarget(p->GetPos() + f);

	Vector3 pos(p->GetPos());
	Vector3 t(p->GetTarget());

	if (KEY(KEY_A, 0) == 3)
	{
		int time = 180;
		if (KEY(KEY_R1, 0))time = 0;
		FILE* fp = fopen("DATA\\ScriptFile\\test.txt", "a");
		fprintf(fp, "P(%.2f,%.2f,%.2f)T(%.2f,%.2f,%.2f),M(%d) \n", pos.x, pos.y, pos.z, t.x, t.y, t.z, time);
		fclose(fp);
	}
}
void FreeMoveCamera::Exit(_client_type_ptr p)
{

}

//****************************************************************
//	プレイヤー通常状態でのカメラ
//****************************************************************

UsualPlayerMoveCamera::UsualPlayerMoveCamera(
	LPCAMERA		pCamera,
	LPCHARACTER		pFollowCharacter,		//追尾するキャラクタ
	float			rotate_speed,			//プレイヤーの向きについていくスピード
	float			local_pos_move_speed,	//ローカル位置の補間スピード
	CRVector3		local_pos,				//カメラのローカル位置
	CRVector3		local_target			//カメラのローカルターゲット
	):
	m_rotate_speed(rotate_speed),
	m_finish_rotate_position(local_pos),
	m_finish_rotate_target(local_target),
	m_pFollowCharacter(pFollowCharacter),
	m_local_pos_move_speed(local_pos_move_speed)
{
	//	初期化
	m_camera_pose = GetTargetQuaternion(m_pFollowCharacter);
	//初期(今までカメラがいた場所)を設定
	CRVector3 chr_pos = ControllObjFunc::GetPos(pFollowCharacter);

	m_rotate_position = Vector3RotateQuaternionInverse(m_camera_pose, pCamera->GetPos() - chr_pos);
	m_rotate_target = Vector3RotateQuaternionInverse(m_camera_pose, pCamera->GetTarget() - chr_pos);
}

UsualPlayerMoveCamera::~UsualPlayerMoveCamera()
{

}

bool UsualPlayerMoveCamera::isEquQuaternion(const DXQuaternion& q1, const DXQuaternion& q2)
{
	DXQuaternion v1(0, 0, 1, 0), v2(0, 0, 1, 0), conj;

	D3DXQuaternionConjugate(&conj, &q1);
	v1 = conj*v1*q1;

	D3DXQuaternionConjugate(&conj, &q2);
	v2 = conj*v1*q2;

	return D3DXVec3Dot((D3DXVECTOR3*)&v1, (D3DXVECTOR3*)&v2) > 0.0f;
}

DXQuaternion UsualPlayerMoveCamera::GetTargetQuaternion(
	LPCHARACTER pFollowCharacter)
{

	//プレイヤーが向いているベクトルに回転させるクォータニオンを作成する
	DXQuaternion target;
#if 0
	//回転目標ベクトル(視線の逆方向)
	Vector3 target_v = ChrFunc::GetFront(pFollowCharacter),axis;

	//回転軸を算出
	Vector3Cross(axis, Vector3(0,0,1), target_v);	
	axis.Normalize();

	const float angle = Vector3Radian(target_v, Vector3(0, 0, 1));

	//クォータニオンを作成
	if (angle > QuaExp&&!Vector3isZero(axis))
	{
		D3DXQuaternionRotationAxis(
			&target,
			(D3DXVECTOR3*)&axis,
			angle
			);
	}else{
		D3DXQuaternionIdentity(&target);
	}
	NANCHECK(target)
	{
		int a=0;
	}
#else
	//↑よりも回転行列から作成したほうが…いいかも
	Matrix m = ChrFunc::GetParam(pFollowCharacter).m_trans_mat;
	Vector3 v(m._11, m._12, m._13);

	v.Normalize();
	m._11 = v.x;
	m._12 = v.y;
	m._13 = v.z;

	v.x = m._21;
	v.y = m._22;
	v.z = m._23;
	v.Normalize();
	m._21 = v.x;
	m._22 = v.y;
	m._23 = v.z;

	v.x = m._31;
	v.y = m._32;
	v.z = m._33;
	v.Normalize();
	m._31 = v.x;
	m._32 = v.y;
	m._33 = v.z;

	m._14 = m._24 = m._34 = 0;
	m._41 = m._42 = m._43 = 0;
	m._44 = 1;

	D3DXQuaternionRotationMatrix(&target, &m);

	MyAssert(!isErrorVec(target), "悪夢再び");

#endif
	return target;
}

void UsualPlayerMoveCamera::Enter(_client_type_ptr p)
{

}

#define QEQU(a,b) ((a)>=(b)-QuaExp&&(a)<=(b)+QuaExp)

void UsualPlayerMoveCamera::Execute(_client_type_ptr p)
{
	//	実行
	const DXQuaternion q = m_camera_pose;
	DXQuaternion tmp = GetTargetQuaternion(m_pFollowCharacter);

	//if (QEQU(q.x, tmp.x) && QEQU(q.y, tmp.y) && QEQU(q.z, tmp.z) && QEQU(q.w, tmp.w))
	//if (isEquQuaternion(tmp,q))
	//{
	//	m_camera_pose = tmp;
	//}
	//if (true)
	{
		//補間
		D3DXQuaternionSlerp(
			&m_camera_pose,
			&q,
			&tmp,
			m_rotate_speed);
	}

	CRVector3 follow_chr_pos = ControllObjFunc::GetPos(m_pFollowCharacter);
	
	//キャラクタ空間で座標を補間
	m_rotate_position	+= (m_finish_rotate_position	- m_rotate_position)*m_local_pos_move_speed;
	m_rotate_target		+= (m_finish_rotate_target		- m_rotate_target)	*m_local_pos_move_speed;

	Vector3
		target_cpos		= Vector3RotateQuaternion(m_camera_pose, m_rotate_position) + follow_chr_pos,
		target_ctarget	= Vector3RotateQuaternion(m_camera_pose, m_rotate_target)	+ follow_chr_pos;

	Vector3 out, pos(target_ctarget), vec(target_cpos - target_ctarget);
	float dist = 1000000;

	if (MAP_MANAGER.RayPick(&out, &pos, &vec, &dist))
	{
		if (Vector3(out - target_ctarget).Length() < Vector3(target_cpos - target_ctarget).Length())
		{
			vec = target_ctarget - out;
			vec.Normalize();
			vec *= 0.15f;
			target_cpos = out;
		//	target_cpos += vec;
		}
	}

	//カメラのParameterとしてセット
	p->SetPos(target_cpos);
	p->SetTarget(target_ctarget);
}


void UsualPlayerMoveCamera::Exit(_client_type_ptr p)
{
	//	終了


}

//****************************************************************
//	リスポーン時カメラ
//****************************************************************


RespornCamera::RespornCamera(int time, LPCAMERASTATE pNext):
max_time(time),
now_time(0),
pNextState(pNext)
{

}

RespornCamera::~RespornCamera()
{
	if (pNextState)
	{
		delete pNextState;
	}
}

void RespornCamera::Enter(_client_type_ptr p)
{

}


void RespornCamera::Execute(_client_type_ptr p)
{

	if (max_time <= now_time++)
	{
		p->SetNewState(pNextState);
	}

	if (max_time - now_time < 40)return;

	Vector3 v = p->GetPos() - p->GetTarget();

	v.Normalize();
	v *= 0.001f;

	//カメラのParameterとしてセット
	p->SetPos(p->GetPos() + v);
}


void RespornCamera::Exit(_client_type_ptr p)
{
	//	終了
	pNextState = nullptr;
}

StartCamera::StartCamera(const char* posFile, bool* pIsFinish) :
m_pMove_data(nullptr),
m_MaxData(0),
m_NowIndex(0),
m_frame_count(0),
m_pre_pos(0,0,-1),
m_pre_target(0, 0, 0),
m_pIsFinish(pIsFinish)
{
	MyAssert(LoadData(posFile), "スタート時カメラの情報の読み込みに失敗しました。ファイル名　%s　", posFile);
}

StartCamera::~StartCamera()
{
	if (m_pMove_data)
	{
		delete[]m_pMove_data;
	}
}

bool StartCamera::LoadData(const char* posFile)
{
	textLoader fin((LPSTR)posFile);

	if (!fin.Be_Init())return false;

	m_MaxData = fin.LoadInt();

	if (m_MaxData <= 0)return false;

	m_pMove_data = new PositionData[m_MaxData];

	PositionData* target;

	for (int i = 0; i < m_MaxData; ++i)
	{
		target = &m_pMove_data[i];

		if (!fin.Search("P"))return false;
		
		target->pos.x = fin.LoadFloat();
		target->pos.y = fin.LoadFloat();
		target->pos.z = fin.LoadFloat();

		if (!fin.Search("T"))return false;

		target->target.x = fin.LoadFloat();
		target->target.y = fin.LoadFloat();
		target->target.z = fin.LoadFloat();

		if (!fin.Search("M"))return false;

		target->move_frame = fin.LoadInt();
	}
	return true;

}

void StartCamera::Enter(_client_type_ptr p)
{
	m_NowIndex = 0;
	m_frame_count = 0;
	m_pre_pos = p->GetPos();
	m_pre_target = p->GetTarget();
}

void StartCamera::Execute(_client_type_ptr p)
{
	if (m_NowIndex >= m_MaxData)
	{
		*m_pIsFinish = true;
		return;
	}


	PositionData* target = &m_pMove_data[m_NowIndex];
	RATIO r = (float)m_frame_count / (float)target->move_frame;

	if (r > 1)
	{
		m_frame_count = 0;

		do{
			++m_NowIndex;
		} 
		while (m_NowIndex < m_MaxData&&
			m_pMove_data[m_NowIndex].move_frame <= 0);
		
		m_pre_pos = m_pMove_data[m_NowIndex - 1].pos;
		m_pre_target = m_pMove_data[m_NowIndex - 1].target;

		return;
	}

	p->SetPos(fLERP(m_pre_pos, target->pos, r));
	p->SetTarget(fLERP(m_pre_target, target->target, r));

	++m_frame_count;
}

void StartCamera::Exit(_client_type_ptr p)
{

}

//****************************************************************
//	リスポーン選択時カメラ
//****************************************************************


RespornSelectCamera::RespornSelectCamera(
	CRVector3	TargetPos,
	CRVector3	TargetTarget,
	CRVector3	PrePos,
	CRVector3	PreTarget,
	float		speed,
	bool&		isMove
	):
	m_TargetPos(TargetPos),
	m_TargetTarget(TargetTarget),
	m_PrePos(PrePos),
	m_PreTarget(PreTarget),
	m_Speed(speed),
	isMove(isMove)
{
	
}

RespornSelectCamera::~RespornSelectCamera()
{

}

void RespornSelectCamera::Enter(_client_type_ptr p)
{

}

void RespornSelectCamera::Execute(_client_type_ptr p)
{
	D3DXQUATERNION rot;
	Vector3 pos, target, axis, center;
	Vector3 v1, v2;
	RADIAN angle;

	//時間経過
	m_T += m_Speed;

	if (m_T >= 1)
	{
		m_T = 1;
		isMove = false;
	}
	else{
		isMove = true;
	}

	if (Vector3(m_TargetPos - m_PrePos).Length() < 0.5f)
	{
		return;
	}

	//目標座標の計算

	//移動中心を算出
	center = (m_TargetPos - m_PrePos) / 2 + m_PrePos;

	v1 = m_PrePos - center;
	v2 = m_TargetPos - center;

	//回転軸の算出
	Vector3Cross(axis, v1, v2);
	axis.Normalize();

	angle = acosf(Vector3Dot(v1, v2) / (v1.Length()*v2.Length()));

	D3DXQuaternionRotationAxis(&rot, (D3DXVECTOR3*)&axis, angle);
	D3DXQuaternionSlerp(&rot, &D3DXQUATERNION(0, 0, 0, 1), &rot, m_T);

	//移動軸を回転
	v1 = Vector3RotateQuaternion(rot, v1);

	//セット
	v1.y *= 1.5f;

	if (v1.y < 0)v1.y = -v1.y;

	if(
		isErrorVec(v1) ||
		isErrorVec(center) ||
		isErrorVec(m_PreTarget) ||
		isErrorVec(m_TargetTarget)
		)
	{
		return;
	}

	p->SetPos(center + v1);
	p->SetTarget(m_PreTarget*(1 - m_T) + m_TargetTarget*m_T);
}

void RespornSelectCamera::Exit(_client_type_ptr p)
{

}
