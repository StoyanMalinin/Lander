#pragma once

#include <chrono>

#include "RandomNumberGenerator.h"

namespace random
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
		return ((float)(rnd()&(maxNum-1)))/((float)(maxNum-1));
	}
};