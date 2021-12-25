#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "Collider.h"
#include "GameMaster.h"
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class GameMaster;

class Visualizer : public olc::PixelGameEngine
{
private:
	GameMaster *gm;
	float startX, endX, startY, endY;

private:
	void visualizeColliders(olc::Pixel color);
	void visualizePlayers();
	olc::vi2d inv(const olc::vi2d& v);

public:
	void FillTriangle(const olc::vi2d& pos1, const olc::vi2d& pos2, const olc::vi2d& pos3, olc::Pixel p = olc::WHITE) override;
	void DrawLine(const olc::vi2d& pos1, const olc::vi2d& pos2, olc::Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF) override;

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
};

#endif // !VISUALIZER_H