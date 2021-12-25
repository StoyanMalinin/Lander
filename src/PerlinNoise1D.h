#pragma once

#include "RandomNumberGenerator.h"

namespace random
{
	class RandomNumberGenerator;

	template <int gridSz>
	class PerlinNoise1D
	{
	private:
		float vectors[gridSz+1];
		RandomNumberGenerator rnd;

	public:
		PerlinNoise1D();
		PerlinNoise1D(uint32_t seed);

		void init();
		float sample(float x, float minX, float maxX);
	
	private:
		float linearInterpolation(float x, float x1, float y1, float x2, float y2);
	};

	template <int gridSz>
	PerlinNoise1D<gridSz>::PerlinNoise1D()
	{
		this->rnd = RandomNumberGenerator();
		this->init();
	}

	template <int gridSz>
	PerlinNoise1D<gridSz>::PerlinNoise1D(uint32_t seed)
	{
		this->rnd = RandomNumberGenerator(seed);
		this->init();
	}

	template <int gridSz>
	void PerlinNoise1D<gridSz>::init()
	{
		for (int i = 0; i <= gridSz; i++)
			vectors[i] = rnd.randFloat() * 2.0f - 1.0f;
	}

	template <int gridSz>
	float PerlinNoise1D<gridSz>::sample(float x, float minX, float maxX)
	{
		float blockSz = (maxX - minX) / gridSz;
		int block = (int)((x - minX) / blockSz);
		if (block == gridSz) return vectors[block];

		float v1 = vectors[block], offset1 = x - ((float)block) * blockSz;
		float v2 = vectors[block + 1], offset2 = ((float)(block + 1)) * blockSz - x;

		float val = linearInterpolation(x, ((float)block) * blockSz, v1 * offset1, ((float)(block + 1)) * blockSz, v2 * offset2);
		float minVal = -1 * blockSz, maxVal = +1*blockSz;

		return (val - minVal) / (maxVal - minVal);
	}

	template <int gridSz>
	float PerlinNoise1D<gridSz>::linearInterpolation(float x, float x1, float y1, float x2, float y2)
	{
		return (y1 * (x2 - x) + y2 * (x - x1)) / (x2 - x1);
	}
};
