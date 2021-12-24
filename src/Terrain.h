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
};

class VerticalTerrain : public Terrain
{
private:
	static std::mt19937 rnd;

public:
	VerticalTerrain(float gravity) : Terrain(gravity) {}

private:
	float maxHeight, maxDelta;

public:
	void generate(float screenWidth, float screenHeight) override;
};