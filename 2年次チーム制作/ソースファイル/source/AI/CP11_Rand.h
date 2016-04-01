#ifndef __CP11_RAND_H__
#define __CP11_RAND_H__
#include	<random>

class Rand
{
private:
	static UINT seed128[4];
	static std::mt19937 mt_Rand;

public :
	static void	Initialize();
	static UINT rand();

	static	int	Equal_DistanceNum(int min, int max)
	{
		std::uniform_int_distribution<int> dist(min, max);
		return dist(mt_Rand);
	}

};



#endif
