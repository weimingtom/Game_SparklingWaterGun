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
	*@brief �w�肵�����W�Ƃ̎��E����Ǝː�������Ƃ�
	*@param1 �L�����N�^�[���
	*@param2 ���E������s�����W
	*@param3 �߂�l�Ƃ��Ď��E���茋��
	*@param4 �߂�l�Ƃ��Ďː����茋��
	*@param5 �����t���O
	*@note ���̏ꍇMapObject�Ƃ�Ray�Ɏ��s���邩��
	*@note Ray�̖{�������炷���߂Ɏ��E�Ǝː�����͓����ɍs��
	*/
	bool		SightCheck_and_Line_of_Fire(Character* character, const Vector3& target,
																bool* visibility, bool* line_of_Fire,bool* in_Fov, int see_through = -1);
	/**
	*@brief �G�̍s����\������
	*/
	ExpectationAct	Expectation_Action(Character* me, Character* enemy, const Character_Record* before_Record);
	/**
	*@brief �^�[�Q�b�g�ƂȂ�G�����肷��
	*/
	void	Decision_TargetEnemy(Character* character);
	/**
	*@brief �^�[�Q�b�g�ƂȂ�L��Object�����肷��
	*/
	bool	Direction_AdvertisementPointObject(Character* character);
	/**
	*@brief ���ݎ����Ă����񂩂�^�[�Q�b�g�L�����N�^�[�̈ʒu��\������
	*@param �Ō�Ɏ��F�����Ƃ��̋L��
	*@param �L���̐M���l
	*@return �\���ɐ��������ꍇ��true: ���s��false
	*@note �Ō�ɋL�^�����i�s�x�N�g���ƃm�[�h������g����΂Ȃ�Ƃ�
	*/
	bool	Expectation_TargetPos_Character(const Character_Record* memory, Vector3* ret_Pos, const float& memory_Retability);
	/**
	*@brief ���ݎ����Ă����񂩂�mapobject�̈ʒu��\������
	*@param �Ō�Ɏ��F�����Ƃ��̋L��
	*@param �L���̐M���l
	*@return �\���ɐ��������ꍇ��true: ���s��false
	*@note �Ō�ɋL�^�����i�s�x�N�g���ƃm�[�h������g����΂Ȃ�Ƃ�
	*/
	bool	Expectation_TargetPos_MapObejct(const MapObject_Record* memory, Vector3* target_Pos, const float& memory_Retability);
	/**
	*brief �˒������ǂ����𔻒肷��
	*return �����Ƀ^�[�Q�b�g�Ƃ̋���(sq)
	*return �����Ɏː����ʂ��Ă��邩�ǂ���(true�Ŏː����ʂ�)
	*return �����Ɏ��E��(Fov)�ɂ͂����Ă邩�ǂ���
	*/
	bool	Range_of_Traget(Character* character, float* target_dist,
										bool* line_of_fire = nullptr,bool* in_Fov = nullptr);
	/**
	*@brief ���݂̃^�[�Q�b�g�����E���ɂ��邩�ǂ������擾
	*@return	true	�Ŏ��E�ɂ���
	*/
	bool	Target_InSight();
	/**
	*@brief �^�[�Q�b�g�����E���猩�������Ƃ��ɂ�Ԃ��
	*@���� Evalutor_GoalAttack
	*/
	bool	Target_at_LoseSight(Character* character, Vector3* evalute_GoalPos, float* target_dist);

	/**
	*@brief �񕜂��ׂ��Ώۂ����肷��
	*/
	bool	Direction_MedicalFriend(NPC_Brain* brain, Character* character);
	/**
	*@brief ���݂̃^�[�Q�b�g���ߐڍU���̎˒����ɓ����Ă������ǂ�����]������
	*@return �˒��������Ȃ�true
	*@note current cse ShotWeaponAttackEnemy::Process
	*/
	bool	Target_is_NearRange(NPC_Brain* brain, Character* character);
	/**
	*@brief �ߐڍU�����s���G�����肷��
	*/
	bool	Direction_NearAttackTarget(NPC_Brain* brain, Character* character);
	/**
	*@brief �O���l�[�h�𓊂���Ώۂ����肷��
	*/
	bool	Direction_GrenadeAttackTarget(NPC_Brain* brain, Character* character);
	/**
	*@brief �g���b�N�Ȃǂ̃I�u�W�F�N�g�ɑ΂��Ăǂꂭ�炢�댯���ǂ����`�F�b�N����B�댯�Ɣ��f�����ꍇ�͉��炩��NotPlanningGoal��}������
	*@return �댯�ȏꍇ��true,����ȊO��false
	*/
	bool	Check_Danger_Object(NPC_Brain* brain, Character* character,
		MapObject_Record* check_record);
	/**
	*@brief �ޔ�ΏۂƂȂ�MapObject����ޔ����W�����肷��
	*@param �^�[�Q�b�g�ƂȂ�Mapobject�̋L��
	*@param �߂�l�Ƃ��č��W
	*@return ��������W������ł�����true
	*/
	bool	Direction_AvoidancePoint(NPC_Brain* brain, Character* character,
													MapObject_Record*	target_record, Vector3* ret_Pos);
	
};

#endif
