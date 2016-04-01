#ifndef __FUZZYMODULE_H__
#define __FUZZYMODULE_H__

#include <map>
#include	 <vector>
#include <string>

class FuzzyVariable;
class FuzzyRule;
class FuzzyOperator;

namespace defuzzify_alg
{
	enum defuzzify
	{
		max_av,
		centroid,	//�d�S����
	};
}
typedef defuzzify_alg::defuzzify DEFUZZIFY_ALG;

class FuzzyModule
{

private :
	std::map<std::string, FuzzyVariable*>		 variable_Map;	//�t�@�W�[�ϐ���map
	std::vector<FuzzyRule*>							rule_vecter;		//�t�@�W�[���[����container�i�t�@�W�[���[���͔��Ώ����̏W���́j

public :
	FuzzyModule();
	~FuzzyModule();
	/**
	*@brief �����FLV���쐬����
	*/
	FuzzyVariable&		Create_EnptyFLV(const std::string& var_Name);
	/**
	*@brief			�l���t�@�W�[������
	*@param1		�g�p����FLV�̖��O
	*@param2		�t�@�W�[������FLV
	*/
	void	Fuzzify(const std::string& use_Name, float val);
	/**
	*@brief		�t�@�W�[�����ꂽ�l���X�N���v�l�ɖ߂�
	*@param1	�X�N���v������t�@�W�[�W���ϐ��̖��O
	*@param2	�X�N���v���̃A���S���Y��
	*@return	�X�N���v�����ꂽ�l
	*/
	float	Defuzzify(const std::string& key, DEFUZZIFY_ALG alg = defuzzify_alg::max_av);
	/**
	*@brief		�t�@�W�[���[����ǉ�����
	*@param1	�O����(IF)
	*@param2 �㌏��(THEN)
	*/
	void	Add_Rule(FuzzyOperator& antecedent, FuzzyOperator& consequence);
	/**
	*@brief ���݂̃��[���̌㌏�������Z�b�g����
	*/
	void	Reset_Confidences_of_Consequents();

	void	Print();
};

#endif
