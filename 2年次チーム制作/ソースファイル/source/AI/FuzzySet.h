#ifndef __FUZZYSET_H__
#define __FUZZYSET_H__

#include	"itDebugStringManager.h"

class FuzzySet
{
protected :
	float	dom;								//���̃t�@�W�[�W���̃����o�[�V�b�v�x
	float	representaive_Val;		//���̏W���̃����o�[�V�b�v�֐��̍ő�l
	std::string	name;						//���̏W���̖��O
public: 
	FuzzySet(const float& rep_Val,std::string& name) : name(name),dom(.0f), representaive_Val(rep_Val){}
	/**
	*@brief		�^����ꂽ�W���ɑ΂��郁���o�[�V�b�v�x��Ԃ�
	*@param1	�����o�[�V�b�v�x�ɕϊ�����X�N���v�l
	*@return	�����o�[�V�b�v�x
	*/
	virtual float	Calculate_DOM(float val) = 0;
	/**
	*@brief val > dom��OR���Z���s���Ă���i��\�l�j
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
//		�������炻�ꂼ��̃t�@�W�[�W���̃N���X
//********************************************************

//********************************************************
//												*
//											*		*
//										*				*
//********************************************************
class FuzzySet_Triangle : public FuzzySet
{
private :
	float	peak_Point;	//�O�p�n�̒��_
	float	left_Offset;	//�����̃I�t�Z�b�g
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
