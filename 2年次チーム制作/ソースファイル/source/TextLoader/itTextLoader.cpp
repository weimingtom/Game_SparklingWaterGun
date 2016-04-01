#include "itTextLoader.h"
#include <cstdio>
#include <cstdlib>

#define isEof(i) (i>=m_file_size-1)
#define isSpace(c) (c==' ')
#define isNewline(c) (c=='\n')
#define intc_size 10
#define floatc_size 10

//�ʏ�R���X�g���N�^
itTextLoader::itTextLoader():
m_pBuf(nullptr),
m_index(0),
m_file_size(0)
{

}


itTextLoader::itTextLoader(const char *file_name):
m_index(0)
{//�������Ɠ����Ƀt�@�C�����J��
	fileOpen(file_name);
}

//�f�X�g���N�^
itTextLoader::~itTextLoader()
{
	//�o�b�t�@���
	bufferRelease();
}

//�t�@�C���ۑ��̈�����
void itTextLoader::bufferRelease()
{
	if (m_pBuf)
	{
		delete[]m_pBuf;
		m_pBuf = nullptr;
		m_index = 0;
		m_file_size = 0;
	}
}

//�󔒓ǂݔ�΂�
bool itTextLoader::skipSpace()
{
	while (!isEof(m_index))
	{
		if (m_pBuf[m_index] != ' '	//�󔒂������͉��s���X�L�b�v
			&&m_pBuf[m_index] != '\n')return true;
		++m_index;
	}
	return false;
}

//�t�@�C���I�[�v��(�J���Ȃ������ꍇ��false��Ԃ�)
bool itTextLoader::fileOpen(const char *file_name)
{
	FILE* fp;

	if (fopen_s(&fp, file_name, "r"))
		return false;//�G���[�̏ꍇ

	//�ꉞ������Ă���
	bufferRelease();

	//�t�@�C���T�C�Y���擾
	fseek(fp, (long)0, SEEK_END);
	m_file_size = ftell(fp);

	//�V�[�N��擪��
	fseek(fp, 0, SEEK_SET);

	//�o�b�t�@�쐬
	m_pBuf = new char[m_file_size];

	//�t�@�C���ǂݍ���
	fread_s(
		(void*)m_pBuf,
		(size_t)m_file_size,
		sizeof(char),
		(size_t)m_file_size,
		fp);

	//�t�@�C���|�C���^�����
	fclose(fp);

	return true;
}

//���݃t�@�C�����J���Ă��邩�ǂ���(�J���Ă����ꍇ��true��Ԃ�)
bool itTextLoader::isOpen()
{
	return m_pBuf != nullptr;
}

//���݂�index�ʒu���t�@�C���̏I�[�̏ꍇtrue��Ԃ�(�J���Ă��Ȃ������ꍇ��true��Ԃ�)
bool itTextLoader::isFileEnd()
{
	return isEof(m_index);
}


//���݂�index�ʒu���������������܂�index���ړ�(�t�@�C���I�[�܂Ŋ֐���true��Ԃ��Ȃ������ꍇ�͂��̊֐���false��Ԃ�)
bool itTextLoader::findStr(const char *str)
{
	int stri, bufi, savei = m_index;
	bool flg;

	while (!isEof(m_index))
	{
		flg = true;
		stri = 0;
		bufi = m_index;

		while (str[stri] != '\0')	//����������̏I�[�܂Ń��[�v
		{
			if (m_pBuf[bufi++] != str[stri++]||	//������v���Ȃ������烋�[�v�𔲂���
				isEof(bufi))
			{
				flg = false;
				break;
			}
		}
		if (flg)	//�t���O��true�Ȃ猟�������Ȃ̂�index���ړ�
		{
			m_index = bufi;
			return true;
		}
		++m_index;
	}

	//�������s�Ȃ���Ƃ̃C���f�b�N�X�ɂ��ǂ�
	m_index = savei;

	return false;
}


void itTextLoader::loadInt(int& out)		
{
	//int�^�ǂݍ���
	char t[intc_size];	
	loadStr(t, intc_size);
	out = atoi(t);
}

void itTextLoader::loadFloat(float& out)
{
	//float�^�ǂݍ���
	char t[floatc_size];
	loadStr(t, floatc_size);
	out = (float)atof(t);
}

void itTextLoader::loadStr(char* out, int out_size)
{
	//char�^������ǂݍ���(�T�C�Y�����܂肫��Ȃ��ꍇ�͓r���őł��؂���\0���I�[�ɃZ�b�g)
	skipSpace();	//�󔒂��΂�
	int i;
	--out_size;

	for (i = 0; i < out_size; i++)
	{
		out[i] = m_pBuf[m_index++];

		char temp = m_pBuf[m_index];

		if (isSpace(m_pBuf[m_index]) ||
			isNewline(m_pBuf[m_index]) ||
			isEof(m_index))
		{
			break;
		}
	}

	out[i + 1] = '\0';
}