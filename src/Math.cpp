#ifndef MATH_CPP
#define MATH_CPP

#include "Math.h"
#include "olcPixelGameEngine.h"

namespace math
{
	float calcArea(olc::vf2d A, olc::vf2d B, olc::vf2d C)
	{
		return ((A.x * B.y + A.y * C.x + B.x * C.y) - (A.y*B.x + A.x*C.y + B.y*C.x))*0.5;
	}

	float calcDist(olc::vf2d A, olc::vf2d B)
	{
		return sqrtf((A.x-B.x)*(A.x-B.x) + (A.y - B.y) * (A.y - B.y));
	}

	float sign(float x)
	{
		const float eps = 0.0000001f;
		
		if (x < -eps) return -1;
		if (x > +eps) return +1;
		return 0;
	}

	bool between(olc::vf2d P, olc::vf2d A, olc::vf2d B)
	{
		if (std::min(A.x, B.x) <= P.x && P.x <= std::max(A.x, B.x)
			&& std::min(A.y, B.y) <= P.y && P.y <= std::max(A.y, B.y))
		{
			return true;
		}

		return false;
	}

	bool segmentIntersection(olc::vf2d A1, olc::vf2d A2, olc::vf2d B1, olc::vf2d B2)
	{
		const float eps = 0.00000001;
		float sB1 = calcArea(A1, B1, A2);
		float sB2 = calcArea(A1, B2, A2);
		float sA1 = calcArea(B1, A1, B2);
		float sA2 = calcArea(B1, A2, B2);

		if (fabs(sB1) < eps && between(B1, A1, A2)==true) return true;
		if (fabs(sB2) < eps && between(B2, A1, A2) == true) return true;
		if (fabs(sA1) < eps && between(A1, B1, B2)==true) return true;
		if (fabs(sA2) < eps && between(A2, B1, B2)==true) return true;
		if (fabs(sB1) < eps || fabs(sB2) < eps || fabs(sA1) < eps || fabs(sA2) < eps) return false;

		if (sign(sB1) != sign(sB2) && sign(sA1) != sign(sA2)) return true;
		return false;
	}

	bool insideQuad(olc::vf2d A, olc::vf2d p1, olc::vf2d p2, olc::vf2d p3, olc::vf2d p4)
	{
		float S = fabs(calcArea(p1, p2, p3)) + fabs(calcArea(p1, p3, p4));
		float S1 = fabs(calcArea(A, p1, p2)), S2 = fabs(calcArea(A, p2, p3)), S3 = fabs(calcArea(A, p3, p4)), S4 = fabs(calcArea(A, p4, p1));
		
		const float eps = 0.0000001;
		return (fabs((S1 + S2 + S3 + S4) - S) < eps);
	}
	bool checkCollinear(olc::vf2d A, olc::vf2d B, olc::vf2d C, const float eps)
	{
		float ab = calcDist(A, B);
		float ac = calcDist(A, C);
		float bc = calcDist(B, C);
	
		if (ab >= ac && ab >= bc && fabs(ab - (ac + bc)) < eps) return true;
		if (ac >= ab && ac >= bc && fabs(ac - (ab + bc)) < eps) return true;
		if (bc >= ab && bc >= ac && fabs(bc - (ab + ac)) < eps) return true;
	
		return false;
	}
};

#endif