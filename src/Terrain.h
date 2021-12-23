#pragma once

#include <vector>
#include "olcPixelGameEngine.h"

#include "Collider.h"
#include "Visualizer.h"

class Visualizer;

class Terrain
{
public:
	float gravity;
private:
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

	void generate();
};

