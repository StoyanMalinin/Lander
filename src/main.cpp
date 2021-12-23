#define OLC_PGE_APPLICATION

#include <iostream>
#include "Visualizer.h"

using namespace std;

int main()
{
	Visualizer v;
	
	v.Construct(1280, 720, 1, 1);
	v.Start();
}