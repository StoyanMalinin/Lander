#pragma once
#include "olcPixelGameEngine.h"

#include "Collider.h"

struct Player
{
	float scale = 1;
	const float height = 30, width = 45;

	olc::vf2d pos = { 0, 0 };
	olc::vf2d orientation = { 0, - 1 };
	olc::Pixel color = olc::RED;

	olc::vf2d velocity = { 0, 0 };

	colliders::BoxCollider* bodyCollider;
	
	Player(float x, float y, olc::Pixel color); 

	void rotate(float angle);
	void applyForce(olc::vf2d F, float elapsedTime);
	void update(float elapsedTime);
};