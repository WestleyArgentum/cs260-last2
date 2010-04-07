/*
a useful random number generator that you can set an arbitrary seed to at any time you like.  you probably
won't need to change this class at all
*/

#pragma once
#include <stdlib.h>

class RandomNumberGenerator
{
public:
	RandomNumberGenerator(void);
	~RandomNumberGenerator(void);
	static void Seed(int seed);
	static int GetRandomNumber(int min, int max);

};
