#include	"..\All.h"

//********************************************************
//		AND
//********************************************************
Fuzzy_AND::~Fuzzy_AND()
{
	for (auto it = child_Operators.begin(); it != child_Operators.end(); it++)
	{
		SAFE_DELETE((*it));
	}
}
Fuzzy_AND::Fuzzy_AND(const Fuzzy_AND& obj)
{
	for (auto it = obj.child_Operators.begin(); it != obj.child_Operators.end(); it++)
	{
		child_Operators.push_back((*it)->Clone());
	}
}

Fuzzy_AND::Fuzzy_AND(FuzzyOperator& op1, FuzzyOperator& op2)
{
	child_Operators.push_back(op1.Clone());
	child_Operators.push_back(op2.Clone());

}

Fuzzy_AND::Fuzzy_AND(FuzzyOperator& op1, FuzzyOperator& op2, FuzzyOperator& op3)
{
	child_Operators.push_back(op1.Clone());
	child_Operators.push_back(op2.Clone());
	child_Operators.push_back(op3.Clone());
}

Fuzzy_AND::Fuzzy_AND(FuzzyOperator& op1, FuzzyOperator& op2, FuzzyOperator& op3, FuzzyOperator& op4)
{
	child_Operators.push_back(op1.Clone());
	child_Operators.push_back(op2.Clone());
	child_Operators.push_back(op3.Clone());
	child_Operators.push_back(op4.Clone());
}

float Fuzzy_AND::Get_DOM()
{
	float ret = FLT_MAX;
	
	for (auto it = child_Operators.begin(); it != child_Operators.end(); it++)
	{
		//AND‚Í
		float work = (*it)->Get_DOM();
		if (work < ret)
			ret = work;
	}
	return ret;
}

void	Fuzzy_AND::Clear_DOM()
{
	for (auto it = child_Operators.begin(); it != child_Operators.end(); it++)
	{
		(*it)->Clear_DOM();
	}
}

void	Fuzzy_AND::ORwithDOM(float val)
{
	for (auto it = child_Operators.begin(); it != child_Operators.end(); it++)
	{
		(*it)->ORwithDOM(val);
	}
}

//********************************************************
//		OR
//********************************************************
Fuzzy_OR::~Fuzzy_OR()
{
	for (auto it = child_Operators.begin(); it != child_Operators.end(); it++)
	{
		SAFE_DELETE((*it));
	}
}

Fuzzy_OR::Fuzzy_OR(const Fuzzy_OR& obj)
{
	for (auto it = obj.child_Operators.begin(); it != obj.child_Operators.end(); it++)
	{
		child_Operators.push_back((*it)->Clone());
	}
}

Fuzzy_OR::Fuzzy_OR(FuzzyOperator& op1, FuzzyOperator& op2)
{
	child_Operators.push_back(op1.Clone());
	child_Operators.push_back(op2.Clone());
}

Fuzzy_OR::Fuzzy_OR(FuzzyOperator& op1, FuzzyOperator& op2, FuzzyOperator& op3)
{
	child_Operators.push_back(op1.Clone());
	child_Operators.push_back(op2.Clone());
	child_Operators.push_back(op3.Clone());
}

Fuzzy_OR::Fuzzy_OR(FuzzyOperator& op1, FuzzyOperator& op2, FuzzyOperator& op3, FuzzyOperator& op4)
{
	child_Operators.push_back(op1.Clone());
	child_Operators.push_back(op2.Clone());
	child_Operators.push_back(op3.Clone());
	child_Operators.push_back(op4.Clone());
}

float Fuzzy_OR::Get_DOM()
{
	float ret = .0f;
	for (auto it = child_Operators.begin(); it != child_Operators.end(); it++)
	{
		float work = (*it)->Get_DOM();
		//OR‰‰ŽZ‚Í‘å‚«‚¢•û‚ð•Ô‚·
		if (work >= ret)
			ret = work;
	}
	return ret;
}

void	Fuzzy_OR::Clear_DOM()
{
	for (auto it = child_Operators.begin(); it != child_Operators.end(); it++)
	{
		(*it)->Clear_DOM();
	}
}

void	Fuzzy_OR::ORwithDOM(float val)
{
	for (auto it = child_Operators.begin(); it != child_Operators.end(); it++)
	{
		(*it)->ORwithDOM(val);
	}
}