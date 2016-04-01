#ifndef __FUZZYRULE_H__
#define __FUZZYRULE_H__

class FuzzyOperator;

class FuzzyRule
{
private :
	FuzzyOperator*	antecedent;		//前件部（IF）
	FuzzyOperator*	consequence;	//後件部（THEN）

	//コピーさせない
	FuzzyRule(const FuzzyRule&);
	FuzzyRule& operator = (const FuzzyRule&);

public :
	FuzzyRule(FuzzyOperator& ant, FuzzyOperator& con);
	~FuzzyRule();
	void	Consequence_Zero();
	/**
	*@brief メンバのルールに基づいてDOMを更新する
	*/
	void	Calculate();
};
#endif
