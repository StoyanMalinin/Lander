#define OLC_PGE_APPLICATION
#define OLC_PGEX_GRAPHICS2D

#include <iostream>

#include "Visualizer.h"

using namespace std;

int main()
{
	Visualizer v;

	v.Construct(1600, 900, 1, 1);
	v.Start();
}