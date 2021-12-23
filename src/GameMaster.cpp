#pragma once

#include <iostream>
#include "GameMaster.h"

GameMaster::GameMaster() : vis(nullptr) {}
GameMaster::GameMaster(Visualizer* vis) : vis(vis) 
{
	t = new Terrain(9.81, { olc::vf2d(0, 0), olc::vf2d(100, 100), olc::vf2d(200, 100), olc::vf2d(1000, 400), olc::vf2d(1200, 410) });
}

void GameMaster::addPlayer(int x, int y, olc::Pixel color)
{
	players.emplace_back(x, y, color);
}

void GameMaster::render()
{
	for (Player& p : players)
		renderEntity(p);
	t->render(vis, olc::WHITE);
}

void GameMaster::updateState(float elapsedTime)
{
	std::list<uint16_t> l = players[0].bodyCollider->getAllCollisions();
	if (l.size() > 0)
	{
		std::cout << "collisions " << players[0].bodyCollider->getId() << ": ";
		for (uint16_t id : l)
			std::cout << " " << id;
		std::cout << '\n';
	}

	for (Player& p : players)
		p.applyForce(olc::vf2d(0, -t->gravity), elapsedTime);
	for (Player& p : players)
		p.update(elapsedTime);
}

void GameMaster::renderEntity(Player& p)
{
	FillRotatedRect(p.pos, p.width * p.scale, p.height * p.scale, p.orientation, p.color);
}

void GameMaster::FillRotatedRect(olc::vf2d pos, float width, float height, olc::vf2d orientation, olc::Pixel color)
{
	olc::vf2d down, right;
	
	down = orientation * height;
	right = orientation.perp() * width;

	vis->FillTriangle(pos, pos + down, pos + right, color);
	vis->FillTriangle(pos + down, pos + down + right, pos + right, color);
}
