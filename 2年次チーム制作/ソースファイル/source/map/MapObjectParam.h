#ifndef __MAP_OBJECT_PARAM_H__
#define __MAP_OBJECT_PARAM_H__

#include "iextreme.h"
#include "../CODBOForwardDecl.h"

// �}�b�v�I�u�W�F�N�g��3D�p�����[�^
typedef struct ObjectParam
{
	Vector3 pos;
	Vector3 angle;
	Vector3 scale;
}
OBJECTPARAM, *LPOBJECTPARAM;

// �V�F�A�I�u�W�F�N�g�̃p�����[�^
struct ShareRatioParam
{
	struct Render
	{
		COLORf		NowColor;
		COLORf		DamageTeamColor;
	};
	RATIO            share_power;              //���̃I�u�W�F�N�g�̉e����
	float               share_value;               //���݂̑���x
	float               max_share_value;      //�ő����x
	TEAM_TYPE   team_type;                 //�`�[���^�C�v
	Render           render_param;
	int                  NotDamageCount;
};

#endif // __MAP_OBJECT_PARAM_H__

// EOF