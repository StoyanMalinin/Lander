#pragma once
#include "Player.h"

#include "olcPixelGameEngine.h"

Player::Player(float x, float y, olc::Pixel color, olc::Key thrustKey, olc::Key leftKey, olc::Key rightKey) : pos(x, y), color(color), thrustKey(thrustKey), leftKey(leftKey), rightKey(rightKey)
{
	this->bodyCollider = new colliders::BoxCollider(this->pos, this->pos + this->orientation.perp() * this->width,
		                                            this->pos + this->orientation.perp() * this->width + 0.90*this->orientation * this->height, 
		                                            this->pos + 0.90 * this->orientation * this->height);
	this->footCollider = new colliders::BoxCollider(this->pos + 0.90 * this->orientation * this->height, 
		                                            this->pos + 0.90 * this->orientation * this->height + this->orientation.perp() * this->width,
		                                            this->pos + this->orientation.perp() * this->width + this->orientation * this->height,
		                                            this->pos + this->orientation * this->height);


	this->sprite = new olc::Sprite("./assets/player.png");
	this->trans.Scale(this->width / (float)this->sprite->width, this->height / (float)this->sprite->height);
}

void Player::rotate(float angle)
{
	orientation.rotate(angle);
	trans.Rotate(angle);
}

Player::Player()
{
}

void Player::applyForce(olc::vf2d F, float elapsedTime)
{
	velocity += F * elapsedTime;
}

void Player::update(float elapsedTime)
{	
	pos += velocity * elapsedTime;
	bodyCollider->translate(velocity * elapsedTime);
	footCollider->translate(velocity * elapsedTime);
}


