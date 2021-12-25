#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics2D.h"

#include "Collider.h"

struct Player
{
	float scale = 1;
	const float rotationSpeed = 1.5;
	const float height = 30, width = 30;
	
	olc::Sprite *sprite;
	olc::Pixel color = olc::WHITE;

	olc::vf2d pos = { 0, 0 };
	olc::vf2d velocity = { 0, 0 };
	olc::GFX2D::Transform2D trans;
	olc::vf2d orientation = { 0, - 1 };

	olc::Key thrustKey, leftKey, rightKey;
	colliders::BoxCollider* bodyCollider, *footCollider;
	
	float score = 0;
	std::string name;
	bool landed = false, died = false;

	Player(const std::string& name, float x, float y, olc::Pixel color, olc::Key thrustKey, olc::Key leftKey, olc::Key rightKey);

	void rotate(float angle);
	void applyForce(olc::vf2d F, float elapsedTime);
	void update(float elapsedTime);

	std::string getStats() const;
};