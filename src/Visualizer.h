#ifndef VISUALIZER_H
#define VISUALIZER_H


#include "UI.h"
#include "Terrain.h"
#include "Collider.h"
#include "GameMaster.h"
#include "RandomNumberGenerator.h"

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

class Terrain;
class GameMaster;
enum class TerrainType;
class RandomNumberGenerator;

enum class GameState
{
	WELCOMESCREEN,
	GAMESETTINGS,
	PLAYING,
	ROUNDOVER,
	GAMEOVER,
};

class Visualizer : public olc::PixelGameEngine
{
private:
	GameState gameState;
	TerrainType terrainType;

	GameMaster *gm;
	float startX, endX, startY, endY;

	random::RandomNumberGenerator rnd;

private:
	void visualizeColliders(olc::Pixel color);
	void visualizePlayerStats();
	void visualizePlayers();
	olc::vi2d inv(const olc::vi2d& v);
	
	ui::Button startgame1Button;
	bool WELCOMESCREENupdate(float fElapsedTime);
	
	ui::Button startgame2Button;
	ui::RadioButtonCluster terrainSelecter;
	ui::RadioButtonCluster playerCountSelecter;
	bool GAMESETTINGSupdate(float fElapsedTime);

	void resetRound();
	Terrain* genNewTerrain();
	bool PLAYINGupdate(float fElapsedTime);

	std::string ROUNDOVERtext;
	ui::Button nextRoundButton, resetGameButton;
	bool ROUNDOVERupdate(float fElapsedTime);

public:
	void FillTriangle(const olc::vi2d& pos1, const olc::vi2d& pos2, const olc::vi2d& pos3, olc::Pixel p = olc::WHITE) override;
	void DrawLine(const olc::vi2d& pos1, const olc::vi2d& pos2, olc::Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF) override;

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
};

#endif // !VISUALIZER_H