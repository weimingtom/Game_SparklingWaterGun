#include	"..\All.h"

FuzzyVariable::~FuzzyVariable()
{
	for (auto it = member_Sets.begin(); it != member_Sets.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
}

void	FuzzyVariable::Adjust_Range(float min, float max)
{
	if (min_Range > min)
		min_Range = min;
	if (max_Range < max)
		max_Range = max;
}

void	FuzzyVariable::Fuzzify(float val)
{
	if (val > max_Range || val < min_Range)
	{
		//エラー処理
	}
	for (auto it = member_Sets.begin(); it != member_Sets.end(); it++)
	{
		it->second->Clear_DOM();
		it->second->Set_DOM(it->second->Calculate_DOM(val));
	}
}

float	FuzzyVariable::Defizzify_MaxAv()
{
	return .0f;
}

float	FuzzyVariable::Defizzify_Centroid(int num_Sample)
{
	float step_Size = (max_Range - min_Range) / (float)(num_Sample);
	float total_Area = .0f;
	float sum_of_Moments = .0f;

	for (int samp = 1; samp <= num_Sample; ++samp)
	{
		for (auto cur_Set = member_Sets.begin(); cur_Set != member_Sets.end(); cur_Set++)
		{
			float contribution;
			float work1 = cur_Set->second->Calculate_DOM(min_Range + samp * step_Size);
			//ファジー推論から導き出された連想行列からの代表値
			float work2 = cur_Set->second->Get_DOM();

			//二つの値とのANDを取る
			contribution = (work1 <= work2) ? work1 : work2;
			
			total_Area += contribution;
			sum_of_Moments += (min_Range + samp * step_Size) * contribution;
		}
	}
	if (total_Area == .0f) return .0f;
	return (sum_of_Moments / total_Area);
}

FuzzySet_Proxy FuzzyVariable::Add_LeftShoulderSet(std::string name, float min, float peak, float max)
{
	member_Sets.insert(std::pair<std::string, FuzzySet*>(name, 
												new FuzzySet_LeftShoulder(peak,min, max,name)));
	Adjust_Range(min, max);
	//Proxyを作成して返却
	return FuzzySet_Proxy(*member_Sets[name]);
}

FuzzySet_Proxy FuzzyVariable::Add_RightShoulderSet(std::string name, float min, float peak, float max)
{
	member_Sets.insert(std::pair<std::string, FuzzySet_RightShoulder*>(name,
													new FuzzySet_RightShoulder(peak, min, max,name)));
	Adjust_Range(min, max);
	return FuzzySet_Proxy(*member_Sets[name]);

}

FuzzySet_Proxy FuzzyVariable::Add_TriangleSet(std::string name, float min, float peak, float max)
{
	member_Sets.insert(std::pair<std::string, FuzzySet_Triangle*>(name,
												new FuzzySet_Triangle(peak,min, max,name)));
	Adjust_Range(min, max);
	return FuzzySet_Proxy(*member_Sets[name]);
}

//void	FuzzyVariable::Print()
//{
//	for (auto it = member_Sets.begin(); it != member_Sets.end(); it++)
//	{
//		MyDebugMgrStr(dword_color::white, it->first.c_str());
//		it->second->Print();
//	}
//}

void	FuzzyVariable::Print_Console()
{
	for (std::map<std::string, FuzzySet*>::iterator it = member_Sets.begin();
		it != member_Sets.end();
		++it)
	{
		//printf("%s :  ", it->first.c_str());
		it->second->Print_Console();
	}
}
