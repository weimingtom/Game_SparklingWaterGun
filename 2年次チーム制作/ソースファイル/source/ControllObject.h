#ifndef __CONTROLL_OBJECT_H__
#define __CONTROLL_OBJECT_H__

#include "GameSystem\GameBase.h"
#include "Camera.h"
#include "iextreme.h"
#include "TeamViewData.h"

//�Q�[�����̑���ł��镨�̎����Ă����{�I�ȃp�����[�^�[
typedef struct tagControllObjectParam
{
	int			team_type;
	Vector3		pos;	//���W
	bool		doexplode_blur;
	Vector2		explode_pos;
	float		blur_time;
	RATIO		blur_power;
	bool		do_water_drop;
}
ControllObjectParam, *LPCONTROLLOBJECTPARAM;

//�Q�[�����̑���ł��镨�̊��N���X
class ControllObject:public GameBaseEntity
{
protected:
	LPPLAYERMATCHDATA		m_pMyMatchData;	//�����̃}�b�`�f�[�^�ւ̃|�C���^
	ControllObjectParam		m_base_param;	//��{�I�ȃp�����[�^�[
	Camera					m_camera;		//���̃I�u�W�F�N�g�𑀍삵�Ă���Ƃ��̃J����
	TeamData*				m_pTeamData;	//�����`�[���f�[�^�ւ̃|�C���^
public:
	ControllObject(ENTITY_ID id, int team_type, TeamData* tData);
	virtual~ControllObject(){}
public:
	virtual Vector3 GetBodyCenter() = 0;
	virtual void Controll() = 0;	//����֐�
	virtual void Update(TeamData& teamData) = 0;		//���t���[���̍X�V
//	virtual void Render() = 0;		//�`��
	virtual void Render_ShadowMap(ControllObject& viewowner, TeamData& teamdata) = 0;
	virtual void Render(ControllObject& viewowner,TeamData& teamdata) = 0;		//�`��
	virtual void RenderUI(ControllObject& viewowner, TeamData& teamdata) = 0;	//UI�̕`��
	virtual bool HandleMessage(LPGameMessage msg) = 0;	//���b�Z�[�W
};

#endif