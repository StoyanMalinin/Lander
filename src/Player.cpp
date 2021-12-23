#pragma once
#include "Player.h"

Player::Player(float x, float y, olc::Pixel color) : pos(x, y), color(color)
{
	bodyCollider = new colliders::BoxCollider(pos, pos + orientation.perp() * width, pos + orientation.perp() * width + orientation * height, pos + orientation * height);
}

void Player::rotate(float angle)
{
	orientation.rotate(angle);
}

void Player::applyForce(olc::vf2d F, float elapsedTime)
{
	velocity += F * elapsedTime;
}

void Player::update(float elapsedTime)
{
	pos += velocity * elapsedTime;
	bodyCollider->translate(velocity * elapsedTime);
}


