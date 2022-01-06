#pragma once

#include "Terrain.h"
//#include "PerlinNoise1D.h"

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

Terrain::~Terrain()
{
	points.clear();
	while (colliders.empty() == false)
	{
		delete colliders.back();
		colliders.pop_back();
	}

	id2Ind.clear();
}

VerticalTerrain::VerticalTerrain(float gravity) : Terrain(gravity) {}

VerticalTerrain::VerticalTerrain(float gravity, const std::vector<olc::vf2d>& points) : Terrain(gravity, points) {}


VerticalTerrain::VerticalTerrain(float gravity, uint32_t seed) : Terrain(gravity)
{
	this->rnd = random::RandomNumberGenerator(seed);
}

void VerticalTerrain::generate(float screenWidth, float screenHeight)
{
	int len = rnd.randIntInRange(5, 25);

	int maxX = screenWidth;

	std::vector <int> xVals;
	for (int i = 0; i < len - 2; i++) xVals.push_back(rnd() % maxX);
	xVals.push_back(0); xVals.push_back(maxX);
	std::sort(xVals.begin(), xVals.end());

	for (int i = 0; i < len; i++)
	{
		int y = rnd() % ((int)(screenHeight*0.7) + 1);
		points.emplace_back(xVals[i] , y);
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

