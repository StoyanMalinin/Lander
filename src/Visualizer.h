#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "Collider.h"
#include "GameMaster.h"
#include "olcPixelGameEngine.h"

class GameMaster;

class Visualizer : public olc::PixelGameEngine
{
	GameMaster *gm;
	float startX, endX, startY, endY;

private:
	bool Draw(int32_t x, int32_t y, olc::Pixel p = olc::WHITE)
	{
		//x = round((((float)(x - startX)) / (endX - startX)) * ((float)ScreenWidth()));
		//y = round((((float)(y - startY)) / (endY - startY)) * ((float)ScreenHeight()));
		y = ScreenHeight() - y;

		return olc::PixelGameEngine::Draw(x, y, p);
	}

private:
	void visualizeColliders(olc::Pixel color);

public:
	Visualizer();

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
};

#endif // !VISUALIZER_H