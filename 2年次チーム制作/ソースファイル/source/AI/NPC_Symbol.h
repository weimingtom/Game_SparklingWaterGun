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
	float				m_enemy_InSight;						//Ž‹ŠE“à‚É“G‚ª‚¢‚é‚©‚ÌƒXƒRƒA’l(Ž‹ŠE“à‚É‚¢‚éê‡‚Íˆê”Ô‹ß‚¢“G‚Æ‚Ì‹——£(SQ)A‚¢‚È‚¢ê‡‚Í-1.0f)
	float				m_enemy_range_OfTarget;			//“G‚ªŽË’ö“à‚É‚¢‚é‚©‚ÌƒXƒRƒA’l(ŽË’ö“à‚É‚¢‚éê‡‚Íˆê”Ô‹ß‚¢“G‚Æ‚Ì‹——£(SQ)A‚¢‚È‚¢ê‡‚Í-1.0f)
	ENTITY_ID	m_aimRing_Target;						//AimƒŒƒeƒBƒNƒ‹‚Ìƒ^[ƒQƒbƒgî•ñ
	ENTITY_ID	m_enemy_nearRange_OfTarget;	//‹ßÚUŒ‚‚ª“Í‚­“G‚ª‚¢‚é‚©‚Ç‚¤‚©(‚¢‚È‚¢ê‡‚Íid_error)
	bool				m_importance_Sound;				//d—v‚È‰¹‚ª•·‚±‚¦‚½‚©‚ÌƒXƒRƒA’l
	int				m_weapon_Charge;						//•Ší‚Í‚«‚¿‚ñ‚Æcharge‚³‚ê‚Ä‚¢‚é‚©(-1‚Åcharge‚³‚ê‚Ä‚¢‚éA®”’l‚Å•K—v‚Ècharge“)
	bool				m_attack_Me;								//Ž©•ª‚ªUŒ‚‚³‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©
	//emum	ƒ^[ƒQƒbƒg‚Ìó‘Ô
	bool				m_near_Friend;							//–¡•û‚ª‹ß‚­‚É‚¢‚é‚©
	bool				m_win_Now;								//ƒXƒRƒA‚Å‚©‚Á‚Ä‚¢‚é‚©
	ENTITY_ID	m_pinch_Friend;							//UŒ‚‚³‚ê‚Ä‚¢‚é–¡•û‚ª‹ß‚­‚É‚¢‚é‚©
	ENTITY_ID	m_down_Friend;							//“|‚ê‚Ä‚¢‚é–¡•û‚ª‚¢‚é‚©
	ENTITY_ID	m_is_Danger;								//ŠëŒ¯‚Èó‘Ô‚©‚Ç‚¤‚©(’N‚É‘_‚í‚ê‚Ä‚¢‚é‚©)
	bool				m_attack_OutSight;					//Ž‹ŠEŠO‚©‚çUŒ‚‚³‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©(“G‚ÌID‚ðŠi”[‚·‚é‚ª,UŒ‚‚µ‚Ä‚«‚½•ûŒüˆÈŠO‚Ìî•ñ‚ðŽg—p‚µ‚È‚¢)
	bool				m_im_Fine;								//Œ³‹C‚©‚Ç‚¤‚©

public :
	void	Initialize();
	/**
	@brief update < is_Danger
	@note ‚±‚ÌƒVƒ“ƒ{ƒ‹‚¾‚¯‚ÍÅIƒS[ƒ‹‚ð•]‰¿‚·‚éÛ‚ÉƒŠƒXƒN’l‚ð•]‰¿‚·‚é’iŠK‚ÅŽg‚¦‚é‚Ì‚Åƒvƒ‰ƒ“ƒjƒ“ƒO‚ÌÅ‰‚Ì’iŠK‚Ås‚¤
	*/
	void	Update_IsDanger(NPC_Brain* brain, Character* character);
	bool	Update( NPC_Brain* brain,Character* character);
};

#endif