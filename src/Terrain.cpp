#include "Terrain.h"

void Terrain::render(Visualizer* vis, olc::Pixel color)
{
	for (int i = 0; i + 1 < points.size(); i++)
		vis->DrawLine(points[i], points[i + 1], color);
}

Terrain::Terrain(float gravity, const std::vector<olc::vf2d>& points) : gravity(gravity), points(points)
{
	for (int i = 0; i + 1 < points.size(); i++)
	{
		this->colliders.push_back(new colliders::SegmentCollider(points[i], points[i + 1]));
		this->id2Ind[colliders.back()->getId()] = i;
	}
}

void VerticalTerrain::generate(float screenWidth, float screenHeight)
{
	int len = rnd() % 20 + 1;

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
	{
		colliders.push_back(new colliders::SegmentCollider(points[i], points[i + 1]));
		id2Ind[colliders.back()->getId()] = i;
	}
}

bool VerticalTerrain::checkSafeLanding(colliders::BoxCollider *foot, std::list<uint16_t>& footCollisions)
{
	for (uint16_t id : footCollisions)
		if (id2Ind.count(id) == false) return false;
	footCollisions.sort([&](uint16_t id1, uint16_t id2) { return id2Ind[id1] < id2Ind[id2]; });

	const float eps = 0.2;
	
	for (auto it = footCollisions.begin(); it != footCollisions.end() && next(it) != footCollisions.end(); it++)
	{
		int ind1 = id2Ind[*it], ind2 = id2Ind[*next(it)];
		for (int i = ind1; i <= ind2; i++)
		{
			if (!(math::checkCollinear(foot->p3, colliders[ind1]->A, foot->p4, eps) == true
				&& math::checkCollinear(foot->p3, colliders[ind1]->B, foot->p4, eps) == true))
			{
				return false;
			}
		}
	}

	olc::vf2d leftPoint;
	int ind = id2Ind[*footCollisions.begin()];
	while (ind >= 0)
	{
		if (math::checkCollinear(foot->p3, colliders[ind]->B, foot->p4, eps) == false) break;
		leftPoint = colliders[ind]->B;

		if (math::checkCollinear(foot->p3, colliders[ind]->A, foot->p4, eps) == false) break;
		leftPoint = colliders[ind]->A;

		ind--;
	}
	if (math::calcArea(foot->p1, foot->p4, leftPoint) > 0) return false;

	olc::vf2d rightPoint;
	ind = id2Ind[*prev(footCollisions.end())];

	while (ind < colliders.size())
	{
		if (math::checkCollinear(foot->p3, colliders[ind]->A, foot->p4, eps) == false) break;
		rightPoint = colliders[ind]->A;

		if (math::checkCollinear(foot->p3, colliders[ind]->B, foot->p4, eps) == false) break;
		rightPoint = colliders[ind]->B;

		ind++;
	}
	if (math::calcArea(foot->p2, foot->p3, rightPoint) < 0) return false;

	return true;
}

std::mt19937 VerticalTerrain::rnd = std::mt19937(19112019);
