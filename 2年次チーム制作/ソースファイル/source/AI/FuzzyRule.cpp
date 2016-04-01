#include	"..\All.h"

FuzzyRule::FuzzyRule(FuzzyOperator& ant,FuzzyOperator& con) :
antecedent(ant.Clone()), consequence(con.Clone())
{	}

FuzzyRule::~FuzzyRule()
{
	 SAFE_DELETE(antecedent);
	 SAFE_DELETE(consequence);
}

void	FuzzyRule::Consequence_Zero()
{
	consequence->Clear_DOM();
}

void	FuzzyRule::Calculate()
{
	consequence->ORwithDOM(antecedent->Get_DOM());
}