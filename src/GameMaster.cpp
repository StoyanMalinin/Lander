#pragma once

#include <iostream>
#include "GameMaster.h"
#include "Terrain.h"
#include "VerticalTerrainPerlin.cpp"

GameMaster::GameMaster() : vis(nullptr) {}
GameMaster::GameMaster(Visualizer* vis) : vis(vis) 
{
	//t = new Terrain(5.81, { olc::vf2d(0, 300), olc::vf2d(100, 300), olc::vf2d(200, 300), olc::vf2d(400, 300), olc::vf2d(500, 300), olc::vf2d(1000, 300) });
	t = new VerticalTerrainPerlin<300>(3.81f);
	//t = new VerticalTerrain(3.81f);
	
	t->generate(vis->ScreenWidth(), vis->ScreenHeight());
}

void GameMaster::reset()
{
	players.clear();

	delete t;
	t = nullptr;
}

void GameMaster::addPlayer(const std::string& name, int x, int y, olc::Pixel color, olc::Key thrustKey, olc::Key leftKey, olc::Key rightKey)
{
	players.emplace_back(name, x, y, color, thrustKey, leftKey, rightKey);
}

void GameMaster::updateState(float elapsedTime)
{
	for (Player& p : players)
	{
		if (p.landed == true || p.died == true) continue;

		if (p.bodyCollider->getAllCollisions().empty() == false)
			handlePlayerDeath(p);

		if (p.pos.x <= -50 || p.pos.x >= vis->ScreenWidth() + 50
			|| p.pos.y <= 0 || p.pos.y >= vis->ScreenHeight() + 50)
		{
			handlePlayerDeath(p);
			std::cout << "The player went outside the map" << '\n';
		}
		
		std::list<uint16_t> footCollisions = p.footCollider->getAllCollisions();
		if (footCollisions.empty() == false)
		{
			if (p.velocity.mag() > 30)
			{
				handlePlayerDeath(p);
				std::cout << "The landing velocity was : " << p.velocity.mag() << '\n';
			}
			else
			{
				if (t->checkSafeLanding(p.footCollider.get(), footCollisions) == true)
				{
					handlePlayerLanding(p);
				}
				else
				{
					handlePlayerDeath(p);
					std::cout << "Bad positioning in landing" << '\n';
				}
			}
		}
	}

	for (Player& p : players)
	{
		if (p.died == true) continue;
		if (p.landed == true) continue;

		p.applyForce(olc::vf2d(0, -t->getGravity()), elapsedTime);
		if (vis->GetKey(p.thrustKey).bHeld == true && p.fuel > 0)
		{
			p.applyForce(-p.orientation * 12, elapsedTime);
		
			p.fuel -= elapsedTime*20;
			if (p.fuel < 0.0f) p.fuel = 0.0f;
		}

		if(vis->GetKey(p.leftKey).bHeld==true)
		{
			p.rotate(-p.rotationSpeed * elapsedTime);
		}
		if(vis->GetKey(p.rightKey).bHeld==true)
		{
			p.rotate(+p.rotationSpeed * elapsedTime);
		}
	}
	for (Player& p : players)
		if(p.landed==false && p.died==false) p.update(elapsedTime);
}

bool GameMaster::checkAllDeath()
{
	bool allDeath = true;
	for (const Player& p : players)
	{
		if (p.died == false)
		{
			allDeath = false;
			break;
		}
	}
	if (allDeath == true) return true;

	return false;
}

bool GameMaster::checkForLanding()
{
	for (const Player& p : players)
	{
		if (p.landed == true) return true;
	}

	return false;
}

bool GameMaster::checkRoundOver()
{
	if (checkForLanding() == true) return true;
	if (checkAllDeath() == true) return true;

	return false;
}

void GameMaster::resetPlayers()
{
	for (int i = 0; i < players.size(); i++)
	{
		players[i].resetToPosition(olc::vf2d((i + 1) * vis->ScreenWidth() / (players.size() +1), vis->ScreenHeight()));
		players[i].fuel += std::min(600.0f, 0.2f * players[i].fuel);
	}
}

const Player* GameMaster::getLandedPlayer()
{
	for (const Player& p : players)
		if (p.landed == true) return &p;

	return nullptr;
}

void GameMaster::FillRotatedRect(olc::vf2d pos, float width, float height, olc::vf2d orientation, olc::Pixel color)
{
	olc::vf2d down, right;
	
	down = orientation * height;
	right = orientation.perp() * width;

	vis->FillTriangle(pos, pos + down, pos + right, color);
	vis->FillTriangle(pos + down, pos + down + right, pos + right, color);
}

void GameMaster::handlePlayerDeath(Player& p)
{
	p.died = true;
	p.bodyCollider->active = false;
	p.footCollider->active = false;
	std::cout << p.name << " died" << '\n';
}

void GameMaster::handlePlayerLanding(Player& p)
{
	p.landed = true;
	p.score++;

	std::cout << p.name << " sucessfully landed" << '\n';
}
