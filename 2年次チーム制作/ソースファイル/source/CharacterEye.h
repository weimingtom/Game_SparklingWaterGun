#ifndef __CHARACTER_EYE_H__
#define __CHARACTER_EYE_H__

#include "iextreme.h"

//�L�����N�^�̖ڃN���X

class CharacterEye
{
public:
	//�ڂ̏�ԕϐ�
	enum EYE_STATE
	{
		_OPEN = 0,	//�ʏ�
		_CLOSE,		//�u��
		_DAMAGE,	//��_���[�W��

		__STATE_MAX,
	};
private:
	EYE_STATE		m_State;					//���݂̏��
	int				m_Count;					//��Ԃ̌p������
	float			m_Scale;					//�ڂ̑傫��
	LPIEX2DOBJ		m_pTextures[__STATE_MAX];	//�ڂ̃e�N�X�`��
	LVERTEX			m_Vertexs[4];				//�`��p�ڂ̒��_
public:
	CharacterEye(
		float			Scale	//�ڂ̑傫��
		);
	~CharacterEye();
private:
	void UpdateOpenEye();
	void UpdateCloseEye();
	void UpdateDamageEye();
public:
	//�ڂ̃e�N�X�`�����Z�b�g����
	void SetEyeTexture(
		EYE_STATE		set_eye_state,	//�ݒ肷��ڂ̃^�C�v
		LPIEX2DOBJ		pTexture		//�e�N�X�`���ւ̃|�C���^
		);

	//���t���[���̍X�V
	void Update();

	//�ڂ̏�Ԃ�ύX
	void SetState(
		EYE_STATE next		//���̏��
		);

	//�`��
	void Render(
		CRVector3 right_eye,	//�E�ڍ��W
		CRVector3 left_eye,		//���ڍ��W
		CRVector3 front,		//��̌����x�N�g��(���K���ς݂ł��邱��)
		CRVector3 right,		//��̂悱�x�N�g��(���K���ς݂ł��邱��)
		CRVector3 up			//��̂����x�N�g��(���K���ς݂ł��邱��)
		);
};

#endif