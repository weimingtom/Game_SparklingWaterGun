#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "iextreme.h"
#include "ControllObject.h"
#include "Camera.h"
#include "ParentAndChild.h"
#include "CharacterAim.h"
#include "CharacterUI.h"
#include "CODBOForwardDecl.h"
#include "CharacterEye.h"

//****************************************************//
//�@			�L�����N�^��{�p�����[�^�[
//****************************************************//

typedef struct
{
	Matrix					m_trans_mat;		//�p���s��
	Vector3					m_move;				//�ړ���
	Vector3					m_view_vec;			//�����x�N�g��
	Vector3					m_parent_move;		//�e�̈ړ��ɂ���ē������ꂽ��
	ENTITY_ID				m_RideObjectId;		//����Ă���I�u�W�F�N�g
	float					m_LifePoint;		//�̗�
	float					m_Count;			//���낢��ȏ����̃J�E���g
	ENTITY_ID				m_last_damage_parent;//�ŋߎ󂯂��_���[�W�̎�����
	int						m_last_damage_count; //�ŋߎ󂯂��_���[�W����̌o�߃t���[��
	COLORf					m_OutLineColor;		//���g�̗֊s���̐F

	struct
	{
		float MoveSpeed;
		float ChargeSpeed;
		RATIO PowerUpAlpha;
	}
	UpValue;
}
CHRPARAM, *	LPCHRPARAM;

//****************************************************//
//�@			�L�����N�^�̃t���[�����Ƃ̏��
//****************************************************//
class  CHRFRAMEINFO
{
public:
	Vector3		pre_pos;		//�P�t���[���O�̈ʒu
	Vector3		frame_move;		//�P�t���[���̈ړ�
	bool		isHitWall;		//�ǂƂ̐ڐG���
	bool		isHitFloor;		//���Ƃ̐ڐG���
public:
	CHRFRAMEINFO();
	void Reset();
};

typedef const CHRFRAMEINFO& CRCHRFRAMEINFO;


//�L�����N�^�̏�����͌Œ�ŁI




const Vector3 CHARACTER_UPVEC(0, 1, 0);

//****************************************************//
//�@�L�����N�^�s���X�e�[�g
//****************************************************//

namespace chr_move_state
{
	enum _ENUM_CHARACTER_MOVE_STATE_TYPE
	{
		error_mode = -1,

		usual_mode = 0,		//�ʏ�ړ���
		jump_mode,			//�W�����v��
		dying_mode,			//�m����
		die_mode,			//���S��
		near_atk_mode,		//�ߋ����U��
		throw_grenade,
		rolling,
		vanish,
		parachute,
		apeer_vending_machine,

		_state_max,
	};
}

typedef chr_move_state::_ENUM_CHARACTER_MOVE_STATE_TYPE CHARACTER_STATE_TYPE;

//****************************************************//
//�@�L�����N�^�N���X
//****************************************************//



class Character :public ControllObject,public CHILD
{
private:
	LPCHARACTERMOVESTATETABLE	m_pStateTable;			//���̃L�����N�^�̃X�e�[�g�e�[�u��
protected:
	AimTargetCharacter			m_AimTarget;			//���̃v���C���[���_���^�[�Q�b�g
	LPCHARACTER_UI				m_pUserInterface;		//���̃L�����N�^��UI
	LPCHARACTERMODELBASE		m_pCharacterModel;		//�L�����N�^���f��
	CHRPARAM					m_Param;				//�L�����N�^��{�p�����[�^
	LPCHARACTER_BRAINBASE		m_pBrain;				//���̃L�����N�^�𓮂����]
	CHARACTER_STATE_TYPE		m_State_type;			//���ݎ��s���Ă���X�e�[�g�̃^�C�v
	LPCHARACTERWEPONBASE		m_pWepon;				//�������Ă��镐��
	LPCHARACTER_LOCKON			m_pLockOnClass;			//���݂̃J�����̃��b�N�I����Ԃ�ێ�����N���X
	CHRFRAMEINFO				m_frame_info;			//���݂̃t���[���̏��
	CharacterEye				m_Eye;					//�ڂ̕\���N���X
	R_InterFace_Character*		m_render_Interface;		//�L�����N�^�[�̕`��N���X
public:
	Character(
		ENTITY_ID					id,				//���̃L�����N�^�̈�ӂȔԍ�
		LPCHARACTER_BRAINBASE		m_pBrain,		//���̃L�����N�^�𓮂����]�N���X
		LPCHARACTERMODELBASE		pCharacterModel,//���̃L�����N�^�̃��f���N���X
		LPCHARACTER_UI				pUserInterface,
		int							team_type,
		TeamData*					tData
		);
	~Character();
private:
	//����Ă�����̂��������Ƃ��ɌĂ΂��֐�
	void ParentMove(LPCMATRIX pParentWorldMat, LPCMATRIX pParentWorldMatInv, LPCMATRIX pParentMoveMat, LPCMATRIX AffineMat);
	void PowerUp(MSG_TYPE type, float value);
public:
	CRCHRFRAMEINFO GetFrameInfo()const;
	Vector3 GetBodyCenter();
	void Controll();							//���̃L�����N�^�𑀍삷��֐�
	void Update(TeamData& teamData);		//���t���[���̍X�V
	void Render(ControllObject& viewowner, TeamData& teamdata);								//�`��
	void RenderUI(ControllObject& viewowner, TeamData& teamdata);							//UI��`��
	bool HandleMessage(LPGameMessage msg);		//���̃L�����N�^�ւ̃��b�Z�[�W�𑗂�
	void Render_ShadowMap(ControllObject& viewowner, TeamData& teamdata);
	void TestFunction();
};



#endif