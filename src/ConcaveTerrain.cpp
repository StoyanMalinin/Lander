#pragma once

#include "ConcaveTerrain.h"

ConcaveTerrain::ConcaveTerrain(float gravity) : Terrain(gravity) {}
ConcaveTerrain::ConcaveTerrain(float gravity, const std::vector<olc::vf2d>& points) : Terrain(gravity, points) {}

ConcaveTerrain::ConcaveTerrain(float gravity, uint32_t seed) : Terrain(gravity)
{
	this->rnd = myrandom::RandomNumberGenerator(seed);
}

void ConcaveTerrain::generate(float screenWidth, float screenHeight)
{
	int len = rnd.randIntInRange(5, 15);
	int maxX = screenWidth;

	std::vector <int> xVals;
	for (int i = 0; i < len - 2; i++) xVals.push_back(rnd.randIntInRange(1, maxX-1));
	xVals.push_back(0); xVals.push_back(maxX);
	std::sort(xVals.begin(), xVals.end());

	for (int i = 0; i < len; i++)
	{
		int y = heightFunction(xVals[i], screenWidth, screenHeight);
		points.emplace_back(xVals[i], y);
	}

	innerPoint.x = rnd.randIntInRange(0, maxX - 1);
	innerPoint.y = rnd.randIntInRange(0, heightFunction(innerPoint.x, screenWidth, screenHeight));

	int mutationLen = rnd.randIntInRange(5, 10);
	for (int i = 0; i < mutationLen; i++) mutateTerrain(rnd.randIntInRange(0, points.size() - 2));

	for (int i = 0; i + 1 < points.size(); i++)
	{
		colliders.push_back(new colliders::SegmentCollider(points[i], points[i + 1]));
		id2Ind[colliders.back()->getId()] = i;
	}
}

bool ConcaveTerrain::checkSafeLanding(colliders::BoxCollider* foot, std::list<uint16_t>& footCollisions)
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

int ConcaveTerrain::heightFunction(float x, float screenWidth, float screenHeight)
{
	//f1(x) = -(x-0)(x-screenWidth) - adjusting the function to be both concave and non-negative on the interaval [0, screenWidth]
	//f2(x) = f1(x)/max{f1(x)} = (-(x-0)(x-screenWidth))/(-(screenWidth/2)^2 + screenWidth*(screenWidth/2)) - normalizing the funciton, so it outputs in the range [0, 1]
	//f(x) = f2(x)*(screenHeight*0.7) = (-(x-0)(x-screenWidth))/(-(screenWidth/2)^2 + screenWidth*(screenWidth/2))*(screenHeight*0.7f) - scaling the ouptut

	int y = (-(x - 0) * (x - screenWidth)) / (-(screenWidth / 2) * (screenWidth / 2) + screenWidth * (screenWidth / 2)) * (screenHeight * 0.7f);
	return y;
}

void ConcaveTerrain::mutateTerrain(int ind)
{
	olc::vf2d p = rnd.randomPointInsideTriangle(points[ind], points[ind + 1], innerPoint);
	points.insert(points.begin() + ind + 1, p);
}
