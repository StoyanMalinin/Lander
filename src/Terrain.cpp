#include "Terrain.h"

void Terrain::render(Visualizer* vis, olc::Pixel color)
{
	for (int i = 0; i + 1 < points.size(); i++)
		vis->DrawLine(points[i], points[i + 1], color);
}

Terrain::Terrain(float gravity, const std::vector<olc::vf2d>& points) : gravity(gravity), points(points)
{
	for (int i = 0; i + 1 < points.size(); i++)
		this->colliders.push_back(new colliders::SegmentCollider(points[i], points[i + 1]));
}
