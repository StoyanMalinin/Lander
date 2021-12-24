#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include <vector>

#include "Terrain.h"
#include "Player.h"
#include "Visualizer.h"

#include "olcPixelGameEngine.h"

class Terrain;
class Visualizer;

class GameMaster
{
private:
	Visualizer *vis;
public:
	std::vector <Player> players;

	Terrain* t = nullptr;

public:
	GameMaster();
	GameMaster(Visualizer* vis);

public:
	void addPlayer(int x, int y, olc::Pixel color, olc::Key thrustKey, olc::Key leftKey, olc::Key rightKey);
	void updateState(float elapsedTime);

private:
	void FillRotatedRect(olc::vf2d pos, float width, float height, olc::vf2d orientation, olc::Pixel color);
	void handlePlayerDeath(Player& p);
};

#endif