#ifndef MATH_H
#define MATH_H

#include "olcPixelGameEngine.h"

namespace math
{
	float calcArea(olc::vf2d A, olc::vf2d B, olc::vf2d C);
	float calcDist(olc::vf2d A, olc::vf2d B);
	float sign(float x);

	bool segmentIntersection(olc::vf2d A1, olc::vf2d A2, olc::vf2d B1, olc::vf2d B2);
	bool insideQuad(olc::vf2d A, olc::vf2d p1, olc::vf2d p2, olc::vf2d p3, olc::vf2d p4); //works only for convex quads
	bool checkCollinear(olc::vf2d A, olc::vf2d B, olc::vf2d C, const float eps = 0.00000001);
};

#endif