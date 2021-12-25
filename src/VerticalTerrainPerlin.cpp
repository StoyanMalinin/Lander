#pragma once

#include "Terrain.h"

#include <vector>
#include <algorithm>

template <int perlinGridSz>
class VerticalTerrainPerlin : public VerticalTerrain
{
private:
	random::PerlinNoise1D<perlinGridSz> perlin;

public:
	VerticalTerrainPerlin(float gravity);
	VerticalTerrainPerlin(float gravity, const std::vector<olc::vf2d>& points);
	VerticalTerrainPerlin(float gravity, uint32_t seed);

public:
	void generate(float screenWidth, float screenHeight) override;
};

template<int perlinGridSz>
VerticalTerrainPerlin<perlinGridSz>::VerticalTerrainPerlin(float gravity) : VerticalTerrain(gravity) {}

template<int perlinGridSz>
VerticalTerrainPerlin<perlinGridSz>::VerticalTerrainPerlin(float gravity, uint32_t seed) : Terrain(gravity)
{
	perlin = random::PerlinNoise1D(seed);
}

template <int perlinGridSz>
VerticalTerrainPerlin<perlinGridSz>::VerticalTerrainPerlin(float gravity, const std::vector<olc::vf2d>& points) : VerticalTerrain(gravity, points) {}

template <int perlinGridSz>
void VerticalTerrainPerlin<perlinGridSz>::generate(float screenWidth, float screenHeight)
{
	int len = rnd.randIntInRange(30, 60);
	int maxX = screenWidth;

	std::vector <int> xVals;
	for (int i = 0; i < len-2; i++) xVals.push_back(rnd() % maxX);
	xVals.push_back(0); xVals.push_back(maxX);
	std::sort(xVals.begin(), xVals.end());

	for (int i = 0; i < len; i++)
	{
		float coef = perlin.sample(xVals[i], 0, screenWidth);

		int y = coef * (screenHeight * 0.8);
		points.emplace_back(xVals[i], y);
	}

	for (int i = 0; i + 1 < len; i++)
	{
		colliders.push_back(new colliders::SegmentCollider(points[i], points[i + 1]));
		id2Ind[colliders.back()->getId()] = i;
	}
}