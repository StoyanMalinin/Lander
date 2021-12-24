#pragma once

#include <vector>
#include <random>
#include "Collider.h"
#include "Visualizer.h"

#include "olcPixelGameEngine.h"
class Visualizer;

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
private:
	static std::mt19937 rnd;

public:
	VerticalTerrain(float gravity) : Terrain(gravity) {}
	VerticalTerrain(float gravity, const std::vector<olc::vf2d>& points) : Terrain(gravity, points) {}

private:
	float maxHeight, maxDelta;

public:
	void generate(float screenWidth, float screenHeight) override;
	bool checkSafeLanding(colliders::BoxCollider* foot, std::list<uint16_t>& footCollisions) override;
};