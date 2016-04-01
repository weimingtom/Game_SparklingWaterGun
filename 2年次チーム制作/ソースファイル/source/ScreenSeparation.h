#ifndef __SCREEN_SEPARATION_H__
#define __SCREEN_SEPARATION_H__

#include <list>
#include "Camera.h"
#include "CODBOForwardDecl.h"

class MatchTypeStateBase;
typedef MatchTypeStateBase *LPMatchTypeStateBase;
class ShadowMap;
class ControllObject;
class ScreenCreater_Base;
//**********************************************************************
//	��ʕ����`���S������N���X(��ʈ�ł��`�悷�邯�ǂ�)
//**********************************************************************

class ScreenSeparation
{
public:
private:
	enum{
		RENDER_SCREEN_MAX = 9		//�����ɕ`��ł����ʂ̍ő吔
	};
private:
	int										num_Player;
	iex2DObj**							m_pScreenTextures;	//��ʃe�N�X�`������
	std::list<RECTI>					m_ScreenInfo;															//�e�X�N���[�����ǂ��ɕ`�悷�邩
	iexView									m_view;																		//matView�Ƃ���ݒ肷��̂�
	RECTI									m_screen_texture_size;											//���̃X�N���[���̃T�C�Y
	ScreenCreater_Base*			m_screen_create;

public:
	ScreenSeparation(const int& num_P, ScreenCreater_Base* creater);
	~ScreenSeparation();
public:
	//�`�悷��X�N���[����ݒ肷��
	void SetScreen(const  std::list<RECTI>&	screen_positions);

	//��ʃe�N�X�`���쐬
	void CreateScreenTexture(
		CONTROLLOBJLIST&		obj_list,				//�J�������X�g�����ɓ����Ă��镪������ʂ�\������)
		LPMatchTypeStateBase		pDrawMatchState		//�`�悷��֐��̓������}�b�`�X�e�[�g�N���X�̃|�C���^�[
		);

	//��ʂ�`��
	void DrawScreen();

};
void CreateScreenRect(std::list<RECTI>* pOut, int nPlayer);

#endif