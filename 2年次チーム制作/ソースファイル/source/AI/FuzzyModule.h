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
		centroid,	//重心方式
	};
}
typedef defuzzify_alg::defuzzify DEFUZZIFY_ALG;

class FuzzyModule
{

private :
	std::map<std::string, FuzzyVariable*>		 variable_Map;	//ファジー変数のmap
	std::vector<FuzzyRule*>							rule_vecter;		//ファジールールのcontainer（ファジールールは発火条件の集合体）

public :
	FuzzyModule();
	~FuzzyModule();
	/**
	*@brief からのFLVを作成する
	*/
	FuzzyVariable&		Create_EnptyFLV(const std::string& var_Name);
	/**
	*@brief			値をファジー化する
	*@param1		使用するFLVの名前
	*@param2		ファジー化するFLV
	*/
	void	Fuzzify(const std::string& use_Name, float val);
	/**
	*@brief		ファジー化された値をスクリプ値に戻す
	*@param1	スクリプ化するファジー集合変数の名前
	*@param2	スクリプ化のアルゴリズム
	*@return	スクリプ化された値
	*/
	float	Defuzzify(const std::string& key, DEFUZZIFY_ALG alg = defuzzify_alg::max_av);
	/**
	*@brief		ファジールールを追加する
	*@param1	前件部(IF)
	*@param2 後件部(THEN)
	*/
	void	Add_Rule(FuzzyOperator& antecedent, FuzzyOperator& consequence);
	/**
	*@brief 現在のルールの後件部をリセットする
	*/
	void	Reset_Confidences_of_Consequents();

	void	Print();
};

#endif
