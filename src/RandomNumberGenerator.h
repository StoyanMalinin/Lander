#pragma once

#include <chrono>
#include <random>

namespace random
{
	class RandomNumberGenerator
	{
	private:
		std::mt19937 rnd;
	
	public:
		RandomNumberGenerator();
		RandomNumberGenerator(uint32_t seed);

		uint32_t operator() ();
		int randIntInRange(int l, int r);
		float randFloat(); //returns a float number in the interval [0, 1]
	};
};