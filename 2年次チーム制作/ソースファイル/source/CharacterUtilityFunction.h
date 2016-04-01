#ifndef __CHARACTER_UTILITY_FUNCTION_H__
#define __CHARACTER_UTILITY_FUNCTION_H__

#include "Character.h"
#include "CharacterModel.h"
#include "CharacterModelResource.h"
#include "CODBOForwardDecl.h"
#include "FriendMessage.h"


class	CharacterUtilityFunctions;
typedef CharacterUtilityFunctions	ChrFunc;

struct DamageParam;

class CharacterUtilityFunctions: public Character
{
private:
	CharacterUtilityFunctions();
	~CharacterUtilityFunctions();
public:
	//������
	static void						InitCharacterParam(LPCHRPARAM pOut);	//�S���ڂ�������
	static void						CreateTransMatrix(Matrix& out, CRVector3 front,CRVector3 pos);
public:
	//�Q�b�^�[
	static CHARACTER_STATE_TYPE		GetStateType(LPCharacter C);	//���݂̍s���X�e�[�g�^�C�v�𓾂�
	static const CHRPARAM&			GetParam(LPCharacter C);		//�p�����[�^�[����
	static CRVector3				GetFront(LPCharacter C);		//�O�����x�N�g���𓾂�
	static Vector3					GetRight(LPCharacter C);		//�E�����x�N�g���𓾂�
	static float					GetAngle(LPCharacter C);		//�����Ă���Y���̊p�x�𓾂�(���K���ς�)
	static const LPCAMERA			GetCamera(LPCharacter C);		//�L�����N�^�̎����Ă���J�������𓾂�
	static LPCHARACTERMODELBASE		GetModelPtr(LPCharacter C);		//�L�����N�^�̃��f���|�C���^�𓾂�
	static LPCHARACTERWEPONBASE		GetWepon(LPCharacter C);		//�L�����N�^�̕���𓾂�
	static LPCHARACTER_UI			GetUI(LPCharacter C);
	static RATIO					GetChargePower(LPCharacter C);
	static void						GetBonePos(LPCHARACTER C, CHARACTER_BONE_TYPE bone_type, Vector3& out);
	static void						GetBoneLocalPos(LPCHARACTER C, CHARACTER_BONE_TYPE bone_type, Vector3& out);
public:
	//�֗��֐�����
	static void						GetTransMatrix(LPCharacter C, Matrix& out, CRVector3 scale);
	static Vector3					GetMyCameraFront(LPCharacter C);//�����̃J�����̑O�����x�N�g���𓾂�
	static Vector3					GetMyCameraRight(LPCharacter C);//�����̃J�����̉E�����x�N�g���𓾂�
public:
	//�_���[�W�֘A
	static void						CreateToBrainDamageMsg(LPCHARACTER	C, LPGameMessage msg, DamageParam* pDamage);
	static void						SetCureValue(LPCharacter C,float val);		//��
	static void						SetDamageValue(LPCharacter C, float val);	//�_���[�W
	static bool						isDead(LPCharacter C);	//����ł��邩�ǂ���
	static bool						isDying(LPCharacter C);	//�m�����ǂ���
	static bool						HitCheck(LPCharacter C, CRVector3 hitball, float hitballsize);	//�L�����N�^�Ƌ��̏Փ˔���
	static void						UpdateDamageLifePoint(LPCharacter C);	//��_���[�W���ԃJ�E���g�̍X�V�Ƒ̗͂̉�
public:
	//�쐬�Ə����֘A
	static bool						GetRandomRespornPoint(TEAM_TYPE team, LPMapObjectInterface* ppOut);
	static void						SetStartCharacter(LPCHARACTER C, LPCONTROLLER cnt);
	static void						ApperVendingMachine(LPCHARACTER C, CRVector3 pos, float angle);
	static LPCONTROLLOBJCT			RespornCharacter(LPCHARACTER C);	//�L�����N�^���X�|�[��
	static void						DeleteCharacter(LPCHARACTER C);		//�L�����N�^�𖕏�����
	static void						SetAutoAimPointEnable(LPCHARACTER C, bool flg);
public:
	static bool						HitCheckOtherControllObj(LPCHARACTER C);
public:
	//����֌W
	static bool						isHitColaGun(CRVector3 p);
	static bool						isHitColaGlenade(CRVector3 p);		//���݂�
	static bool						isShuffle(LPCharacter C);		//�U���Ă��邩�ǂ���
	static bool						isZoom(LPCharacter C);
	static bool						WeponFire(LPCHARACTER C);
	static float					GetMaxChargeWeaponRange(LPCHARACTER C);
	static float					GetMinChargeWeaponRange(LPCHARACTER C);
	static float					GetNowChargeRange(LPCHARACTER C);
	static void						SetGaugeMove(LPCHARACTER C,CRVector2 pos, CRVector2 size);
	static void						ResetGauge(LPCHARACTER C);
public:
	//���[���֘A
	static bool						isFrend(LPCHARACTER me, LPCHARACTER you);	//me�ɂƂ���you���������ǂ���
public:
	//���[�V�����֘A
	static void						ResetMotion(LPCHARACTER me, CHR_MOTION_PARTS parts);
	static void						SetMotion(LPCHARACTER me, CHR_MOTION_PARTS parts,CHR_MOTION_TYPE motion);
	static void						SetMotionSpeed(LPCHARACTER me, CHR_MOTION_PARTS parts, float speed);
	static bool						SetWalkingMotion(LPCHARACTER me, CRVector2 move_v, RATIO speed);
public:
	//�\�͒l�֌W
	static void						SetOutLineColorHP(LPCHARACTER me);
	static void						SendMoveSpeedUpMsg(ENTITY_ID to);
	static void						SendChargeSpeedUpMsg(ENTITY_ID to);
	static void						SetPlusAlpha(LPCHARACTER me, RATIO a);
	static void						SetPowerUpEffect(LPCHARACTER me, MSG_TYPE type);
	static void						UpdateUpValueAlpha(LPCHARACTER me);
public:
	static bool						PlayFootSound(LPCHARACTER me, CHARACTER_BONE_TYPE& out_touch_foot);
public:
	static void						SendFriendIconMsgToBrain(LPCHARACTER me, FriendMessageIcon::MsgType type);
public:
	//�`��֘A
	static void						RenderEye(LPCHARACTER me);
	static void						ApperEmptyCan(CRVector3 pos, float size, TEAM_TYPE type);
};


#endif