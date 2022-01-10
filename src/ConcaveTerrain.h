#pragma once

#include <vector>
#include <random>

#include "Collider.h"
#include "Visualizer.h"
#include "PerlinNoise1D.h"

#include "olcPixelGameEngine.h"

class ConcaveTerrain : public Terrain
{
private:
	olc::vf2d innerPoint;

private:
	random::RandomNumberGenerator rnd;
public:
	ConcaveTerrain(float gravity);
	ConcaveTerrain(float gravity, const std::vector<olc::vf2d>& points);
	ConcaveTerrain(float gravity, uint32_t seed);

private:
	void generate(float screenWidth, float screenHeight) override;
	bool checkSafeLanding(colliders::BoxCollider* foot, std::list<uint16_t>& footCollisions);

	int heightFunction(float x, float screenWidth, float screenHeight);
	void mutateTerrain(int ind);
};