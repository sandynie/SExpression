#ifndef _CRANDOM_H
#define _CRANDOM_H
#include <time.h>
class ccRandomGenerator
{
public:
	ccRandomGenerator(int seed =0): m_CurVal(seed){}

	void ReSet(int seed)
	{
		m_CurVal = seed;
	}

	void ReSet()
	{
		int iseed = clock();
		ReSet(iseed);
	}
	//产生 0-32767 之间的随机数 2的16次方-1
	int Get()
	{
		return (((m_CurVal = m_CurVal * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	// random number between 0.0 and 1.0
	float GetFloat()
	{
		return float(Get())*(1.0f/32767.0f);
	};

	int Get( int minvalue, int maxvalue )
	{
		return (Get()%(maxvalue-minvalue+1)) + minvalue;
	}

	float GetFloat( float minvalue, float maxvalue )
	{
		return GetFloat()*(maxvalue-minvalue) + minvalue;
	}
protected:
private:
	int m_CurVal;
};
#endif
