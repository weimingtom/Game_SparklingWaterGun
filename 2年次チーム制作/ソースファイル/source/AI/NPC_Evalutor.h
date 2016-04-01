#ifndef __NPC_EVALUTOR_H__
#define __NPC_EVALUTOR_H__

class NPC_Brain;
class Character;

class NPC_Evalutor
{
	friend class NPC_Brain;
private :
	NPC_Brain* m_holder;

private :
	NPC_Evalutor(NPC_Brain* holder);

public :
	/**
	*@brief 指定した座標との視界判定と射線判定をとる
	*@param1 キャラクター情報
	*@param2 視界判定を行う座標
	*@param3 戻り値として視界判定結果
	*@param4 戻り値として射線判定結果
	*@param5 透視フラグ
	*@note 今の場合MapObjectとのRayに失敗するかも
	*@note Rayの本数を減らすために視界と射線判定は同時に行う
	*/
	bool		SightCheck_and_Line_of_Fire(Character* character, const Vector3& target,
																bool* visibility, bool* line_of_Fire,bool* in_Fov, int see_through = -1);
	/**
	*@brief 敵の行動を予測する
	*/
	ExpectationAct	Expectation_Action(Character* me, Character* enemy, const Character_Record* before_Record);
	/**
	*@brief ターゲットとなる敵を決定する
	*/
	void	Decision_TargetEnemy(Character* character);
	/**
	*@brief ターゲットとなる広告Objectを決定する
	*/
	bool	Direction_AdvertisementPointObject(Character* character);
	/**
	*@brief 現在持っている情報からターゲットキャラクターの位置を予測する
	*@param 最後に視認したときの記憶
	*@param 記憶の信頼値
	*@return 予測に成功した場合はtrue: 失敗でfalse
	*@note 最後に記録した進行ベクトルとノード情報を駆使すればなんとか
	*/
	bool	Expectation_TargetPos_Character(const Character_Record* memory, Vector3* ret_Pos, const float& memory_Retability);
	/**
	*@brief 現在持っている情報からmapobjectの位置を予測する
	*@param 最後に視認したときの記憶
	*@param 記憶の信頼値
	*@return 予測に成功した場合はtrue: 失敗でfalse
	*@note 最後に記録した進行ベクトルとノード情報を駆使すればなんとか
	*/
	bool	Expectation_TargetPos_MapObejct(const MapObject_Record* memory, Vector3* target_Pos, const float& memory_Retability);
	/**
	*brief 射程内かどうかを判定する
	*return 引数にターゲットとの距離(sq)
	*return 引数に射線が通っているかどうか(trueで射線が通る)
	*return 引数に視界内(Fov)にはいってるかどうか
	*/
	bool	Range_of_Traget(Character* character, float* target_dist,
										bool* line_of_fire = nullptr,bool* in_Fov = nullptr);
	/**
	*@brief 現在のターゲットが視界内にいるかどうかを取得
	*@return	true	で視界にいる
	*/
	bool	Target_InSight();
	/**
	*@brief ターゲットを視界から見失ったときによぶやつ
	*@今は Evalutor_GoalAttack
	*/
	bool	Target_at_LoseSight(Character* character, Vector3* evalute_GoalPos, float* target_dist);

	/**
	*@brief 回復すべき対象を決定する
	*/
	bool	Direction_MedicalFriend(NPC_Brain* brain, Character* character);
	/**
	*@brief 現在のターゲットが近接攻撃の射程内に入ってきたかどうかを評価する
	*@return 射程距離内ならtrue
	*@note current cse ShotWeaponAttackEnemy::Process
	*/
	bool	Target_is_NearRange(NPC_Brain* brain, Character* character);
	/**
	*@brief 近接攻撃を行う敵を決定する
	*/
	bool	Direction_NearAttackTarget(NPC_Brain* brain, Character* character);
	/**
	*@brief グレネードを投げる対象を決定する
	*/
	bool	Direction_GrenadeAttackTarget(NPC_Brain* brain, Character* character);
	/**
	*@brief トラックなどのオブジェクトに対してどれくらい危険かどうかチェックする。危険と判断した場合は何らかのNotPlanningGoalを挿入する
	*@return 危険な場合はtrue,それ以外はfalse
	*/
	bool	Check_Danger_Object(NPC_Brain* brain, Character* character,
		MapObject_Record* check_record);
	/**
	*@brief 退避対象となるMapObjectから退避座標を決定する
	*@param ターゲットとなるMapobjectの記憶
	*@param 戻り値として座標
	*@return 逃げる座標が決定できたらtrue
	*/
	bool	Direction_AvoidancePoint(NPC_Brain* brain, Character* character,
													MapObject_Record*	target_record, Vector3* ret_Pos);
	
};

#endif
