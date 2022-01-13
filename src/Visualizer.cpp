#pragma once

#include "Player.h"
#pragma once

#include "olcPGEX_Graphics2D.h"

#include "Terrain.h"
#include "Collider.h"
#include "Visualizer.h"
#include "PerlinNoise1D.h"
#include "ConcaveTerrain.h"
#include "VerticalTerrainPerlin.cpp"

void Visualizer::visualizeColliders(olc::Pixel color)
{
	for (colliders::SegmentCollider* sc : colliders::SegmentCollider::allSegmentColliders)
		DrawLine(sc->A, sc->B, color);

	for (colliders::BoxCollider* bc : colliders::BoxCollider::allBoxColliders)
	{
		DrawLine(bc->p1, bc->p2, color);
		DrawLine(bc->p2, bc->p3, color);
		DrawLine(bc->p3, bc->p4, color);
		DrawLine(bc->p4, bc->p1, color);
	}
}

void Visualizer::visualizePlayerStats()
{
	int yPos = ScreenHeight() - 10;
	for (const Player& p : gm->players)
	{
		this->DrawStringDecal(olc::vf2d(0, yPos), p.getStats(), p.color, {1.3f, 1.3f});
		yPos -= 10;
	}
}

void Visualizer::visualizePlayers()
{
	for (Player& p : gm->players)
	{
		if (p.died == true) continue;

		olc::GFX2D::Transform2D t = p.trans;
		
		t.Translate(p.pos.x, ScreenHeight() - p.pos.y);
		
		SetPixelMode(olc::Pixel::ALPHA);
		olc::GFX2D::DrawSprite(p.sprite.get(), t);
		SetPixelMode(olc::Pixel::NORMAL);

	}
}

olc::vi2d Visualizer::inv(const olc::vi2d& v)
{
	return { v.x, ScreenHeight() - v.y };
}

void Visualizer::FillTriangle(const olc::vi2d& pos1, const olc::vi2d& pos2, const olc::vi2d& pos3, olc::Pixel p)
{
	olc::PixelGameEngine::FillTriangle(inv(pos1), inv(pos2), inv(pos3), p);
}

void Visualizer::DrawLine(const olc::vi2d& pos1, const olc::vi2d& pos2, olc::Pixel p, uint32_t pattern)
{
	olc::PixelGameEngine::DrawLine(inv(pos1), inv(pos2), p, pattern);
}

bool Visualizer::OnUserCreate()
{
	sAppName = "Lander";
	gm = new GameMaster(this);
	gameState = GameState::WELCOMESCREEN;

	playerCountSelecter.addButton(ui::RadioButton(this, 20, olc::vi2d(200, 100), "1 Player"));
	playerCountSelecter.addButton(ui::RadioButton(this, 20, olc::vi2d(200, 150), "2 Players"));
	playerCountSelecter.addButton(ui::RadioButton(this, 20, olc::vi2d(200, 200), "3 Players"));

	terrainSelecter.addButton(ui::RadioButton(this, 20, olc::vi2d(700, 100), "Amplified Vertical Terrain"));
	terrainSelecter.addButton(ui::RadioButton(this, 20, olc::vi2d(700, 150), "Smooth Vertical Terrain"));
	terrainSelecter.addButton(ui::RadioButton(this, 20, olc::vi2d(700, 200), "Concave Terrain"));

	startX = 0; startY = 0;
	endX = ScreenWidth(); endY = ScreenHeight();

	//gm->addPlayer("Player 2", 2 * ScreenWidth() / (2 + 1), ScreenHeight(), olc::RED, olc::Key::W, olc::Key::A, olc::Key::D);

	startgame1Button = ui::Button(this, olc::vi2d(540, 600), olc::vi2d(740, 700), "Setup Game");
	startgame2Button = ui::Button(this, olc::vi2d(540, 600), olc::vi2d(740, 700), "Begin Game");

	nextRoundButton = ui::Button(this, olc::vi2d(540, 100), olc::vi2d(740, 200), "Next Round");
	resetGameButton = ui::Button(this, olc::vi2d(540, 600), olc::vi2d(740, 700), "Reset Game");

	return true;
}

bool Visualizer::ROUNDOVERupdate(float fElapsedTime)
{
	Clear(olc::BLACK);

	//update
	if (resetGameButton.checkCliked() == true)
	{
		gameState = GameState::GAMESETTINGS;
		
		playerCountSelecter.reset();
		terrainSelecter.reset();

		return true;
	}
	if (gm->checkForLanding() == true && nextRoundButton.checkCliked() == true)
	{
		gameState = GameState::PLAYING;
		resetRound();

		return true;
	}

	//render
	ui::renderButton(resetGameButton, this);
	if (gm->checkForLanding() == true) ui::renderButton(nextRoundButton, this);

	olc::vi2d textSz = GetTextSize(ROUNDOVERtext);
	float wTextScale = 0.8f * ((float)ScreenWidth()) / ((float)textSz.x), hTextScale = 0.2f * ((float)ScreenHeight()) / ((float)textSz.y);
	DrawStringDecal(olc::vf2d(0, 0) + olc::vf2d(ScreenWidth() * 0.1, ScreenHeight() * 0.4), ROUNDOVERtext, olc::WHITE, olc::vf2d(wTextScale, hTextScale));

	return true;
}

bool Visualizer::WELCOMESCREENupdate(float fElapsedTime)
{
	Clear(olc::BLACK);

	//update
	if (startgame1Button.checkCliked() == true)
		gameState = GameState::GAMESETTINGS;

	//render
	ui::renderButton(startgame1Button, this);

	olc::vi2d textSz = GetTextSize("Lander");
	float wTextScale = 0.8f * ((float)ScreenWidth()) / ((float)textSz.x), hTextScale = 0.2f * ((float)ScreenHeight()) / ((float)textSz.y);
	DrawStringDecal(olc::vf2d(0, 0) + olc::vf2d(ScreenWidth() * 0.1, ScreenHeight() * 0.4), "Lander", olc::WHITE, olc::vf2d(wTextScale, hTextScale));

	return true;
}

void Visualizer::resetRound()
{
	gm->resetPlayers();

	delete gm->t;
	gm->t = genNewTerrain();
}

bool Visualizer::GAMESETTINGSupdate(float fElapsedTime)
{
	Clear(olc::GREY);
	
	//update
	playerCountSelecter.updateState();
	terrainSelecter.updateState();
	if (startgame2Button.checkCliked() == true)
	{
		if (playerCountSelecter.clicked == true && terrainSelecter.clicked == true)
		{
			gameState = GameState::PLAYING;
			
			gm->reset();
			int playerCnt = -1;
			if (playerCountSelecter.radioButtons[0].clicked == true) playerCnt = 1;
			else if (playerCountSelecter.radioButtons[1].clicked == true) playerCnt = 2;
			else if (playerCountSelecter.radioButtons[2].clicked == true) playerCnt = 3;

			if(playerCnt>=1)
				gm->addPlayer("Player 1", 1 * ScreenWidth() / (playerCnt + 1), ScreenHeight(), olc::WHITE, olc::Key::W, olc::Key::A, olc::Key::D);
			if(playerCnt>=2)
				gm->addPlayer("Player 2", 2 * ScreenWidth() / (playerCnt + 1), ScreenHeight(), olc::Pixel(21, 183, 252), olc::Key::UP, olc::Key::LEFT, olc::Key::RIGHT);
			if (playerCnt >= 3)
				gm->addPlayer("Player 3", 3 * ScreenWidth() / (playerCnt + 1), ScreenHeight(), olc::RED, olc::Key::I, olc::Key::J, olc::Key::L);

			if (terrainSelecter.radioButtons[0].clicked == true) terrainType = TerrainType::VERTICALTERRAIN;
			else if (terrainSelecter.radioButtons[1].clicked == true) terrainType = TerrainType::PERLINNOISETERRAIN;
			else if (terrainSelecter.radioButtons[2].clicked == true) terrainType = TerrainType::CONCAVETERRAIN;
			
			delete gm->t;
			gm->t = genNewTerrain();

			return true;
		}
	}

	//render
	ui::renderRadioButtonCluster(playerCountSelecter, this);
	ui::renderRadioButtonCluster(terrainSelecter, this);
	ui::renderButton(startgame2Button, this);

	return true;
}

bool Visualizer::PLAYINGupdate(float fElapsedTime)
{
	Clear(olc::BLACK);

	//render
	visualizePlayers();
	gm->t->render(this);
	visualizePlayerStats();
	visualizeColliders(olc::GREEN);

	//update
	gm->updateState(fElapsedTime);
	if (gm->checkRoundOver() == true)
	{
		gameState = GameState::ROUNDOVER;
		const Player* p = gm->getLandedPlayer();
		
		if (p == nullptr) ROUNDOVERtext = "All players died this round";
		else ROUNDOVERtext = p->name + " successfully landed";

		return true;
	}

	return true;
}

bool Visualizer::OnUserUpdate(float fElapsedTime)
{
	if (gameState == GameState::WELCOMESCREEN) return WELCOMESCREENupdate(fElapsedTime);
	if (gameState == GameState::GAMESETTINGS) return GAMESETTINGSupdate(fElapsedTime);
    if (gameState == GameState::PLAYING) return PLAYINGupdate(fElapsedTime);
    if (gameState == GameState::ROUNDOVER) return ROUNDOVERupdate(fElapsedTime);

	return true;
}

Terrain* Visualizer::genNewTerrain()
{
	Terrain* t;
	if (terrainType == TerrainType::VERTICALTERRAIN) t = new VerticalTerrain(5.0f);
	else if (terrainType == TerrainType::PERLINNOISETERRAIN) t = new VerticalTerrainPerlin<20>(5.0f);
	else if (terrainType == TerrainType::CONCAVETERRAIN) t = new ConcaveTerrain(5.0f);
	
	t->generate(ScreenWidth(), ScreenHeight());
	return t;
}
