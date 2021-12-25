#pragma once

#include <cmath>
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
		int x = abs((int)rnd());

		return ((float)(x%(maxNum)))/((float)(maxNum-1));
	}
};