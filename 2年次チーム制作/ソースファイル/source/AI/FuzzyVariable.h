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
	std::map<std::string, FuzzySet*> member_Sets;	//ファジー集合のFLV
	float													min_Range;		//変数の最少値
	float													max_Range;		//変数の最大値
private :
	//コピーさせない
	//FuzzyVariable(const FuzzyVariable&);
	FuzzyVariable& operator =(const FuzzyVariable&);
	/**
	*@brief		新しい集合が追加されると引数に合わせて上限と下限を調整する
	*@param1	追加する集合の上限値
	*@param2	下限値
	*/
	void	Adjust_Range(float min, float max);
	/**
	*@brief かなりトリッキー(newでしかインスタンスが作成できなくなるっぽい)
	*/
	//~FuzzyVariable();

public:
	//FuzzyVariable(const FuzzyVariable&);
	~FuzzyVariable();
	FuzzyVariable() : max_Range(.0f), min_Range(.0f)
	{	}
	/**
	*@brief		スクリプ値をファジー化する
	*@param	ファジー化するスクリプ値
	*/
	void			Fuzzify(float val);
	/**
	*@brief	max_av方式を用いて非ファジー化する
	*/
	float			Defizzify_MaxAv();
	/**
	*@brief 重心方式で非ファジー化
	*/
	float			Defizzify_Centroid(int	num_Samples);

	//********************************************************
	//以下のメソッドは、メソッドの中で名前を付けられた集合のインスタンス
	//	を作成し、それをmember_Setsに追加する
	//	追加されるごとにmax_Rangeとmin_Rangeは調整される
	//戻り値に作成したファジーのProxyを返す
	//********************************************************
	FuzzySet_Proxy Add_LeftShoulderSet(std::string name, float min, float peak, float max);
	FuzzySet_Proxy Add_RightShoulderSet(std::string name, float min, float peak, float max);
	FuzzySet_Proxy Add_TriangleSet(std::string name, float min, float peak, float max);

	
	void	Print_Console();
	
};

#endif
