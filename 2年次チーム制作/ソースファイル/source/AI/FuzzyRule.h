#ifndef __FUZZYRULE_H__
#define __FUZZYRULE_H__

class FuzzyOperator;

class FuzzyRule
{
private :
	FuzzyOperator*	antecedent;		//�O�����iIF�j
	FuzzyOperator*	consequence;	//�㌏���iTHEN�j

	//�R�s�[�����Ȃ�
	FuzzyRule(const FuzzyRule&);
	FuzzyRule& operator = (const FuzzyRule&);

public :
	FuzzyRule(FuzzyOperator& ant, FuzzyOperator& con);
	~FuzzyRule();
	void	Consequence_Zero();
	/**
	*@brief �����o�̃��[���Ɋ�Â���DOM���X�V����
	*/
	void	Calculate();
};
#endif
