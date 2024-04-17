#include "Rand.h"
#include <stdlib.h>
#include <time.h>
#include "Ease.h"

using namespace IFE;
using namespace std;

void IFERand::Initialize()
{
	srand((uint32_t)time(nullptr));
}

int32_t IFERand::GetRand()
{
	return rand();
}

int32_t IFERand::GetRand(int32_t min, int32_t max)
{
	return min + rand() % (max + 1 - min);
}

float IFE::IFERand::GetRandF(float min, float max, int32_t resolution)
{
	if (min == max)return 0;
	int32_t r = rand() % (resolution + 1);
	float a = Lerp(min, max, (float)resolution, (float)r);
	return a;
}
