#pragma once

#include "Player.h"
#include "Collider.h"
#include "Visualizer.h"

#include "Terrain.h"
#include "PerlinNoise1D.h"
#include "olcPGEX_Graphics2D.h"

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
	int yPos = 5;
	for (const Player& p : gm->players)
	{
		this->DrawString({ 0, yPos }, p.getStats(), p.color, 1);
		yPos += 10;
	}
}

void Visualizer::visualizePlayers()
{
	for (Player& p : gm->players)
	{
		olc::GFX2D::Transform2D t = p.trans;
		
		t.Translate(p.pos.x, ScreenHeight() - p.pos.y);
		
		SetPixelMode(olc::Pixel::ALPHA);
		olc::GFX2D::DrawSprite(p.sprite, t);
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

	terrainSelecter.addButton(ui::RadioButton(this, 20, olc::vi2d(700, 100), "Predefined Vertical Terrain"));
	terrainSelecter.addButton(ui::RadioButton(this, 20, olc::vi2d(700, 150), "Random Vertical Terrain"));

	startX = 0; startY = 0;
	endX = ScreenWidth(); endY = ScreenHeight();

	gm->addPlayer("Player 1", 1 * ScreenWidth() / (2 + 1), ScreenHeight(), olc::WHITE, olc::Key::W, olc::Key::A, olc::Key::D);
	//gm->addPlayer("Player 2", 2 * ScreenWidth() / (2 + 1), ScreenHeight(), olc::RED, olc::Key::W, olc::Key::A, olc::Key::D);

	startgame1Button = ui::Button(this, olc::vi2d(540, 600), olc::vi2d(740, 700), "Setup Game");
	startgame2Button = ui::Button(this, olc::vi2d(540, 600), olc::vi2d(740, 700), "Begin Game");

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

	return true;
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

	//update
	gm->updateState(fElapsedTime);

	//render
	visualizePlayers();
	gm->t->render(this);
	visualizePlayerStats();
	//visualizeColliders(olc::GREEN);

	return true;
}

bool Visualizer::OnUserUpdate(float fElapsedTime)
{
	if (gameState == GameState::WELCOMESCREEN) return WELCOMESCREENupdate(fElapsedTime);
	if (gameState == GameState::GAMESETTINGS) return GAMESETTINGSupdate(fElapsedTime);
    if (gameState == GameState::PLAYING) return PLAYINGupdate(fElapsedTime);
}