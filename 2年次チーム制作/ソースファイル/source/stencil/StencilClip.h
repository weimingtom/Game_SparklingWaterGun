#ifndef __STENCILCLIP_H__
#define __STENCILCLIP_H__

#include <d3d9.h>

class StencilClip 
{
public:
	// �}�X�N�F
	enum MaskColor {
		MaskColor_Trans = 0x00,	//! �����F
		MaskColor_Fill = 0x01,	//! �h��Ԃ�
		MaskColor_None = 0xff  //! �����J���[
	};

	// ���[�h
	enum Mode {
		Mode_Masking,	//! �}�X�N�������݃��[�h��
		Mode_Draw		//! �`�惂�[�h��
	};
protected:
	Mode				mode_;			//!< ���[�h
	MaskColor			writeColor_;	//!< �}�X�N�������ݎ��̐F
	MaskColor			refColor_;		//!< �`�掞�̃}�X�L���O�F
	IDirect3DDevice9*	device_;		//!< �`��f�o�C�X
	DWORD				curZTest_;		//!< ������Z�e�X�g�̗L�� 
	DWORD				curZFunc_;		//!< ������Z�e�X�g��r�֐�

public:
	StencilClip();
	virtual ~StencilClip();

	//! �N���b�s���O�̈�쐬�J�n
	virtual bool regionBegin(IDirect3DDevice9* device, MaskColor clearColor = MaskColor_Trans);

	//! �N���b�s���O�̈�쐬�I��
	virtual bool regionEnd();

	//! �N���b�s���O�`��J�n
	virtual bool drawBegin();

	//! �N���b�s���O�`��I��
	virtual bool drawEnd(MaskColor clearColor = MaskColor_Trans);

	//! �������ݎ��}�X�N�J���[�̐ݒ�
	void setWriteMaskColor(MaskColor color);

	//! �`�掞�̃}�X�N�J���[�̎w��
	void setRefMaskColor(MaskColor color);
};

#endif