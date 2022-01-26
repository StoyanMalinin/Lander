#pragma once

#include <chrono>
#include <random>

#include "olcPixelGameEngine.h"

namespace myrandom
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
		olc::vf2d randomPointInsideTriangle(olc::vf2d A, olc::vf2d B, olc::vf2d C);
	};
};
