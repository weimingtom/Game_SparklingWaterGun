#ifndef __CHARACTER_MODEL_H__
#define __CHARACTER_MODEL_H__

//#include "CharacterMeshManager.h"
//
//class ShadowMap;
//
//class RenderParam
//{
//public:
//
//	bool	shadow_Flg;		//�e�����邩�ǂ���	
//	bool	wall_Hack;		//��������Ă��邩
//	float	outline_Size;	//�֊s���̃T�C�Y
//	union
//	{
//		float color[4];
//		struct
//		{
//			float outcolor_R, outcolor_G, outcolor_B, outcolor_A;
//		};
//	};
//	RenderParam()
//	{
//		shadow_Flg = true;
//		wall_Hack = false;
//		outline_Size = 1.0f;
//		outcolor_A = 1.0f;
//		outcolor_B = outcolor_G = outcolor_R = 1.0f;
//	}
//};

//**********************************************************************************
//		�{�`�������f���w�b�_�i���[�V�����u�����h�ƕ��ʃ��[�V��������ver�j
//**********************************************************************************
//
//class CharacterModel :public CharacterModelBase
//{
//
//public:
//	//���f����`��
//	void Render(LPCHARACTER C);
//	void Render(LPCHARACTER C,char* shaderName);
//
//	const Matrix& GetMatrix()const
//	{
//		return Matrix();
//	}
//
//	//�A�j���[�V����������
//	void Animation(
//		CHR_MOTION_PARTS parts,//�A�j���[�V���������镔��
//		float frame);	//�o�߃t���[��
//
//	//�A�j���[�V����������t���[�����Z�b�g
//	void SetAnimationFrame(
//		CHR_MOTION_PARTS parts,//�A�j���[�V�������Z�b�g���镔��
//		float setframe);	//�o�߃t���[��
//
//	//���[�V�������Z�b�g
//	void SetMotion(
//		CHR_MOTION_PARTS parts,	//���[�V�������Z�b�g����̂̕���
//		CHR_MOTION_TYPE motion);//���[�V�����^�C�v
//
//	//�u�����h���[�V�������Z�b�g
//	void SetBlendMotion(
//		CHR_MOTION_PARTS parts,	//���[�V�������Z�b�g����̂̕���
//		CHR_MOTION_TYPE motion1,//���[�V�����P
//		CHR_MOTION_TYPE motion2,//���[�V�����Q
//		RATE			rate	//���[�V����1�ɂQ���ǂꂾ���u�����h���邩
//		);
//};

#endif