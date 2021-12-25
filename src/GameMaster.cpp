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
		
		std::list<uint16_t> footCollisions = p.footCollider->getAllCollisions();
		if (footCollisions.empty() == false)
		{
			if (p.velocity.mag() > 25)
			{
				handlePlayerDeath(p);
				std::cout << "The landing velocity was : " << p.velocity.mag() << '\n';
			}
			else
			{
				if (t->checkSafeLanding(p.footCollider, footCollisions) == true)
				{
					std::cout << "Player successfully landed" << '\n';
					p.landed = true;
				}
				else
				{
					std::cout << "Bad positioning in landing" << '\n';
					handlePlayerDeath(p);
				}
			}
		}
	}

	for (Player& p : players)
	{
		if (p.died == true) continue;
		if (p.landed == true) continue;

		p.applyForce(olc::vf2d(0, -t->getGravity()), elapsedTime);
		if (vis->GetKey(p.thrustKey).bHeld == true)
			p.applyForce(-p.orientation * 12, elapsedTime);
		if(vis->GetKey(p.leftKey).bHeld==true)
		{
			p.bodyCollider->rotate(p.pos, -p.rotationSpeed * elapsedTime);
			p.footCollider->rotate(p.pos, -p.rotationSpeed * elapsedTime);

			p.orientation.rotate(-p.rotationSpeed * elapsedTime);
			p.trans.Rotate(-p.rotationSpeed * elapsedTime);
		}
		if(vis->GetKey(p.rightKey).bHeld==true)
		{
			p.bodyCollider->rotate(p.pos, +p.rotationSpeed * elapsedTime);
			p.footCollider->rotate(p.pos, +p.rotationSpeed * elapsedTime);

			p.orientation.rotate(+p.rotationSpeed * elapsedTime);
			p.trans.Rotate(+p.rotationSpeed * elapsedTime);
		}
	}
	for (Player& p : players)
		if(p.landed==false && p.died==false) p.update(elapsedTime);
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
	std::cout << "Player died" << '\n';
}
