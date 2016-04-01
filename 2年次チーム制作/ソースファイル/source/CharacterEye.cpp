#include "CharacterEye.h"



CharacterEye::CharacterEye(
	float			Scale					//�ڂ̑傫��
	):
	m_Scale(Scale),
	m_Count(0),
	m_State(_OPEN)
{
	//������
	for (int i = 0; i < __STATE_MAX; ++i)
	{
		m_pTextures[i] = nullptr;
	}
}



CharacterEye::~CharacterEye()
{

}


void CharacterEye::UpdateOpenEye()
{
	//�����ŏu����ԂɈڍs
	if (rand() % 20 == 0)
	{
		SetState(_CLOSE);
	}
}


void CharacterEye::UpdateCloseEye()
{
	//��莞�ԂŒʏ��Ԃ�
	if (m_Count++ > 5)
	{
		SetState(_OPEN);
	}
}

void CharacterEye::UpdateDamageEye()
{
	//�������Ȃ�

}



void CharacterEye::Update()
{
	//���t���[���̍X�V(�ڂ̏�ԍX�V)
	
	void(CharacterEye::*functions[])()=
	{
		&CharacterEye::UpdateOpenEye,
		&CharacterEye::UpdateCloseEye,
		&CharacterEye::UpdateDamageEye
	};

	//�X�V
	(this->*functions[m_State])();
}

//�ڂ̃e�N�X�`�����Z�b�g����
void CharacterEye::SetEyeTexture(
	EYE_STATE		set_eye_state,	//�ݒ肷��ڂ̃^�C�v
	LPIEX2DOBJ		pTexture		//�e�N�X�`���ւ̃|�C���^
	)
{
	MyAssert(
		(int)set_eye_state >= 0 &&
		(int)set_eye_state < __STATE_MAX,
		"���݂��Ȃ��ڂ̃^�C�v�Ƀe�N�X�`�����ݒ肳��܂��� type=%d ",
		(int)set_eye_state);

	m_pTextures[set_eye_state] = pTexture;
}

//�ڂ̏�Ԃ�ύX
void CharacterEye::SetState(
	EYE_STATE next		//���̏��
	)
{
	m_State = next;
	m_Count = 0;
}


//�`��
void CharacterEye::Render(
	CRVector3 right_eye,	//�E�ڍ��W
	CRVector3 left_eye,		//���ڍ��W
	CRVector3 front,		//��̌����x�N�g��(���K���ς݂ł��邱��)
	CRVector3 right,		//��̂悱�x�N�g��(���K���ς݂ł��邱��)
	CRVector3 up			//��̂����x�N�g��(���K���ς݂ł��邱��)
	)
{
	LPIEX2DOBJ pTex = m_pTextures[m_State];
	Vector3	sUp(up), sFront(front), sRight(right);
	int i;

	sUp *= m_Scale;
	sFront *= m_Scale;
	sRight *= m_Scale;

	//���_�ɏ���ݒ�
	for (i = 0; i < 4; ++i)
	{
		m_Vertexs[i].color = 0xFFFFFFFF;
		m_Vertexs[i]._v = left_eye;
	}

	m_Vertexs[0].tu = 0;
	m_Vertexs[0].tv = 0;

	m_Vertexs[1].tu = 1;
	m_Vertexs[1].tv = 0;

	m_Vertexs[2].tu = 0;
	m_Vertexs[2].tv = 1;

	m_Vertexs[3].tu = 1;
	m_Vertexs[3].tv = 1;

	m_Vertexs[0]._v = -sRight + sUp + m_Vertexs[0]._v;
	m_Vertexs[1]._v = sRight + sUp + m_Vertexs[1]._v;
	m_Vertexs[2]._v = -sRight + -sUp + m_Vertexs[2]._v;
	m_Vertexs[3]._v = sRight + -sUp + m_Vertexs[3]._v;

	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//�`��
	iexPolygon::Render3D(m_Vertexs, 2, pTex, RS_COPY);


	//�E�ڂ̏��ݒ�
	for (i = 0; i < 4; ++i)
	{
		m_Vertexs[i].tu = 1 - m_Vertexs[i].tu;
		m_Vertexs[i]._v =
			-left_eye + m_Vertexs[i]._v + right_eye;
	}


	//�`��
	iexPolygon::Render3D(m_Vertexs, 2, pTex, RS_COPY);

	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}