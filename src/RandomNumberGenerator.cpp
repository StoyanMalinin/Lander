#ifndef RANDOMNUMBERGENERATOR_CPP
#define RANDOMNUMBERGENERATOR_CPP

#include <cmath>
#include <chrono>

#include "RandomNumberGenerator.h"
#include "RandomNumberGenerator.cpp"

namespace myrandom
{
	RandomNumberGenerator::RandomNumberGenerator() : rnd((uint32_t)std::chrono::steady_clock::now().time_since_epoch().count()) {}
	RandomNumberGenerator::RandomNumberGenerator(uint32_t seed) : rnd(seed) {}

	uint32_t RandomNumberGenerator::operator()()
	{
		return rnd();
	}

	int RandomNumberGenerator::randIntInRange(int l, int r)
	{
		return l + rnd()%(r-l+1);
	}

	float RandomNumberGenerator::randFloat()
	{
		const int maxNum = (1 << 10);
		int x = abs((int)rnd());

		return ((float)(x%(maxNum)))/((float)(maxNum-1));
	}
	olc::vf2d RandomNumberGenerator::randomPointInsideTriangle(olc::vf2d A, olc::vf2d B, olc::vf2d C)
	{
		float c1 = this->randFloat();
		float c2 = this->randFloat();
		if (c1 > c2) std::swap(c1, c2);

		return A * (c1 - 0) + B * (c2 - c1) + C * (1.0f - c2);
	}
};

#endif
