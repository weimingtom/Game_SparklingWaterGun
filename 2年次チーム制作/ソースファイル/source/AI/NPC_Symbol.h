#ifndef __SYMBOL_H__
#define __SYMBOL_H__

class NPC_Brain;

class Symbol
{
	friend class NPC_Brain;
private :
	//@symbol < target_InSight	Range_OfTarget
	void	Update_Target_Enemy( NPC_Brain* brain, Character* character);
	//@update < inportance_sound
	void	Update_Importance_Sound( NPC_Brain* brain, Character* character);
	//@update < weapon_Chargedesire
	void	Update_Weapon_Charge(NPC_Brain* brain, Character* character);
	//@update < near_Friend	pinch_Friend		down_Friend
	void	Update_Friend( NPC_Brain* brain, Character* character);
	//@update < im_Fine
	void	Update_Helth( NPC_Brain* brain, Character* character);
	//@update < attack_OutSight
	void	Update_OutSight(NPC_Brain* brain, Character* character);
	//@update < enemy_nearRange_OfTarget;
	void	Update_NearRangeTarget(NPC_Brain* brain, Character* character);
	//@update < aimRind_Target
	void	Update_AimRingTarget(NPC_Brain* brain, Character* character);

public :
	float				m_enemy_InSight;						//視界内に敵がいるかのスコア値(視界内にいる場合は一番近い敵との距離(SQ)、いない場合は-1.0f)
	float				m_enemy_range_OfTarget;			//敵が射程内にいるかのスコア値(射程内にいる場合は一番近い敵との距離(SQ)、いない場合は-1.0f)
	ENTITY_ID	m_aimRing_Target;						//Aimレティクルのターゲット情報
	ENTITY_ID	m_enemy_nearRange_OfTarget;	//近接攻撃が届く敵がいるかどうか(いない場合はid_error)
	bool				m_importance_Sound;				//重要な音が聞こえたかのスコア値
	int				m_weapon_Charge;						//武器はきちんとchargeされているか(-1でchargeされている、整数値で必要なcharge％)
	bool				m_attack_Me;								//自分が攻撃されているかどうか
	//emum	ターゲットの状態
	bool				m_near_Friend;							//味方が近くにいるか
	bool				m_win_Now;								//スコアでかっているか
	ENTITY_ID	m_pinch_Friend;							//攻撃されている味方が近くにいるか
	ENTITY_ID	m_down_Friend;							//倒れている味方がいるか
	ENTITY_ID	m_is_Danger;								//危険な状態かどうか(誰に狙われているか)
	bool				m_attack_OutSight;					//視界外から攻撃されているかどうか(敵のIDを格納するが,攻撃してきた方向以外の情報を使用しない)
	bool				m_im_Fine;								//元気かどうか

public :
	void	Initialize();
	/**
	@brief update < is_Danger
	@note このシンボルだけは最終ゴールを評価する際にリスク値を評価する段階で使えるのでプランニングの最初の段階で行う
	*/
	void	Update_IsDanger(NPC_Brain* brain, Character* character);
	bool	Update( NPC_Brain* brain,Character* character);
};

#endif