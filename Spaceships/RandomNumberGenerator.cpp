#include "RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator(void)
{
}

RandomNumberGenerator::~RandomNumberGenerator(void)
{
}

void RandomNumberGenerator::Seed(int seed){
	srand(seed);
}

int RandomNumberGenerator::GetRandomNumber(int min, int max){
	int u = (int)((double)rand() / (RAND_MAX + 1) * (max - min)
            + min);
	return u;

}
