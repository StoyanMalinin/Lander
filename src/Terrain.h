#pragma once

#include <vector>
#include <random>

#include "Collider.h"
#include "Visualizer.h"
#include "PerlinNoise1D.h"

#include "olcPixelGameEngine.h"

class Visualizer;
namespace random
{
	template <int gridSz>
	class PerlinNoise1D;
};

class Terrain
{
protected:
	float gravity;
protected:
	std::vector <olc::vf2d> points;
	std::vector <colliders::SegmentCollider*> colliders;

	std::unordered_map <uint16_t, int> id2Ind;

public:
	void render(Visualizer* vis, olc::Pixel color = olc::WHITE);

public:
	Terrain(float gravity) : gravity(gravity) {}
	Terrain(float gravity, const std::vector<olc::vf2d>& points);

	void addPoint(olc::vf2d p)
	{
		points.emplace_back(p.x, p.y);
	}
	void addPoint(float x, float y)
	{
		points.emplace_back(x, y);
	}

	float getGravity() { return gravity; }

	virtual void generate(float screenWidth, float screenHeight) {}
	virtual bool checkSafeLanding(colliders::BoxCollider* foot, std::list<uint16_t>& footCollisions) { return false; }
};

class VerticalTerrain : public Terrain
{
protected:
	std::mt19937 rnd;

public:
	VerticalTerrain(float gravity);
	VerticalTerrain(float gravity, const std::vector<olc::vf2d>& points);
	VerticalTerrain(float gravity, uint32_t seed);

protected:
	float maxHeight, maxDelta;

public:
	void generate(float screenWidth, float screenHeight) override;
	bool checkSafeLanding(colliders::BoxCollider* foot, std::list<uint16_t>& footCollisions) override;
};