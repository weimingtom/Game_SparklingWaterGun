#include	"..\All.h"

FuzzyModule::FuzzyModule()
{
	variable_Map.clear();
}

FuzzyModule::~FuzzyModule()
{
	for (auto it = variable_Map.begin(); it != variable_Map.end(); it++)
	{
		SAFE_DELETE(it->second);
	}

	for (auto it = rule_vecter.begin(); it != rule_vecter.end(); it++)
	{
		SAFE_DELETE((*it));
	}
}

void	FuzzyModule::Add_Rule(FuzzyOperator& antecedent, FuzzyOperator& consequence)
{
	rule_vecter.push_back(new FuzzyRule(antecedent, consequence));
}

FuzzyVariable&	 FuzzyModule::Create_EnptyFLV(const std::string& var_Name)
{
	variable_Map.insert(std::pair<std::string, FuzzyVariable*>(var_Name,new FuzzyVariable()));
	return *variable_Map[var_Name];
}

void	FuzzyModule::Fuzzify(const std::string& use_Name, float val)
{
	auto it = variable_Map.find(use_Name);
	if (it == variable_Map.end())
	{
		return;
	}

	variable_Map[use_Name]->Fuzzify(val);
}

float	FuzzyModule::Defuzzify(const std::string& key, DEFUZZIFY_ALG alg)
{
	auto it = variable_Map.find(key);
	if (it == variable_Map.end())
	{
		return .0f;
	}
	//現在のルールに基づいて推論し結論を出す
	//後件部（THEN）をリセットする
	Reset_Confidences_of_Consequents();
	//それぞれのIF THENを計算する
	for (auto cur_Rule = rule_vecter.begin(); cur_Rule != rule_vecter.end(); cur_Rule++)
	{
		(*cur_Rule)->Calculate();
	}
	
	//結論を出す
	float ret =  variable_Map[key]->Defizzify_Centroid(10);
	
	//MyDebugMgrStr(dword_color::green, "Defizzify		%s",key.c_str());
	//MyDebugMgrStr(dword_color::white, "%5f", ret);
	return ret;
}

void	FuzzyModule::Print()
{
	for (auto it = variable_Map.begin(); it != variable_Map.end(); it++)
	{
		//MyDebugMgrStr(dword_color::red, it->first.c_str());
		it->second->Print_Console();
	}
}

void		FuzzyModule::Reset_Confidences_of_Consequents()
{
	for (auto it = rule_vecter.begin(); it != rule_vecter.end(); it++)
	{
		(*it)->Consequence_Zero();
	}
}