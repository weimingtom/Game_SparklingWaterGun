#ifndef __IT_TEXT_LOADER_H__
#define __IT_TEXT_LOADER_H__

//******************************************************************
//		�e�L�X�g���[�_�[�N���X
//******************************************************************

class itTextLoader
{
private:
	char*			m_pBuf;					//�t�@�C���ۑ��̈�
	int				m_index;				//���݂̗v�f�ԍ�
	int				m_file_size;			//�t�@�C���T�C�Y
public:
	itTextLoader();							//�ʏ�R���X�g���N�^
	itTextLoader(const char *file_name);	//�������Ɠ����Ƀt�@�C�����J��

	~itTextLoader();						//�f�X�g���N�^
private:
	void bufferRelease();					//�t�@�C���ۑ��̈�����
	bool skipSpace();						//�󔒓ǂݔ�΂�
public:
	bool fileOpen(const char *file_name);	//�t�@�C���I�[�v��(�J���Ȃ������ꍇ��false��Ԃ�)
public:
	bool isOpen();							//���݃t�@�C�����J���Ă��邩�ǂ���(�J���Ă����ꍇ��true��Ԃ�)
	bool isFileEnd();						//���݂�index�ʒu���t�@�C���̏I�[�̏ꍇtrue��Ԃ�(�J���Ă��Ȃ������ꍇ��true��Ԃ�)
public:
	bool findStr(const char *str);			//���݂�index�ʒu��������̕�����̏I���܂�index���ړ�������(�t�@�C���I�[�܂Ŋ֐���true��Ԃ��Ȃ������ꍇ�͂��̊֐���false��Ԃ�)
public:
	void loadInt(int& out);					//int�^�ǂݍ���
	void loadFloat(float& out);				//float�^�ǂݍ���
	void loadStr(char* out, int out_size);	//char�^������ǂݍ���(�T�C�Y�����܂肫��Ȃ��ꍇ�͓r���őł��؂���\0���I�[�ɃZ�b�g)
};


#endif