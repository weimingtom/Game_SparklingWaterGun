#ifndef __FUZZYSET_H__
#define __FUZZYSET_H__

#include	"itDebugStringManager.h"

class FuzzySet
{
protected :
	float	dom;								//このファジー集合のメンバーシップ度
	float	representaive_Val;		//この集合のメンバーシップ関数の最大値
	std::string	name;						//この集合の名前
public: 
	FuzzySet(const float& rep_Val,std::string& name) : name(name),dom(.0f), representaive_Val(rep_Val){}
	/**
	*@brief		与えられた集合に対するメンバーシップ度を返す
	*@param1	メンバーシップ度に変換するスクリプ値
	*@return	メンバーシップ度
	*/
	virtual float	Calculate_DOM(float val) = 0;
	/**
	*@brief val > domはOR演算を行っている（代表値）
	*/
	void	ORwithDOM(float val){ if (val > dom) dom = val; }

	void	Clear_DOM()
	{
		dom = .0f; 
	}
	float	Get_Rep_Val()const
	{ 
		return representaive_Val; 
	}
	float	Get_DOM()
	{ 
		return dom; 
	}
	void	Print(){ /*MyDebugMgrStr(dword_color::yellow, "%5f", dom);*/ }
	
	const char*	Get_Name()
	{ 
		return name.c_str(); 
	}
	void	Set_DOM(float val)
	{
		dom = val; 
	}

	void	Print_Console()
	{
		printf("%s : ", name.c_str());
		printf("%5f\n", dom);
	}
};

//********************************************************
//		ここからそれぞれのファジー集合のクラス
//********************************************************

//********************************************************
//												*
//											*		*
//										*				*
//********************************************************
class FuzzySet_Triangle : public FuzzySet
{
private :
	float	peak_Point;	//三角系の頂点
	float	left_Offset;	//左側のオフセット
	float	right_Offset;

public :
	FuzzySet_Triangle(const float& peak, const float& left, const float&right, std::string& name) :
		FuzzySet(peak,name), peak_Point(peak), left_Offset(left), right_Offset(right)
	{	}

	float	Calculate_DOM(float val);

};

//********************************************************
//								****************
//							*						
//						*								
//********************************************************
class FuzzySet_RightShoulder : public FuzzySet
{
private :
	float	peak_Point;
	float	left_Offset;
	float	right_Offset;

public :
	FuzzySet_RightShoulder(const float& peak, const float& left, const float& right, std::string& name) :
		FuzzySet((peak + right) + peak / 2,name), peak_Point(peak), left_Offset(left), right_Offset(right)
	{	}
	float	Calculate_DOM(float val);
};
//********************************************************
//							***************
//														*
//															*
//********************************************************

class FuzzySet_LeftShoulder :public FuzzySet
{
private:
	float	peak_Point;
	float	left_Offset;
	float	right_Offset;

public:
	FuzzySet_LeftShoulder(const float& peak, const float& left, const float& right, std::string& name) :
		FuzzySet((peak - left) + peak / 2,name), peak_Point(peak), left_Offset(left), right_Offset(right)
	{	}
	float	Calculate_DOM(float val);
};
#endif
