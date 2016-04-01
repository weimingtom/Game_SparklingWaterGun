#ifndef __FUZZYVARIABLE_H__
#define __FUZZYVARIABLE_H__

#include	 <map>
#include	<string>

class FuzzySet;
class FuzzyOperator;
class FuzzyModule;

class FuzzyVariable
{
	friend class FuzzyModule;
private :
	std::map<std::string, FuzzySet*> member_Sets;	//�t�@�W�[�W����FLV
	float													min_Range;		//�ϐ��̍ŏ��l
	float													max_Range;		//�ϐ��̍ő�l
private :
	//�R�s�[�����Ȃ�
	//FuzzyVariable(const FuzzyVariable&);
	FuzzyVariable& operator =(const FuzzyVariable&);
	/**
	*@brief		�V�����W�����ǉ������ƈ����ɍ��킹�ď���Ɖ����𒲐�����
	*@param1	�ǉ�����W���̏���l
	*@param2	�����l
	*/
	void	Adjust_Range(float min, float max);
	/**
	*@brief ���Ȃ�g���b�L�[(new�ł����C���X�^���X���쐬�ł��Ȃ��Ȃ���ۂ�)
	*/
	//~FuzzyVariable();

public:
	//FuzzyVariable(const FuzzyVariable&);
	~FuzzyVariable();
	FuzzyVariable() : max_Range(.0f), min_Range(.0f)
	{	}
	/**
	*@brief		�X�N���v�l���t�@�W�[������
	*@param	�t�@�W�[������X�N���v�l
	*/
	void			Fuzzify(float val);
	/**
	*@brief	max_av������p���Ĕ�t�@�W�[������
	*/
	float			Defizzify_MaxAv();
	/**
	*@brief �d�S�����Ŕ�t�@�W�[��
	*/
	float			Defizzify_Centroid(int	num_Samples);

	//********************************************************
	//�ȉ��̃��\�b�h�́A���\�b�h�̒��Ŗ��O��t����ꂽ�W���̃C���X�^���X
	//	���쐬���A�����member_Sets�ɒǉ�����
	//	�ǉ�����邲�Ƃ�max_Range��min_Range�͒��������
	//�߂�l�ɍ쐬�����t�@�W�[��Proxy��Ԃ�
	//********************************************************
	FuzzySet_Proxy Add_LeftShoulderSet(std::string name, float min, float peak, float max);
	FuzzySet_Proxy Add_RightShoulderSet(std::string name, float min, float peak, float max);
	FuzzySet_Proxy Add_TriangleSet(std::string name, float min, float peak, float max);

	
	void	Print_Console();
	
};

#endif
