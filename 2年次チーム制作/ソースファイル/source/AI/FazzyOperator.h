#ifndef __FAZZYOPERATOR_H__
#define __FAZZYOPERATOR_H__

#include	<vector>

#include	"FuzzySet.h"


class FuzzyOperator
{
public :
	virtual ~FuzzyOperator(){}

	virtual FuzzyOperator* Clone() = 0;

	virtual float	Get_DOM() = 0;

	virtual void	Clear_DOM() = 0;

	virtual void	ORwithDOM(float val) = 0;
};

//********************************************************
//		Proxy
//********************************************************
class FuzzySet_Proxy : public FuzzyOperator
{
private :
	FuzzySet& set;

public :
	FuzzySet_Proxy(FuzzySet & fs) : set(fs)
	{	}
	FuzzyOperator* Clone(){ return new FuzzySet_Proxy(*this); }
	float	Get_DOM(){ return set.Get_DOM(); }
	void	Clear_DOM(){ set.Clear_DOM(); }
	void	ORwithDOM(float val){ set.ORwithDOM(val); }
	const char*	Get_Name(){ return set.Get_Name(); }
};



//********************************************************
//		Hedges
//********************************************************

class  Fuzzy_Very : public FuzzyOperator
{
private :
	FuzzySet&	set;	//このクラスが所属？するファジー集合

	Fuzzy_Very(const Fuzzy_Very& obj) : set(obj.set)
	{	}
	Fuzzy_Very& operator = (const Fuzzy_Very&);

public :
	Fuzzy_Very(FuzzySet& fs) : set(fs)
	{	}

	void	Clear_DOM(){ set.Clear_DOM(); }
	void	ORwithDOM(float val){ set.ORwithDOM(val*val); }
	float	Get_DOM(){ return set.Get_DOM() * set.Get_DOM(); }
	FuzzyOperator* Clone(){ return new Fuzzy_Very(*this); }
};

class  Fuzzy_Fairly : public FuzzyOperator
{
private:
	FuzzySet&	set;	//このクラスが所属？するファジー集合

	Fuzzy_Fairly(const Fuzzy_Fairly& obj) : set(obj.set)
	{	}
	Fuzzy_Fairly& operator = (const Fuzzy_Fairly&);

public:
	Fuzzy_Fairly(FuzzySet& fs) : set(fs)
	{	}

	void	Clear_DOM(){ set.Clear_DOM(); }
	void	ORwithDOM(float val){ set.ORwithDOM(val*val); }
	float	Get_DOM(){ return set.Get_DOM() * set.Get_DOM(); }
	FuzzyOperator* Clone(){ return new Fuzzy_Fairly(*this); }
};

//********************************************************
//		Operators
//********************************************************

class Fuzzy_AND : public FuzzyOperator
{
private :
	//@<Fuzzy_Proxyのポインタに変えてもいいかも
	std::vector<FuzzyOperator*> child_Operators;	//演算を行うファジーProxyのベクタ（そのうち名前変更したほうがいいかも）

	Fuzzy_AND& operator = (const Fuzzy_AND&);

public :
	Fuzzy_AND(const Fuzzy_AND& obj);
	~Fuzzy_AND();

	Fuzzy_AND(FuzzyOperator& op1, FuzzyOperator& op2);
	Fuzzy_AND(FuzzyOperator& op1, FuzzyOperator& op2, FuzzyOperator& op3);
	Fuzzy_AND(FuzzyOperator& op1, FuzzyOperator& op2, FuzzyOperator& op3, FuzzyOperator& op4);

	float Get_DOM();
	void Clear_DOM();
	void ORwithDOM(float val);
	FuzzyOperator* Clone(){ return new Fuzzy_AND(*this); }

};

class Fuzzy_OR : public FuzzyOperator
{
private:
	std::vector<FuzzyOperator*> child_Operators;

	Fuzzy_OR& operator = (const Fuzzy_OR&);

public:
	Fuzzy_OR(const Fuzzy_OR& obj);
	~Fuzzy_OR();

	Fuzzy_OR(FuzzyOperator& op1, FuzzyOperator& op2);
	Fuzzy_OR(FuzzyOperator& op1, FuzzyOperator& op2, FuzzyOperator& op3);
	Fuzzy_OR(FuzzyOperator& op1, FuzzyOperator& op2, FuzzyOperator& op3, FuzzyOperator& op4);

	float Get_DOM();
	void Clear_DOM();
	void ORwithDOM(float val);
	FuzzyOperator* Clone(){ return new Fuzzy_OR(*this); }

};


#endif
