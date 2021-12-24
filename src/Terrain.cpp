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

void VerticalTerrain::generate(float screenWidth, float screenHeight)
{
	int len = rnd() % 10 + 1;

	int xSum = screenWidth;
	std::vector <float> xDeltas;

	for (int i = 0; i < len-2; i++)
	{
		xDeltas.push_back(rnd() % ((xSum-5) / 3) + 1);
		xSum -= xDeltas.back();
	}
	xDeltas.push_back(xSum);

	int x = 0;
	for (int i = 0; i < len; i++)
	{
		int y = rnd() % ((int)(screenHeight*0.7) + 1);
		points.emplace_back(x, y);

		if (i != len - 1) x += xDeltas[i];
	}

	for (int i = 0; i + 1 < len; i++)
		colliders.push_back(new colliders::SegmentCollider(points[i], points[i + 1]));
}

std::mt19937 VerticalTerrain::rnd = std::mt19937(19112019);
