#pragma once

#include "Terrain.h"

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
	int len = rnd() % 30 + 10;

	int xSum = screenWidth;
	std::vector <int> xDeltas;

	for (int i = 0; i < len - 2; i++)
	{
		xDeltas.push_back(rnd() % ((xSum - 5) / 3) + 1);
		xSum -= xDeltas.back();
	}
	xDeltas.push_back(xSum);

	int x = 0;
	for (int i = 0; i < len; i++)
	{
		int y = ((perlin.sample(x, 0, screenWidth) + 1.0f)*0.5f) * (screenHeight * 0.7);
		points.emplace_back(x, y);

		if (i != len - 1) x += xDeltas[i];
	}

	for (int i = 0; i + 1 < len; i++)
	{
		colliders.push_back(new colliders::SegmentCollider(points[i], points[i + 1]));
		id2Ind[colliders.back()->getId()] = i;
	}
}