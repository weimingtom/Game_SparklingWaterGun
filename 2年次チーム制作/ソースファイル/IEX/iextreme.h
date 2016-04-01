#ifndef __IEXTREME_H__
#define __IEXTREME_H__

#define _CRT_SECURE_NO_WARNINGS
#define	_CRT_SECURE_NO_DEPRECATE

//*****************************************************************************************************************************
//
//	IEX Version2010
//
//*****************************************************************************************************************************
#include	<windows.h>
#include	<d3dx9.h>
#include	<dsound.h>

#define		DIRECTINPUT_VERSION	0x0800 
#include	<dinput.h>
#include	<cassert>
#include	"../source/ItDebug.h"
#include	"../source/ColorUtility.h"

//*****************************************************************************
//
//		変数型定義
//
//*****************************************************************************

#define	null	NULL

#define fLERP(a,b,t) ((a)*(1.0f-(t))+((b)*(t)))
#define fRAND(Min,Max) (((float)rand()/RAND_MAX)*((Max)-(Min))+(Min))

#define	s8		signed char
#define	s16		short
#define	s32		int

#define	u8		unsigned char
#define	u16		unsigned short
#define	u32		unsigned long

#define PS3_CONTRLLER	1
#define PS2_CONTRLLER	2

#define USE_CONTROLLER_TYPE PS3_CONTRLLER

typedef D3DXVECTOR4		Vector4;
typedef D3DXMATRIX		Matrix;
typedef const Matrix&	CRMatrix;

typedef IDirect3DTexture9 Texture2D;
typedef IDirect3DSurface9 Surface;
typedef IDirect3DDevice9 DEVICE, *LPDEVICE;

//*****************************************************************************
//
//*****************************************************************************

#define RELEASE(obj) if( obj ){ (obj)->Release(); (obj) = null; }

//ポインターdeleteしてnullに
#define SAFE_DELETE(ptr) if(ptr){ delete (ptr);	ptr=nullptr; }

//ポインターの有無を確認して実行…これはやりすぎわら
#define SAFE_EXE(ptr,function,...) ((bool)(ptr != NULL)?(ptr->function(__VA_ARGS__)) : false)

#define ARRAY_SET(_array,_max,val)for(int i=0;i<_max;++i){_array[i]=val;}

#define isErrorValuef(f) (isnan(f)||isinf(f))
#define isErrorVec(v) (isErrorValuef((v).x)||isErrorValuef((v).y)||isErrorValuef((v).z))

class iex2DObj;
class iexShader;

//*****************************************************************************************************************************
//
//		i-Extremeシステム
//
//*****************************************************************************************************************************
//------------------------------------------------------
//		スクリーンモード
//------------------------------------------------------
#define	SCREEN640	0
#define	SCREEN800	1
#define	SCREEN1024	2

#define	SCREEN720p	11
#define	SCREEN800p	12

class iexSystem {
private:
	static LPDIRECT3D9				lpD3D;
	static D3DPRESENT_PARAMETERS	d3dpp;

public:
	//------------------------------------------------------
	//	グローバルパラメータ
	//------------------------------------------------------
	//	スクリーンパラメータ
	static DWORD ScreenWidth;		//	スクリーン幅
	static DWORD ScreenHeight;		//	スクリーン高
	static D3DFORMAT ScreenFormat;	//	スクリーンフォーマット

	//	３Ｄデバイス
	static LPDEVICE	Device;			//	デバイス
	static inline LPDEVICE GetDevice(){ return Device; };

	//
	static HWND	Window;				//	ウィンドウハンドル
	static void GetScreenRect( DWORD mode, RECT& rc );

	//------------------------------------------------------
	//	初期化・解放
	//------------------------------------------------------
	static BOOL Initialize( HWND hWnd, BOOL bFullScreen, DWORD ScreenMode );
	static void Release()
	{
		RELEASE( Device );
		RELEASE( lpD3D );
	}

	//------------------------------------------------------
	//	シーン開始・終了
	//------------------------------------------------------
	static void BeginScene(){ Device->BeginScene(); }
	static void EndScene()
	{
		Device->EndScene();
		if( FAILED( Device->Present( NULL, NULL, NULL, NULL ) )) Device->Reset(&d3dpp);
	}

	//------------------------------------------------------
	//	デバッグウィンドウ
	//------------------------------------------------------
	static void OpenDebugWindow();
	static void CloseDebugWindow();
	static void printf( const char* format, ...);

};

extern	Matrix	matView;		//	カメラ行列
extern	Matrix	matProjection;	//	投影変換行列


//int型矩形情報
typedef struct
{
	int x, y, width, height;
}
RECTI, *LPRECTI;

//*****************************************************************************************************************************
//
//		数学関連
//
//*****************************************************************************************************************************

#define	PI		((FLOAT)  3.141592654f)

#define PI_d2	((FLOAT)  PI/2)	//90度
#define PI_d4	((FLOAT)  PI/4)	//45度

#define PI_x2	((FLOAT)  PI*2)	//360度

typedef float RATE;		//0～１の間
typedef float RATIO;	//↑はまちがっていました…w
typedef float RADIAN;	//ラジアン

//float==比較
#define FLOATEQU(a,b) ((a)>=(b)-FLT_EPSILON&&(a)<=(b)+FLT_EPSILON)
//絶対値
#define ABS(x) ((x)>=0?(x):-(x))

//------------------------------------------
//	２次元ベクトルクラス
//------------------------------------------
class  Vector2
{
public:
	float x, y;
public:
	Vector2(){}
	inline Vector2(float x, float y) : x(x), y(y)	{}
public:
	inline Vector2 operator +()const{ return Vector2(x, y); }
	inline Vector2 operator -()const{ return Vector2(-x, -y); }

	inline Vector2 operator +(const Vector2& v)	const{ return Vector2(x + v.x, y + v.y); }
	inline Vector2 operator -(const Vector2& v)	const{ return Vector2(x - v.x, y - v.y); }
	inline Vector2 operator *(float v)			const{ return Vector2(x * v, y * v); }
	inline Vector2 operator /(float v)			const{ return Vector2(x / v, y / v); }

	inline Vector2&operator +=(const Vector2& v){ x += v.x; y += v.y; return *this; }
	inline Vector2&operator -=(const Vector2& v){ x -= v.x; y -= v.y; return *this; }
	inline Vector2&operator *=(float v)			{ x *= v; y *= v; return *this; }
	inline Vector2&operator /=(float v)			{ x /= v; y /= v; return *this; }

	inline Vector2& operator = (const Vector2& v){ x = v.x; y = v.y; return *this; }

	inline bool operator ==(const Vector2 v)const{ return ((x == v.x) && (y == v.y)); }
	inline bool operator !=(const Vector2 v)const{ return ((x != v.x) || (y != v.y)); }
};

static const Vector2 VECTOR2ZERO(0, 0);

typedef const Vector2&		CRVector2;

//------------------------------------------------------
//	長さを計算
//------------------------------------------------------
inline float	Vector2Length(const Vector2 & v){ return sqrtf(v.x*v.x + v.y*v.y); }
inline float	Vector2LengthSq(const Vector2 & v){ return (v.x*v.x + v.y*v.y); }

//------------------------------------------------------
//	正規化
//------------------------------------------------------
inline Vector2	Vector2Normalize(const Vector2 & v)
{
	float len = sqrtf(v.x*v.x + v.y*v.y);
	if (len != 0)return Vector2(v.x / len, v.y / len);
	return Vector2(0, 0);
}

//------------------------------------------------------
//	内積
//------------------------------------------------------
inline float Vector2Dot(const Vector2 & v1, const Vector2 & v2)
{
	return (v1.x*v2.x + v1.y*v2.y);
}

//------------------------------------------------------
//	成分
//------------------------------------------------------
inline float* Vector2GetComponent(Vector2 & v, int ComponentNumber)
{
	return (float*)((BYTE*)(&v) + sizeof(float)*ComponentNumber);
}

//*****************************************************************************
//		３Ｄベクトル
//*****************************************************************************
//------------------------------------------------------
//	３Ｄベクトル基本構造体
//------------------------------------------------------
typedef struct Vector {
	float	x, y, z;
} Vector;

//------------------------------------------------------
//	３Ｄベクトル構造体
//------------------------------------------------------
typedef struct Vector3 : public Vector
{
public:
	//	コンストラクタ
	Vector3() {};
	inline Vector3( float x, float y, float z ){ this->x=x, this->y=y, this->z=z; } 
	inline Vector3( CONST Vector& v ){ this->x=v.x, this->y=v.y, this->z=v.z; } 

	//	距離計算
	inline float Length()const{ return sqrtf(x*x + y*y + z*z); }
	inline float LengthSq()const{ return x*x + y*y + z*z; }

	//	正規化
	void Normalize()
	{
		float l = Length();
		if( l != .0f ){ x /= l; y /= l; z /= l; }
	}

	//	オペレーター
	inline Vector3& operator = ( CONST Vector& v ){ x=v.x; y=v.y; z=v.z; return *this; } 
	inline Vector3& operator += ( CONST Vector3& v ){ x+=v.x; y+=v.y; z+=v.z; return *this; } 
	inline Vector3& operator -= ( CONST Vector3& v ){ x-=v.x; y-=v.y; z-=v.z; return *this; }
	inline Vector3& operator *= ( FLOAT v ){ x*=v; y*=v; z*=v; return *this; }
	inline Vector3& operator /= ( FLOAT v ){ x/=v; y/=v; z/=v; return *this; }

	inline Vector3 operator + () const { Vector3 ret( x, y, z ); return ret; }
	inline Vector3 operator - () const { Vector3 ret( -x, -y, -z ); return ret; }

	inline Vector3 operator + ( CONST Vector3& v ) const { return Vector3(x+v.x, y+v.y, z+v.z); }
	inline Vector3 operator - ( CONST Vector3& v ) const { return Vector3(x-v.x, y-v.y, z-v.z); }
	inline Vector3 operator * ( FLOAT v ) const { Vector3 ret(x*v, y*v, z*v); return ret; }
	inline Vector3 operator / ( FLOAT v ) const { Vector3 ret(x/v, y/v, z/v); return ret; }

	BOOL operator == ( CONST Vector3& v ) const { return (x==v.x) && (y==v.y) && (z==v.z); }
	BOOL operator != ( CONST Vector3& v ) const { return (x!=v.x) || (y!=v.y) || (z!=v.z); }

} Vector3, *LPVECTOR3;

typedef const Vector3*	LPCVECTOR3;
typedef const Vector3&  CRVector3;	//const Vector3& よく使うので


//------------------------------------------------------
//	演算マクロ
//------------------------------------------------------

#define VECTOR3TOZERO(out)\
{\
	out.x = 0;\
	out.y = 0;\
	out.z = 0;\
}

//足し算
#define VECTOR3PLUS(out,v1,v2)\
{\
	(out).x = (v1).x + (v2).x; \
	(out).y = (v1).y + (v2).y; \
	(out).z = (v1).z + (v2).z; \
}

//引き算
#define VECTOR3MINUS(out,v1,v2)\
{\
	(out).x = (v1).x - (v2).x; \
	(out).y = (v1).y - (v2).y; \
	(out).z = (v1).z - (v2).z; \
}

//掛け算
#define VECTOR3MULSCALAR(out,v,s)\
{\
	(out).x = (v).x *(s); \
	(out).y = (v).y *(s); \
	(out).z = (v).z *(s); \
}

//割り算
#define VECTOR3DIVSCALAR(out,v,s)\
{\
	(out).x = (v).x / (s); \
	(out).y = (v).y / (s); \
	(out).z = (v).z / (s); \
}

//長さ
#define VECTOR3LENGTH(v)\
	sqrtf((v).x*(v).x + (v).y*(v).y + (v).z*(v).z) 


//２点間の距離
#define VECTOR3POINTTOPOINT(out_length,p1,p2,tempv)\
{\
	VECTOR3MINUS(tempv, p1, p2); \
	out_length=VECTOR3LENGTH(tempv); \
}

#define VECTOR3NORMALIZE(v)\
{\
	float l = sqrtf((v).x*(v).x + (v).y*(v).y + (v).z*(v).z); \
if (l){\
	v.x /= l, v.y /= l, v.z /= l;\
}\
}

#define VECTOR3CROSS(out,v1,v2)\
{\
	out.x = v1.y*v2.z - v1.z*v2.y;\
	out.y = v1.z*v2.x - v1.x*v2.z;\
	out.z = v1.x*v2.y - v1.y*v2.x;\
}

#define VECTOR3DOT(v1,v2) (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z)
//------------------------------------------------------
//	長さ
//------------------------------------------------------
inline float Vector3Length(const Vector& v)
{
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

inline float Vector3LengthSq(const Vector& v)
{
	return (v.x*v.x + v.y*v.y + v.z*v.z);
}

inline float Vector3XYLength(const Vector& v)
{
	return sqrtf(v.x*v.x + v.y*v.y);
}

inline float Vector3XZLength(const Vector& v)
{
	return sqrtf(v.x*v.x + v.z*v.z);
}

inline bool Vector3isZero(const Vector& v)
{
	return (v.x == 0.0f&&v.y == 0.0f&&v.z == 0.0f);
}


//------------------------------------------------------
//	外積
//------------------------------------------------------
inline void Vector3Cross(Vector& out, const Vector& v1, const Vector& v2)
{
	out.x = v1.y*v2.z - v1.z*v2.y;
	out.y = v1.z*v2.x - v1.x*v2.z;
	out.z = v1.x*v2.y - v1.y*v2.x;
}

//------------------------------------------------------
//	内積
//------------------------------------------------------
inline float Vector3Dot(const Vector& v1, const Vector& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

//------------------------------------------------------
//	2ベクトル間の角度を得る
//------------------------------------------------------
inline RADIAN Vector3Radian(const Vector& v1, const Vector& v2)
{
	return acosf(Vector3Dot(v1, v2) / (Vector3Length(v1)*Vector3Length(v2)));
}


//------------------------------------------------------
//	乗算
//------------------------------------------------------
inline Vector3 Vector3MulMatrix(const Vector& v, const Matrix& m)
{
	return Vector3(
		v.x*m._11 + v.y*m._21 + v.z*m._31 + m._41,
		v.x*m._12 + v.y*m._22 + v.z*m._32 + m._42,
		v.x*m._13 + v.y*m._23 + v.z*m._33 + m._43
		);
}

inline Vector3 Vector3MulMatrix3x3(const Vector& v, const Matrix& m)
{
	return Vector3(
		v.x*m._11 + v.y*m._21 + v.z*m._31,
		v.x*m._12 + v.y*m._22 + v.z*m._32,
		v.x*m._13 + v.y*m._23 + v.z*m._33
		);
}

inline Vector3 Vector3MulMatrixDivW(const Vector& v, const Matrix& m)
{
	Vector3 ret(
		v.x*m._11 + v.y*m._21 + v.z*m._31 + m._41,
		v.x*m._12 + v.y*m._22 + v.z*m._32 + m._42,
		v.x*m._13 + v.y*m._23 + v.z*m._33 + m._43
		);

	float w = v.x*m._14 + v.y*m._24 + v.z*m._34 + m._44;
	ret.x /= w;
	ret.y /= w;
	ret.z /= w;
	return ret;
}

//------------------------------------------------------
//	正規化
//------------------------------------------------------
inline Vector3 Vector3Normalize(const Vector& v)
{
	float l = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
	if (l != .0f)
	{
		return Vector3(v.x / l, v.y / l, v.z / l);
	}
	return Vector3(0, 0, 0);
}

inline Vector3 Vector3Rand()
{
	Vector3 ret(((float)rand() / RAND_MAX) - 0.5f, ((float)rand() / RAND_MAX) - 0.5f, ((float)rand() / RAND_MAX) - 0.5f);
	VECTOR3NORMALIZE(ret);
	return ret;
}


//------------------------------------------------------
//	回転
//------------------------------------------------------
inline Vector3 Vector3RotateQuaternion(const D3DXQUATERNION& q, const Vector& rotate_vector)
{

	D3DXQUATERNION inv, vq(rotate_vector.x, rotate_vector.y, rotate_vector.z, 0);
	D3DXQuaternionConjugate(&inv, &q);
	vq = inv*vq*q;

	return Vector3(vq.x, vq.y, vq.z);
}

inline Vector3 Vector3RotateQuaternionInverse(const D3DXQUATERNION& q, const Vector& rotate_vector)
{

	D3DXQUATERNION inv, vq(rotate_vector.x, rotate_vector.y, rotate_vector.z, 0);
	D3DXQuaternionConjugate(&inv, &q);
	vq = q*vq*inv;

	return Vector3(vq.x, vq.y, vq.z);
}

inline Vector3 Vector3RotateAxis(const Vector& axis,float angle,const Vector& rotate_vector)
{
	D3DXQUATERNION q;
	if (ABS(angle) < 0.01f)return rotate_vector;
	D3DXQuaternionRotationAxis(&q, &D3DXVECTOR3(axis.x, axis.y, axis.z), angle);
	return Vector3RotateQuaternion(q, rotate_vector);
}

inline Vector3 Vector3Reflect(const Vector3& v, const Vector3& normal, float coefficient)
{
	Vector3 ret = normal;
	VECTOR3NORMALIZE(ret);
	ret = v + normal *-Vector3Dot(ret, v)*(coefficient + 1);
	return ret;
}

//------------------------------------------------------
//	成分
//------------------------------------------------------
inline float* Vector3GetComponent(Vector& v, int ComponentNumber)
{
	return (float*)((BYTE*)(&v) + sizeof(float)*ComponentNumber);
}

//------------------------------------------------------
//	変換
//------------------------------------------------------
template<class from,class to>
inline to Vector3Trans(const from& v)
{
	to ret;
	ret.x = v.x;
	ret.y = v.y;
	ret.z = v.z;
	return ret;
}

//------------------------------------------------------
//	各軸に平行なベクトル
//------------------------------------------------------
static const Vector3
VECTOR3AXISX(1, 0, 0),	//Ｘ
VECTOR3AXISY(0, 1, 0),	//Ｙ
VECTOR3AXISZ(0, 0, 1),	//Ｚ
VECTOR3ZERO(0, 0, 0);

//*****************************************************************************
//		行列
//*****************************************************************************

//		変換行列
void	SetTransformMatrixZXY( Matrix *Mat, float x, float y, float z, float ax, float ay, float az );
void	SetTransformMatrixXYZ( Matrix *Mat, float x, float y, float z, float ax, float ay, float az );
//		ビュー変換表列
void	LookAtLH( Matrix& mat, Vector& Eye, Vector& At, Vector& Up );
//		投影変換表列
void	PerspectiveLH( Matrix& mat, float fovY, float aspect, float zn, float zf );
void	OlthoLH( Matrix& mat, float w, float h, float zn, float zf );

void	MulMatrix3x3(Matrix&out,const Matrix* m1,const Matrix* m2);

//*****************************************************************************
//		クォータニオン
//*****************************************************************************
typedef struct  Quaternion
{
public:
	//------------------------------------------------------
	//	パラメータ
	//------------------------------------------------------
	float	x, y, z, w;

	//------------------------------------------------------
	//	コンストラクタ
	//------------------------------------------------------
	Quaternion(){}
	Quaternion( float sx, float sy, float sz, float sw ) : x(sx), y(sy), z(sz), w(sw) {}
	
	//------------------------------------------------------
	//	生成
	//------------------------------------------------------
	//	単位クォータニオン生成
	void Identity(){ x = y = z = 0; w = 1; }

	//	正規化
	inline void normalize()
	{
		float legnth = getLength();
		if(legnth == .0f ) return;
		float invL = 1.0f / legnth;
		(*this) *= invL;
	}

	//------------------------------------------------------
	//	情報取得
	//------------------------------------------------------
	inline float getLength() const{ return sqrtf( x*x + y*y + z*z + w*w); }
 	void toMatrix( Matrix& m );

	//------------------------------------------------------
	//	オペレーター
	//------------------------------------------------------
	inline Quaternion operator + () const { Quaternion ret( x, y, z, w ); return ret; }
	inline Quaternion operator - () const { Quaternion ret( -x, -y, -z, -w ); return ret; }

	//	VS クォータニオン
	inline Quaternion& operator +=(const Quaternion& v){ x+=v.x; y+=v.y; z+=v.z; w+=v.w; return *this; }
	inline Quaternion& operator -=(const Quaternion& v){ x-=v.x; y-=v.y; z-=v.z; w-=v.w; return *this; }
	inline Quaternion& operator *=(const Quaternion& v)
	{
		x = y * v.z - z * v.y + x * v.w + w * v.x;
		y = z * v.x - x * v.z + y * v.w + w * v.y;
		z = x * v.y - y * v.x + z * v.w + w * v.z;
		w = w * v.w - x * v.x - y * v.y - z * v.z;
		return *this;
    }

	inline Quaternion operator +(const Quaternion& v) const{ return Quaternion( x+v.x, y+v.y, z+v.z, w+v.w); }
	inline Quaternion operator -(const Quaternion& v) const{ return Quaternion( x-v.x, y-v.y, z-v.z, w-v.w); }
	inline Quaternion operator *(const Quaternion& v) const
	{
		return Quaternion(
			y * v.z - z * v.y + x * v.w + w * v.x,
			z * v.x - x * v.z + y * v.w + w * v.y,
			x * v.y - y * v.x + z * v.w + w * v.z,
			w * v.w - x * v.x - y * v.y - z * v.z
		);
    }

	//	VS 値
	inline Quaternion& operator *=(float v){ x*=v; y*=v; z*=v; w*=v; return *this; }
	inline Quaternion& operator /=(float v){ x/=v; y/=v; z/=v; w/=v; return *this; }

	inline Quaternion operator *( float v ) const{ return Quaternion( x*v, y*v, z*v, w*v); }
	inline Quaternion operator /( float v ) const{ return Quaternion( x/v, y/v, z/v, w/v); }

} Quaternion;

//------------------------------------------------------
//	球面線形補間
//------------------------------------------------------
Quaternion QuaternionSlerp(Quaternion& q, Quaternion& r, float a);

//------------------------------------------------------
//	変換
//------------------------------------------------------
template<class from, class to>
inline to QuaternionTrans(const from& q)
{
	to ret;
	ret.x = v.x;
	ret.y = v.y;
	ret.z = v.z;
	ret.w = v.w;
	return ret;
}

typedef D3DXQUATERNION DXQuaternion, *LPDXQuaternion;

//*****************************************************************************************************************************
//
//		３Ｄ基本設定
//
//*****************************************************************************************************************************
//*****************************************************************************
//
//		レンダーステート設定
//
//*****************************************************************************
class iexRenderState
{
private:

public:
	static void Initialize();
	static void Set( DWORD state, D3DMATERIAL9* lpMaterial, Texture2D* lpTexture );
	//	フィルタON・OFF
	static void	Filter( BOOL bFilter );
};

//*****************************************************************************
//
//		ライト関連
//
//*****************************************************************************
class iexLight
{
private:
public:
	static void SetAmbient( DWORD ambient );
	static void SetFog( float Param1, float Param2, DWORD Color );

	static void DirLight( iexShader* shader, int index, Vector3* dir, float r, float g, float b );
	static void DirLight( int index, Vector3* dir, float r, float g, float b );
	static void PointLight( int index, Vector3* Pos, float r, float g, float b, float range );
};

//*****************************************************************************
//
//		視界関連
//
//*****************************************************************************
class iexView {
private:
	Matrix	matView;
	Matrix	matProj;

	//	視点パラメータ
	Vector Pos;
	Vector Target;

	//	視界パラメータ
	D3DVIEWPORT9 Viewport;

	//	投影パラメータ
	float FovY;
	float Near, Far;
	float Aspect;

public:
	iexView()
	{
		SetViewport();
		SetProjection( D3DX_PI / 4.0f, 0.2f, 190.0f );
	}
	void GetViewParameters(float& out_near, float& out_far, float& out_fov)
	{
		out_near = Near;
		out_far = Far;
		out_fov = FovY;
	}

	//	視界クリア
	void Clear( DWORD color=0, bool bClearZ=true );
	void ClearZ();

	//	視点設定
	void Set( Vector& pos, Vector& target );
	void Set( float x, float y, float z, float ax, float ay, float az );
	
	void SetPos( float x, float y, float z );
	void SetAngle( float x, float y, float z );

	//	投影平面設定
	void SetViewport();
	void SetViewport( int x, int y, int w, int h );

	//	投影設定
	void SetProjection( float FovY, float Near, float Far );
	void SetProjection( float FovY, float Near, float Far, float asp );

	void Activate();
};

typedef iexView	*LPIEXVIEW;

//*****************************************************************************************************************************
//
//		テクスチャ関連
//
//*****************************************************************************************************************************
class iexTexture {
private:
	static const int MAX_TEXTURE = 512;

	struct TEXINFO {
		char		filename[64];	//	ファイルパス
		int			UseCount;		//	使用カウント
		Texture2D*	lpTexture;		//	テクスチャ
	};
	static struct TEXINFO TexInfo[MAX_TEXTURE];

public:
	//	初期化
	static void Initialize();
	//	読み込み・解放
	static Texture2D* Load( char* filename, int flag = 0 );
	static void	Release( Texture2D* lpTexture );

};

extern	Texture2D*	lpLastTexture;

//*****************************************************************************
//
//		転送フラグ
//
//*****************************************************************************

#define	RM_NORMAL	0
#define	RM_ADD		1
#define	RM_SUB		2
#define	RM_MUL		4
#define	RM_NEGA		6
#define	RM_INVERT	30

#define	RS_COPY		RM_NORMAL
#define	RS_LCOPY	10
#define	RS_ADD		RM_ADD
#define	RS_SUB		RM_SUB
#define	RS_MUL		RM_MUL
#define	RS_NEGA		RM_NEGA
#define	RS_INVERT	RM_INVERT

#define	RS_SHADER	0xFE
#define	RS_NONE		RM_NONE

//*****************************************************************************
//
//		カスタム頂点
//
//*****************************************************************************

//	２Ｄ用頂点（ライティング＆トランスフォーム済み）
#define D3DFVF_TLVERTEX		( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
typedef struct tagTLVERTEX {
	float	sx, sy, sz;
	float	rhw;
	COLOR	color;
	float	tu, tv;
} TLVERTEX, *LPTLVERTEX;

//	２Ｄ用頂点（ライティング＆トランスフォーム済み、色なし）
#define D3DFVF_TLVERTEX2	( D3DFVF_XYZRHW | D3DFVF_TEX1 )
typedef struct tagTLVERTEX2 {
	float	sx, sy, sz;
	float	rhw;
	float	tu, tv;
} TLVERTEX2, *LPTLVERTEX2;

//	３Ｄ用頂点(法線なし）
#define D3DFVF_VERTEX		( D3DFVF_XYZ | D3DFVF_TEX1 )
typedef struct tagVERTEX {
	float	x, y, z;
	float	tu, tv;
} VERTEX, *LPVERTEX;

//	３Ｄ用頂点(ライティング済み）
#define D3DFVF_LVERTEX		( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
typedef struct tagLVERTEX {
	union {
		struct{
			float	x, y, z;
		};
		Vector _v;
	};
	COLOR	color;
	float	tu, tv;
} LVERTEX, *LPLVERTEX;

//	３Ｄメッシュ用頂点
#define D3DFVF_MESHVERTEX		( D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1 )
typedef struct tagMESHVERTEX {
	float	x, y, z;
	float	nx, ny, nz;
	float	tu, tv;
} MESHVERTEX, *LPMESHVERTEX;

//	３Ｄ用頂点(ライティング済み）
#define D3DFVF_MESHVERTEX2		( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
typedef struct tagLNVERTEX {
	float	x, y, z;
	float	nx, ny, nz;
	COLOR	color;
	float	tu, tv;
} MESHVERTEX2, *LPMESHVERTEX2;


//*****************************************************************************************************************************
//
//		２Ｄオブジェクト関連
//
//*****************************************************************************************************************************

//------------------------------------------------------
//		作成フラグ
//------------------------------------------------------
#define	IEX2D_RENDERTARGET	1
#define	IEX2D_USEALPHA		2
#define	IEX2D_SYSTEMMEM		100
#define	IEX2D_FLOAT			111
#define	IEX2D_FLOAT2		110

struct TextureRect
{
	int left;
	int top;
	int width;
	int height;
};

typedef TextureRect*		LPTextureRect;
typedef const TextureRect&	CRTextureRect;

//描画パラメーター
struct TextureRenderRect
{
	TextureRect render_rect;
	TextureRect read_tex_rect;
};

typedef const TextureRenderRect& CRTextureRenderRect;

//------------------------------------------------------
//		２Ｄオブジェクト
//------------------------------------------------------
class iex2DObj {
private:
protected:
	u32			dwFlags;		//	フラグ					


	Surface*	lpSurface;
	Texture2D*	lpTexture;		//	テクスチャ

	u32			width;			//	幅
	u32			height;			//	高さ
public:

	//------------------------------------------------------
	//	作成・解放
	//------------------------------------------------------
	iex2DObj(){};
	//	ファイルから作成
	iex2DObj(char* filename);
	//	作成
	iex2DObj(u32 width, u32 height, u8 flag);
	//	解放
	~iex2DObj()
	{
		if (lpSurface) lpSurface->Release();
		if (lpTexture) iexTexture::Release(lpTexture);
	}

	//------------------------------------------------------
	//	設定・取得
	//------------------------------------------------------
	void RenderTarget(int index = 0);
	Texture2D*	GetTexture(){ return lpTexture; }
	inline u32 GetWidth()const{ return width; }
	inline u32 GetHeight()const{ return height; }
	//------------------------------------------------------
	//	描画
	//------------------------------------------------------
	void Render();
	void Render(iexShader* shader, char* tech);
	void Render(s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, u32 dwFlags = RS_COPY, COLOR color = 0xFFFFFFFF, float z = .0f);
	void Render(s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, iexShader* shader, char* tech, COLOR color = 0xFFFFFFFF, float z = .0f);
	void Render(CRTextureRenderRect render_rect, u32 dwFlags = RS_COPY, COLOR color = 0xFFFFFFFF, float z = .0f);
	

	void Render(
		const Vector2& screen_left_up_pos,
		const Vector2& screen_render_size,
		const Vector2& texture_left_up_pos,
		const Vector2& texture_render_size,
		DWORD	dwFlags = RS_COPY,
		COLOR color = 0xFFFFFFFF,
		float z = 0
		);


	//回転付き描画
	void RenderPlus(
		const Vector2& screen_center_pos,	//描画する中心位置
		const Vector2& screen_render_size,	//描画する大きさ
		float angle,						//回転角度
		const Vector2& texture_left_up_pos,	//画像上の左上の座標
		const Vector2& texture_render_size,	//左上からどれだけ読み込むか
		DWORD	dwFlags=RS_COPY,			//描画方法
		COLOR color=0xFFFFFFFF,				//頂点カラー
		float z=0							//頂点ｚ値
		);
};


//	２Ｄオブジェクト
typedef iex2DObj IEX2DOBJ, *LPIEX2DOBJ;

//*****************************************************************************************************************************
//
//		ポリゴン関連
//
//*****************************************************************************************************************************

class iexPolygon {
public:
	static void Render3D( LPLVERTEX lpVertex, int Num, LPIEX2DOBJ lpObj, u32 dwFlags );
	static void Render3D( LPLVERTEX lpVertex, int Num, LPIEX2DOBJ lpObj, iexShader* shader, char* name );
	static void Render2D( LPTLVERTEX lpVertex, int Num, LPIEX2DOBJ lpObj, u32 dwFlags );
	static void Render2D( LPTLVERTEX lpVertex, int Num, LPIEX2DOBJ lpObj, iexShader* shader, char* name );
	static void Rect( s32 DstX, s32 DstY, s32 DstW, s32 DstH, u32 dwFlags, COLOR color, float z=.0f );
	static void Rect( s32 DstX, s32 DstY, s32 DstW, s32 DstH, iexShader* shader, char* name, COLOR color, float z=.0f );

};

//*****************************************************************************************************************************
//
//		シェーダー関連
//
//*****************************************************************************************************************************
class	iexShader {
private:
	LPD3DXEFFECT    m_pShader;			// シェーダー
	
	D3DXHANDLE		m_hmWVP;			// 射影変換行列
	D3DXHANDLE		m_htexDecale;		// テクスチャ

protected:
public:
	//------------------------------------------------------
	//	初期化・解放
	//------------------------------------------------------
	iexShader( char* filename );
	~iexShader();

	//------------------------------------------------------
	//	利用開始・終了
	//------------------------------------------------------
	inline UINT Begine( char* name )
	{
		UINT pass;
		//	シェーダーの適用
		m_pShader->SetTechnique( name );
		m_pShader->Begin( &pass, 0 );
		return pass;
	}
	inline void End(){ m_pShader->End(); }

	//------------------------------------------------------
	//	パスの開始・終了
	//------------------------------------------------------
	inline void BeginePass( int pass ){ m_pShader->BeginPass( pass ); }
	inline void EndPass(){ m_pShader->EndPass(); }
	inline void CommitChanges(){ m_pShader->CommitChanges(); }

	//------------------------------------------------------
	//	パラメータ設定
	//------------------------------------------------------
	void SetTexture( Texture2D* tex );
	void SetTexture( Texture2D& tex );

	void SetMatrix( Matrix* mat );
	void SetMatrix( Matrix& mat );

	void SetValue( char* name, iex2DObj* obj );
	void SetValue( char* name, iex2DObj& obj );

	void SetValue( char* name, Texture2D* tex );
	void SetValue( char* name, Texture2D& tex );

	void SetValue( char* name, Matrix* mat );
	void SetValue( char* name, Matrix& mat );

	void SetValue( char* name, D3DXVECTOR4* v );
	void SetValue( char* name, D3DXVECTOR4& v );

	void SetValue( char* name, Vector3* v );
	void SetValue( char* name, Vector3& v );

	void SetValue( char* name, float f );
	void SetValue( char* name, int d );
	void SetValue( char* name, DWORD d );
};

//*****************************************************************************************************************************
//
//		メッシュ関連
//
//*****************************************************************************************************************************
class iexMesh {
private:
protected:
	u32				dwFlags;		//	フラグ
	u8				bChanged;		//	変更フラグ

	Vector3			Pos;			//	メッシュ座標
	Vector3			Angle;			//	メッシュ回転量
	Vector3			Scale;			//	メッシュスケール

	D3DMATERIAL9	*lpMaterial;	//	材質
	Texture2D*		*lpTexture;		//	テクスチャ
	Texture2D*		*lpNormal;		//	法線テクスチャ
	Texture2D*		*lpSpecular;	//	スペキュラテクスチャ
	Texture2D*		*lpHeight;		//	高さテクスチャ
	Texture2D*		*lpLight;
	u32				MaterialCount;	//	材質数

	LPD3DXMESH		lpMesh;			//	メッシュ

public:
	BOOL			bLoad;
	Matrix			TransMatrix;	//	転送行列

	//------------------------------------------------------
	//	初期化
	//------------------------------------------------------
	iexMesh( char* filename );
	iexMesh(){
		lpMesh = NULL;
		lpTexture = NULL;
		lpNormal = NULL;
		lpSpecular = NULL;
		lpHeight = NULL;
		lpLight = NULL; 
		bLoad = FALSE;
	}
	iexMesh*	Clone();
	~iexMesh();

	//------------------------------------------------------
	//	読み込み
	//------------------------------------------------------
	BOOL LoadIMO( char* filename );
	BOOL LoadX( char* filename );

	//------------------------------------------------------
	//	更新
	//------------------------------------------------------
	void Update();

	//------------------------------------------------------
	//	描画
	//------------------------------------------------------
virtual	void Render();
virtual	void Render( iexShader* shader, char* name );
virtual	void Render(iexShader* shader, char* name, Matrix& trans);
	void Render(u32 dwFlags, float a, float r, float g, float b);
	void Render(u32 dwFlags, float param = -1);

	//------------------------------------------------------
	//	レイ判定
	//------------------------------------------------------
	int	RayPick( Vector3* out, Vector3* pos, Vector3* vec, float *Dist );
	int	RayPickUD( Vector3* out, Vector3* pos, Vector3* vec, float *Dist );

	//Rayワールド変換レイピック(スケールとか回転とかしててもちゃんとあたる)
	int RayPickPlus(Vector3* out, Vector3* pos, Vector3* vec, float *Dist);

	//Rayワールド変換レイピック(引数でもってくるバージョン)
	int RayPickPlus(Vector3* out, Vector3* pos, Vector3* vec, float *dist, const Matrix& trans_matrix, const Matrix& inv_matrix);

	//曲線とのあたり判定
	int CurveRayPick(Vector3* out, Vector3* pos,Vector3* vec, CRVector3 g, float *Dist);

	//円とメッシュのめり込み判定
	bool HitCheckXZArea(CRVector3 pos, float size, Vector3& out_pos);
	bool HitCheckXZAreaPlus(CRVector3 pos, float size, Vector3& out_pos, const Matrix& trans_matrix, const Matrix& inv_matrix);

	//------------------------------------------------------
	//	情報設定・取得
	//------------------------------------------------------
	//	位置
	void SetPos( const Vector3& pos );
	void SetPos( float x, float y, float z );
	inline Vector3	GetPos(){ return Pos; }
	//	角度
	void SetAngle( Vector3& angle );
	void SetAngle( float angle );
	void SetAngle( float x, float y, float z );
	inline Vector3	GetAngle(){ return Angle; }
	//	スケール
	void SetScale( Vector3& scale );
	void SetScale( float scale );
	void SetScale( float x, float y, float z );
	inline Vector3	GetScale(){ return Scale; }

	//	情報
	LPD3DXMESH	GetMesh(){ return lpMesh; }
	Texture2D*	GetTexture( int n ){ return lpTexture[n]; }
	void SetTexure(int n, Texture2D* tex){ lpTexture[n] = tex; }
};

typedef iexMesh IEXMESH, *LPIEXMESH;

//*****************************************************************************************************************************
//
//		３Ｄオブジェクト
//
//*****************************************************************************************************************************

typedef struct tagIEMFILE IEMFILE, *LPIEMFILE;

//------------------------------------------------------
//	アニメーション構造体
//------------------------------------------------------
typedef struct tagIEXANIME2 {
	u32			rotNum;
	u16*		rotFrame;
	Quaternion*	rot;

	u32			posNum;
	u16*		posFrame;
	Vector3*	pos;
} IEXANIME2, *LPIEXANIME2;

//------------------------------------------------------
//	３Ｄオブジェクト
//------------------------------------------------------
class iex3DObj : public iexMesh
{
protected:
	u8				version;
	u8				Param[16];

	u8				Motion;			//	現在のモーション番号
	u16				M_Offset[256];	//	モーション先頭フレーム

	u32				dwFrame;		//	現在のフレーム
	u32				NumFrame;		//	総フレーム数
	u16*			dwFrameFlag;	//	フレーム情報

	u32				RenderFrame;	//	レンダリングフレーム

	LPIEXANIME2		lpAnime;		//	ボーンアニメーション

	//	頂点情報
	DWORD			NumVertex;
	void*			lpVertex;

	// スキンメッシュ関係
	LPD3DXSKININFO		lpSkinInfo;	// スキン情報

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
	void	SetLoadFlag( BOOL bLoad ){ this->bLoad = bLoad; }
	iex3DObj(){
		bLoad = FALSE;
		for( int i=0 ; i<16 ; i++ ) Param[i] = 0;
	}
	iex3DObj( char* filename );
	~iex3DObj();

	iex3DObj*	Clone();

	BOOL LoadObject( char* filename );
	int LoadiEM( LPIEMFILE lpIem, LPSTR filename );
	BOOL CreateFromIEM( char* path, LPIEMFILE lpIem );

	LPD3DXSKININFO	CreateSkinInfo( LPIEMFILE lpIem );
	LPD3DXMESH	CreateMesh( LPIEMFILE lpIem );



	static BOOL SaveObject( LPIEMFILE lpIem, LPSTR filename );

	void Update();
	void SetMotion( int motion );
	inline int GetMotion(){ return Motion; }
	inline WORD GetMotionOffset( int m ){ return M_Offset[m]; }

	inline void SetFrame( int frame ){ dwFrame = frame; }
	inline int GetFrame(){ return dwFrame; }

	void Animation();

	void Render();
	void Render( DWORD flag, float alpha=-1 );
	void Render( iexShader* shader, char* name );
	void Render(iexShader* shader, char* name, Matrix& trans);
	void Render(const Matrix& TransMatrix);

	inline int GetParam( int n ){ return Param[n]; }
	inline void SetParam( int n, int p ){ Param[n] = p; }

	inline WORD GetFrameFlag( int frame ){ return dwFrameFlag[frame]; }
	inline void SetFrameFlag( int frame, WORD p ){ dwFrameFlag[frame] = p; }

	inline int GetNumFrame(){ return NumFrame; }

	inline Quaternion*	GetPose( int n ){ return &CurPose[n]; }
	inline Vector3*		GetBonePos( int n ){ return &CurPos[n]; }
	inline int	GetNumBone(){ return NumBone; }
	inline Matrix*	GetBone( int n ){ return &lpBoneMatrix[n]; }

	void UpdateSkinMeshFrame( float frame );
	void UpdateBoneMatrix();
	void UpdateSkinMesh();
};

typedef iex3DObj IEX3DOBJ, *LPIEX3DOBJ;

//*****************************************************************************************************************************
//
//		パーティクル関連
//
//*****************************************************************************************************************************

typedef struct tagPARTICLE	{
	int		type;			//	形

	int		aFrame;			//	出現フレーム
	COLOR	aColor;			//	出現カラー
	
	int		eFrame;			//	消滅フレーム
	COLOR	eColor;			//	出現カラー

	int		mFrame;			//	最高フレーム
	COLOR	mColor;			//	最高カラー

	Vector3		Pos;
	Vector3		Move;
	Vector3		Power;
	float		rotate;
	float		stretch;

	float	angle;
	float	scale;

	u8		flag;
} PARTICLE, *LPPARTICLE;

class iexParticleData
{
protected:
	u8		bFlags;
	float	r, g, b;
	PARTICLE	pdata;
	int		CurFrame;
	float	CurAlpha;

public:

	inline BOOL bActive(){ return (bFlags != 0); }
	
	void Set( LPPARTICLE pd );
	void SetFlag( u8 flag ){ bFlags = flag; }
	//	更新
	void Update();
	//	描画
	inline bool SetVertex( TLVERTEX* v );
	void Render();
	void Render( iexShader* shader, char* name );
};



class iexParticle
{
protected:
	static int	nParticles;					//	パーティクル最大数
	static iexParticleData*	ParticleData;	//	パーティクルデータ
	static iex2DObj* lpTexture;				//	テクスチャ

public:
	static Matrix CurViewProjection;
	//	初期化・解放
	static void Initialize( char* filename, int nParticles );
	static void Release();
	//	出現
	static void Set( LPPARTICLE pd );
	static void	Set( int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor, 
						LPVECTOR3 Pos, LPVECTOR3 Move, LPVECTOR3 Power, float rotate, float stretch, float scale, u8 flag );
	static void	Set( int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha, 
						LPVECTOR3 Pos, LPVECTOR3 Move, LPVECTOR3 Power, float r, float g, float b, float scale, u8 flag );

	//	更新
	static void Update();
	static void Reset();
	//	描画
	static void Render();
	static void Render( iexShader* shader, char* name );
};

//*****************************************************************************
//
//		２Ｄフォント関連
//
//*****************************************************************************

void	IEX_InitText( void );
void	IEX_ReleaseText( void );
void	IEX_DrawText( LPSTR str, s32 x, s32 y, s32 width, s32 height, COLOR color, BOOL bMini=FALSE );

//*****************************************************************************************************************************
//
//		入力関連
//
//*****************************************************************************************************************************

//*****************************************************************************
//		入力デバイス管理
//*****************************************************************************
class iexInputManager
{
private:
	static LPDIRECTINPUT8 pDI;

	static int NumDevice;
	static DIDEVICEINSTANCE	didi[4];

	static BOOL CALLBACK EnumDeviceCallback( const DIDEVICEINSTANCE* pdidi, VOID* pContext );
	static BOOL CALLBACK EnumAxes(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

public:
	static void Initialize();
	static LPDIRECTINPUTDEVICE8 GetDevice( int n );

	static void Release()
	{
		if( pDI ) pDI->Release();
	}
};


typedef u8	KEYCODE, *LPKEYCODE;

#define	KEY_UP		0
#define	KEY_DOWN	1
#define	KEY_LEFT	2
#define	KEY_RIGHT	3

#define	KEY_A		4
#define	KEY_B		5
#define	KEY_C		6
#define	KEY_D		7
#define	KEY_X		6
#define	KEY_Y		7

#define	KEY_L		8
#define	KEY_L1		8
#define	KEY_L2		10
#define	KEY_L3		12

#define	KEY_R		9
#define	KEY_R1		9
#define	KEY_R2		11
#define	KEY_R3		13

#define	KEY_START	14
#define	KEY_SELECT	15
#define	KEY_BACK	15

#define	KEY_ENTER	14
#define	KEY_SPACE	15

#define	KEY_B1		4
#define	KEY_B2		5
#define	KEY_B3		6
#define	KEY_B4		7
#define	KEY_B5		8
#define	KEY_B6		9
#define	KEY_B7		10
#define	KEY_B8		11
#define	KEY_B9		12
#define	KEY_B10		13
#define	KEY_B11		14
#define	KEY_B12		15
#define	KEY_B13		16
#define	KEY_B14		17
#define	KEY_B15		18

#define	KEY_AXISX	200
#define	KEY_AXISY	201
#define	KEY_AXISX2	202
#define	KEY_AXISY2	203

#define	KEY_AXISX3	204
#define	KEY_AXISY3	205
#define KEY_AAXISX3 206



typedef struct tagKEYSET 
{
	u8	up, down, left, right;
	u8	A, B, X, Y;
	u8	L1, L2, L3;
	u8	R1, R2, R3;
	u8	START, SELECT;
} KEYSET, *LPKEYSET;


#define	AXIS_X	0
#define	AXIS_Y	1
#define	AXIS_Z	2
#define	AXIS_RX	3
#define	AXIS_RY	4
#define	AXIS_RZ	5

#define AXIS_SLIDERX 6
#define AXIS_SLIDERY 7

typedef struct tagPADSET 
{
	u8	lx, ly, rx, ry, sx, sy;
	u8	A, B, X, Y;
	u8	L1, L2, L3;
	u8	R1, R2, R3;
	u8	START, SELECT;
} PADSET, *LPPADSET;

class iexInput
{
private:
	LPDIRECTINPUTDEVICE8 lpDevice;
	LPDIRECTINPUTEFFECT	 pEffect;

	static const int OrgKeyMap[20];
	static const int OrgJoyMap[20];
	int KeyMap[20];
	int JoyMap[20];
	u8	KeyInfo[20], JoyInfo[20];
	int		PadAxisX, PadAxisY;
	int		PadAxisX2, PadAxisY2;
	int		PadAxisX3, PadAxisY3;

	BOOL InitVibration();


public:
	static PADSET	ps101;
	static PADSET	sixaxis;
	static PADSET	xbox360;
	static PADSET	ps3;
	static PADSET padset_School;

	iexInput( int n );
	~iexInput();

	void Asign( KEYSET& keyset );
	void PadAsign( PADSET& padset );

	void SetInfo();
	int Get( KEYCODE key );

	void Vibration( u32 gain, float period );
};


//*****************************************************************************
//		アクセス関数
//*****************************************************************************
void	KEY_Asign( KEYSET& padset ,int n);
void	KEY_PadAsign(PADSET& padset, int n);

void	KEY_SetInfo(int n);
int		KEY_Get(KEYCODE key, int n);
void	KEY_Vibration(u32 gain, float period, int n);

int		KEY_GetAxisX(int n);
int		KEY_GetAxisY(int n);
int		KEY_GetAxisX2(int n);
int		KEY_GetAxisY2(int n);

BOOL	IEX_InitInput();
void	IEX_ReleaseInput();

#define	KEY(x,n)	KEY_Get(x,n)


//*****************************************************************************
//
//		オーディオ関連
//
//*****************************************************************************

#include "ogg/vorbisfile.h"

#define	STR_NORMAL	0
#define	STR_FADEIN	1
#define	STR_FADEOUT	2

#define	TYPE_WAV	0
#define	TYPE_OGG	1

class SoundBuffer
{
private:
	LPBYTE LoadWAV( LPSTR fname, LPDWORD size, LPWAVEFORMATEX wfx);
	LPDIRECTSOUNDBUFFER		lpBuf;
	LPDIRECTSOUND3DBUFFER	lpBuf3D;

public:
	SoundBuffer( LPDIRECTSOUND lpDS, char* filename, bool b3D );
	~SoundBuffer();

	LPDIRECTSOUNDBUFFER GetBuf(){ return lpBuf; }
	void Play( BOOL loop );
	void Stop();

	void	SetVolume( int volume );
	BOOL	isPlay();

	void	SetPos( Vector3* pos );
};

class iexStreamSound
{
private:
	static const int STRSECOND = 1;
	int		rate;
	LPDIRECTSOUNDBUFFER	lpStream;		// ストリーム用二次バッファ
	DSBPOSITIONNOTIFY	pn[3];

	HANDLE	hStrThread;
	u32	dwThreadId;
	u32	dwThrdParam;

	FILE*	hStrFile;
	OggVorbis_File	vf;
	u32	StrSize;
	u32	StrPos;
	u32	LoopPtr;

	u8		mode;
	s32		param;
	s32		volume;

	void Initialize( LPDIRECTSOUND lpDS, int rate );

	BOOL	OGGRead( LPBYTE dst, unsigned long size );

public:
	BYTE	type;
	HANDLE	hEvent[3];
	LPDIRECTSOUNDNOTIFY	lpStrNotify;


	iexStreamSound( LPDIRECTSOUND lpDS, LPSTR filename, BYTE mode, int param );
	~iexStreamSound();

	BOOL	SetBlockOGG( int block );
	BOOL	SetBlockWAV( int block );

	BOOL	SetWAV( LPDIRECTSOUND lpDS, char* filename );
	BOOL	SetOGG( LPDIRECTSOUND lpDS, char* filename );

	void	Stop();
	void	SetVolume( int volume );
	int		GetVolume(){ return volume; }

	void	SetMode( BYTE mode, int param );
	u8		GetMode(){ return mode; }
	int		GetParam(){ return param; }

};

typedef iexStreamSound DSSTREAM, *LPDSSTREAM;
typedef int iexSoundNumber;

class iexSound
{
private:
	static const int WavNum = 128;
	
	HWND	hWndWAV;

	LPDIRECTSOUND8			lpDS;		// DirectSoundオブジェクト
	LPDIRECTSOUNDBUFFER	lpPrimary;		// 一次バッファ

	LPDIRECTSOUND3DLISTENER lp3DListener;

	SoundBuffer* buffer[WavNum];

public:
	iexSound();
	~iexSound();

	void Set(iexSoundNumber no, char* filename, bool b3D = false);
	void SetPos(iexSoundNumber no, Vector3* pos){ buffer[no]->SetPos(pos); }
	void Play(iexSoundNumber no, BOOL loop = FALSE);
	void Stop(iexSoundNumber no);

	void SetListener( Vector3* pos, Vector3* target );


	void	SetVolume( int no, int volume );
	BOOL	isPlay( int no );

	iexStreamSound* PlayStream( char* filename );
	iexStreamSound* PlayStream( char* filename, BYTE mode, int param );

};








//*****************************************************************************
//
//		i-Extremeシステム初期化	
//
//*****************************************************************************

inline BOOL IEX_Initialize( HWND hWnd, BOOL bFullScreen, DWORD ScreenSize )
{
	return iexSystem::Initialize( hWnd, bFullScreen, ScreenSize );
}

inline void	IEX_Release(){ iexSystem::Release(); }

inline HWND	IEX_GetWindow(){ return iexSystem::Window; }
inline LPDEVICE IEX_GetD3DDevice(){ return iexSystem::Device; }


//*****************************************************************************
//
//		各種設定
//
//*****************************************************************************

void	IEX_SetProjection( float FovY, float Near, float Far );
void	IEX_SetProjectionEx( float FovY, float Near, float Far, float asp );
void	IEX_SetViewport( int x, int y, int w, int h );

//	レンダーステート設定関連
void	IEX_InitRenderState( void );
void	IEX_SetRenderState( DWORD state, D3DMATERIAL9* lpMaterial, LPDIRECT3DTEXTURE9 lpTexture );
//	フィルター設定
void	IEX_UseFilter( BOOL bFilter );
//	ライト関連
void	IEX_ResetLight( int index );

void	IEX_SetAmbient( COLOR Color );
void	IEX_SetDirLight( s32 index, Vector3* dir, float r, float g, float b );
void	IEX_SetPointLight( s32 index, Vector3* Pos, float r, float g, float b, float range );

void	IEX_SetFog( u32 Mode, float Param1, float Param2, COLOR Color );


//*****************************************************************************
//
//		視界関連
//
//*****************************************************************************
void	IEX_SetView( float posx, float posy, float posz, float anglex, float angley, float anglez );
void	IEX_SetViewTarget( Vector3 *pos, Vector3 *target );

//*****************************************************************************
//
//		メッシュ関連
//
//*****************************************************************************

//
LPIEXMESH	IEX_LoadXFile( LPSTR path, LPSTR filename );
LPIEXMESH	IEX_LoadMeshFromX( LPSTR path, LPSTR filename );
LPIEXMESH	IEX_LoadMeshFromX( LPSTR filename );
void		IEX_ReleaseMesh( LPIEXMESH lpMesh );

//
void	IEX_SetMeshPos( LPIEXMESH lpMesh, float x, float y, float z );
void	IEX_SetMeshAngle( LPIEXMESH lpMesh, float x, float y, float z );
void	IEX_SetMeshScale( LPIEXMESH lpMesh, float x, float y, float z );

//
void	IEX_RenderMesh( LPIEXMESH lpMesh );
void	IEX_RenderMesh( LPIEXMESH lpMesh, u32 dwFlags, float param );
void	IEX_RenderMesh( LPIEXMESH lpMesh, iexShader* shader, char* name );

int	IEX_RayPickMesh( LPIEXMESH lpMesh, Vector3* out, Vector3* pos, Vector3* vec, float *Dist );

//
//		頂点カラーメッシュ
//

LPIEXMESH	IEX_LoadIMO( LPSTR filename );

//*****************************************************************************
//
//		３Ｄオブジェクト
//
//*****************************************************************************

//	ロード・解放
LPIEX3DOBJ	IEX_Load3DObject( LPSTR filename );
void		IEX_Release3DObject( LPIEX3DOBJ lpObj );

//	レンダリング
void	IEX_NoRender3DObject( LPIEX3DOBJ lpObj );
void	IEX_Render3DObject( LPIEX3DOBJ lpObj );
void	IEX_Render3DObject( LPIEX3DOBJ lpObj, DWORD flag, float alpha=-1.0f );
void	IEX_Render3DObject( LPIEX3DOBJ lpObj, iexShader* shader, char* name );

//	モーション
void	IEX_SetObjectMotion( LPIEX3DOBJ lpObj, s32 motion );
s32		IEX_GetObjectMotion( LPIEX3DOBJ lpObj );
void	IEX_ObjectFrameNext( LPIEX3DOBJ lpObj );

//	情報
void	IEX_SetObjectPos( LPIEX3DOBJ lpObj, float x, float y, float z  );
void	IEX_SetObjectAngle( LPIEX3DOBJ lpObj, float x, float y, float z  );
void	IEX_SetObjectScale( LPIEX3DOBJ lpObj, float scale );
void	IEX_SetObjectScale( LPIEX3DOBJ lpObj, float scaleX, float scaleY, float scaleZ );

//	パラメータ
u8		IEX_GetObjectParam( LPIEX3DOBJ lpObj, s32 index );
void	IEX_SetObjectParam( LPIEX3DOBJ lpObj, s32 index, u8 param );

//*****************************************************************************
//
//		２Ｄオブジェクト関連
//
//*****************************************************************************

//	２Ｄオブジェクト
inline LPIEX2DOBJ	IEX_Load2DObject( LPSTR filename ){ return new iex2DObj(filename); }
inline LPIEX2DOBJ	IEX_Create2DObject( int width, int height, u8 flag ){ return new iex2DObj( width, height, flag ); }
inline void	IEX_Release2DObject( LPIEX2DOBJ lpObj ){ if( lpObj ) delete lpObj; }

void	IEX_Render2DObject( s32 DstX, s32 DstY, s32 DstW, s32 DstH, LPIEX2DOBJ lpObj, s32 SrcX, s32 SrcY, s32 width, s32 height, u32 dwFlags=RS_COPY, COLOR color=0xFFFFFFFF );

//*****************************************************************************
//
//		パーティクル関連
//
//*****************************************************************************
void	IEX_InitParticle( LPSTR filename );
void	IEX_ReleaseParticle( void );
void	IEX_DrawParticles( void );
void	IEX_ExecuteParticles( void );

//	構造体指定
void	IEX_SetParticle( LPPARTICLE p );

//
void	IEX_SetParticle( int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor, 
						Vector3* Pos, Vector3* Move, Vector3* Power, float rotate, float stretch, float scale, u8 flag );

//	色変化、スケール変化なし
void	IEX_SetParticle( int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha, 
						Vector3* Pos, Vector3* Move, Vector3* Power,
						float r, float g, float b, float scale, u8 flag );

//*****************************************************************************
//
//		３Ｄポリゴン関連
//
//*****************************************************************************

//	４頂点ポリゴン描画
void	IEX_Render3DPolygon( LPVERTEX v, LPIEX2DOBJ lpObj, u32 dwFlags, COLOR color );
void	IEX_Render3DPolygon( Vector3 p[4], float tu[4], float tv[4], LPIEX2DOBJ lpObj, u32 dwFlags, COLOR color );

//	ストリップポリゴン描画
void	IEX_RenderStripPolygon( LPLVERTEX lpVertex, s32 StripNum, LPIEX2DOBJ lpObj, u32 dwFlags );

//*****************************************************************************
//
//		２Ｄポリゴン関連
//
//*****************************************************************************

void	IEX_DrawRect( s32 DstX, s32 DstY, s32 DstW, s32 DstH, u32 dwFlags, COLOR color );
void	IEX_DrawRectZ( s32 DstX, s32 DstY, s32 DstW, s32 DstH, float z, u32 dwFlags, COLOR color );

void	IEX_Render2DPolygon( LPTLVERTEX v, int NumPoly, LPIEX2DOBJ lpObj, u32 dwFlags );
void	IEX_Render2DPolygon( float* vx, float* vy, float* tu, float* tv, int NumPoly, LPIEX2DOBJ lpObj, u32 dwFlags, COLOR color );


//*****************************************************************************
//
//		オーディオ関連
//
//*****************************************************************************

//	サウンドシステム初期化・終了
BOOL	IEX_InitAudio();
void	IEX_ReleaseAudio();
//	サウンドセット・再生・停止	
BOOL	IEX_SetWAV(iexSoundNumber no, LPSTR fname, bool b3D = false);
void	IEX_PlaySound(iexSoundNumber no, BOOL loop);
void	IEX_StopSound(iexSoundNumber no);
void	IEX_SetSoundPos(iexSoundNumber no, Vector3* pos);

//	情報設定・取得
BOOL	IEX_GetSoundStatus(iexSoundNumber no);
void	IEX_SetSoundVolume(iexSoundNumber no, s32 volume);

//	再生・停止・解放	
LPDSSTREAM	IEX_PlayStreamSound( LPSTR filename );
LPDSSTREAM	IEX_PlayStreamSoundEx( LPSTR filename, u8 mode, s32 param );
BOOL		IEX_StopStreamSound( LPDSSTREAM lpStream );
BOOL		IEX_ReleaseStreamSound( LPDSSTREAM lpStream );

//	情報設定
void	IEX_SetStreamSoundVolume( LPDSSTREAM lpStream, s32 volume );
void	IEX_SetStreamMode( LPDSSTREAM lpStream, u8 mode, s32 param );


#endif

