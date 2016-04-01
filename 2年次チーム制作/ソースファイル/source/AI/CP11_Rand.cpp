#include	"..\All.h"

UINT	Rand::seed128[4] = { 123456789, 362436069, 521288629, 88675123 };
std::mt19937 Rand::mt_Rand;

void	Rand::Initialize()
{
	std::random_device rd;
	for (UINT i = 0; i < 4; i++)
	{
		ULONG r = rd();
		seed128[i] = r = 1812433253U * (r ^ (r >> 30)) + i;
	}

	mt_Rand.seed(rd());
}

UINT	Rand::rand()
{
	UINT* p = seed128;
	UINT t = (p[0] ^ (p[0] << 11));
	p[0] = p[1];	p[1] = p[2];	p[2] = p[3];
	return (p[3] = (p[3] ^ (p[3] >> 19)) ^ (t ^ (t >> 8)));
}