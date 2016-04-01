#include	"..\All.h"

float FuzzySet_Triangle::Calculate_DOM(float val)
{
	//�O�p�`�ɍ���Ȃ��ꍇ
	if (val > right_Offset || val < left_Offset)
		return .0f;
	
	//�O�p�`�̍����ɏ�������ꍇ
	if (val >= left_Offset && val <= peak_Point)
	{
		float grad = peak_Point - left_Offset;
		float ret = (float)((val - left_Offset) / (grad));
		return ret;
	}
	//�E���̏ꍇ
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
	//���������̏ꍇ
	if (val < left_Offset)
		return .0f;
	
	//�����璸�_�̊Ԃ̏ꍇ
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
	//�������璸�_�̊Ԃɂ���ꍇ
	if (val > left_Offset && val < peak_Point)
	{
		float grad = peak_Point - left_Offset;
		float ret = (float)((val - left_Offset) / grad);
		return ret;
	}
	//�s�[�N���E�ȏꍇ
	if (val >= peak_Point)
		return 1.0f;

	return .0f;
}