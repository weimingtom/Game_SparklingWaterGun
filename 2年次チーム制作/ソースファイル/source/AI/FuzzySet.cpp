#include	"..\All.h"

float FuzzySet_Triangle::Calculate_DOM(float val)
{
	//三角形に合わない場合
	if (val > right_Offset || val < left_Offset)
		return .0f;
	
	//三角形の左側に所属する場合
	if (val >= left_Offset && val <= peak_Point)
	{
		float grad = peak_Point - left_Offset;
		float ret = (float)((val - left_Offset) / (grad));
		return ret;
	}
	//右側の場合
	if (val > peak_Point && val <= right_Offset)
	{
		float grad = right_Offset - peak_Point;
		float ret = 1.0f - (float)(((val - peak_Point) / grad));
		return ret;
	}
	return .0f;
}

float FuzzySet_LeftShoulder::Calculate_DOM(float val)
{
	//左よりも左の場合
	if (val < left_Offset)
		return .0f;
	
	//左から頂点の間の場合
	if (val >= left_Offset && val <= peak_Point)
		return 1.0f;

	if (val > peak_Point && val <= right_Offset)
	{
		float grad = right_Offset - peak_Point;
		float ret = 1.0f - (float)(((val - peak_Point) / grad));
		return ret;
	}

	return .0f;
}

float FuzzySet_RightShoulder::Calculate_DOM(float val)
{
	//左側から頂点の間にある場合
	if (val > left_Offset && val < peak_Point)
	{
		float grad = peak_Point - left_Offset;
		float ret = (float)((val - left_Offset) / grad);
		return ret;
	}
	//ピークより右な場合
	if (val >= peak_Point)
		return 1.0f;

	return .0f;
}