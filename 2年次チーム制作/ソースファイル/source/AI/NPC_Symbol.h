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
	float				m_enemy_InSight;						//���E���ɓG�����邩�̃X�R�A�l(���E���ɂ���ꍇ�͈�ԋ߂��G�Ƃ̋���(SQ)�A���Ȃ��ꍇ��-1.0f)
	float				m_enemy_range_OfTarget;			//�G���˒����ɂ��邩�̃X�R�A�l(�˒����ɂ���ꍇ�͈�ԋ߂��G�Ƃ̋���(SQ)�A���Ȃ��ꍇ��-1.0f)
	ENTITY_ID	m_aimRing_Target;						//Aim���e�B�N���̃^�[�Q�b�g���
	ENTITY_ID	m_enemy_nearRange_OfTarget;	//�ߐڍU�����͂��G�����邩�ǂ���(���Ȃ��ꍇ��id_error)
	bool				m_importance_Sound;				//�d�v�ȉ��������������̃X�R�A�l
	int				m_weapon_Charge;						//����͂������charge����Ă��邩(-1��charge����Ă���A�����l�ŕK�v��charge��)
	bool				m_attack_Me;								//�������U������Ă��邩�ǂ���
	//emum	�^�[�Q�b�g�̏��
	bool				m_near_Friend;							//�������߂��ɂ��邩
	bool				m_win_Now;								//�X�R�A�ł����Ă��邩
	ENTITY_ID	m_pinch_Friend;							//�U������Ă��閡�����߂��ɂ��邩
	ENTITY_ID	m_down_Friend;							//�|��Ă��閡�������邩
	ENTITY_ID	m_is_Danger;								//�댯�ȏ�Ԃ��ǂ���(�N�ɑ_���Ă��邩)
	bool				m_attack_OutSight;					//���E�O����U������Ă��邩�ǂ���(�G��ID���i�[���邪,�U�����Ă��������ȊO�̏����g�p���Ȃ�)
	bool				m_im_Fine;								//���C���ǂ���

public :
	void	Initialize();
	/**
	@brief update < is_Danger
	@note ���̃V���{�������͍ŏI�S�[����]������ۂɃ��X�N�l��]������i�K�Ŏg����̂Ńv�����j���O�̍ŏ��̒i�K�ōs��
	*/
	void	Update_IsDanger(NPC_Brain* brain, Character* character);
	bool	Update( NPC_Brain* brain,Character* character);
};

#endif