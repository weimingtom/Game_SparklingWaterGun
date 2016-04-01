#ifndef __AIM_POINT_H__
#define __AIM_POINT_H__

#include "iextreme.h"
#include "ManagerBase.h"
#include "GameSystem\GameBase.h"

enum AIMTARGET_ID
{
	ERR_AIMTARGET_ID = -1
};

//********************************************************
//		狙う目標の情報
//********************************************************
struct AimTargetInfo
{
	Vector3			cli_pos;	//目標位置(ワールド空間上)
	Vector2			scr_pos;	//目標位置(スクリーン上)
	Vector3			cli_move;	//１フレーム間の移動量
	DWORD			color;		//エイムの円の色
	int				team_type;	//チーム番号(チームではない場合は-1)
	float			weight;		//優先度
	float			size;		//エイムの円の大きさ
	AIMTARGET_ID	id;			//エイムターゲットID
	ENTITY_ID		entity_id;	//ID(errorの場合あり)
};

//********************************************************
//		狙っている者の情報
//********************************************************
struct AimOwnerInfo
{
	LPCAMERA	pCamera;				//狙っている者のカメラ
	int			team_type;				//チームタイプ
	Vector3		position;				//位置
	Vector3		view_vec;				//視線ベクトル
	float		circle_size;			//認識する範囲
	float		range;					//0~1
	float		camera_z;				//カメラ空間でのZ値
	ENTITY_ID	id;
};

typedef const AimOwnerInfo&		CRAimOwnerInfo;

//********************************************************
//		エイムターゲットベースクラス
//********************************************************
class AimTarget
{
protected:
	const AIMTARGET_ID myId;
public:
	AimTarget();
	virtual ~AimTarget();
	virtual bool isTarget(AimTargetInfo &out, CRAimOwnerInfo info) = 0;	//情報を得る (ターゲットでない場合はfalseを返す)
};

typedef AimTarget* LPAIMTARGET;

//********************************************************
//		エイムの円のクラス
//********************************************************
class AimPoint
{
private:
	static const float max_scale;
	LPIEX2DOBJ		m_pTexture;		//円テクスチャ
	Vector2			m_position2d;	//座標
	Vector3			m_position;
	COLOR			m_color;		//色
	AimTargetInfo	m_point[2];		//移動情報
	float			m_next_weight;	//次のターゲットの重み
	float			m_scale;		//現在の大きさ(直径)
	RATIO			m_hit_reticle;	//ヒットレティクルのアルファ
	float			m_movearea_size;//移動可能範囲
	float			m_speed;		//目標に移動する補間スピード
	float			m_default_size;	//通常時の大きさ(この大きさに徐々にもどる)
	float			m_scale_speed;	//元の大きさに戻るスピード
	bool			m_change_target;//サイト内に目標が二体以上いた場合に自動的に目標を変えるかどうかのフラグ
public:
	AimPoint(
		LPIEX2DOBJ	texture,
		float		movearea_size,
		float		default_size,
		float		scale_speed,
		float		move_speed
		);
	~AimPoint();
private:
	//移動目標の情報を得る
	void GetGoPosition(
		AimTargetInfo &out,
		CRAimOwnerInfo owner);

	//反動を更新
	void					ReactionUpdate(float target);
	Vector2					GetInCircleRandPos(RATE seed);							//サークル内の場所をランダムに返す
public:
	void					SetChangeTarget(bool changeFlg);						//サイト内に目標が二体以上いた場合に自動的に目標を変えるかどうか
	const AimTargetInfo&	GetTarget()const{ return m_point[1]; }
	void					GetReactionTargetPos(CRVector3 arm_pos,Vector3 &out);
	CRVector2				GetPosition();											//サークル中心の画面座標を得る
	void					SetReaction(float power);								//サークルを大きくする
	void					Update(CRAimOwnerInfo owner);							//更新
	void					Render();												//描画
};

//********************************************************
//		ターゲットを管理
//********************************************************
static const int AIM_TARGET_MAX = 50;

class AimTargetManager:
	public		ManagerBase<AimTarget, AIM_TARGET_MAX>,
	protected	GameBaseEntity
{
public:
	AimTargetManager();
	~AimTargetManager();
public:
	void GetTarget(AimTargetInfo &out, CRAimOwnerInfo info);
	bool HandleMessage(LPGameMessage msg);
};




#endif