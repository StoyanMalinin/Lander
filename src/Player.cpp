#pragma once
#include "Player.h"

#include "olcPixelGameEngine.h"

Player::Player(const std::string& name, float x, float y, olc::Pixel color, olc::Key thrustKey, olc::Key leftKey, olc::Key rightKey) : name(name), pos(x, y), 
																																	   color(color), thrustKey(thrustKey), 
																																	   leftKey(leftKey), rightKey(rightKey),
																																	   fuel(600.0f)
{
	this->bodyCollider = std::make_shared<colliders::BoxCollider>(this->pos, this->pos + this->orientation.perp() * this->width,
																  this->pos + this->orientation.perp() * this->width + 0.90*this->orientation * this->height, 
																  this->pos + 0.90 * this->orientation * this->height);
	this->footCollider = std::make_shared<colliders::BoxCollider>(this->pos + 0.90 * this->orientation * this->height,
																  this->pos + 0.90 * this->orientation * this->height + this->orientation.perp() * this->width,
																  this->pos + this->orientation.perp() * this->width + this->orientation * this->height,
																  this->pos + this->orientation * this->height);

	this->bodyCollider->addIgnoredId(footCollider->getId());
	this->footCollider->addIgnoredId(bodyCollider->getId());

	this->sprite = std::make_shared<olc::Sprite>("./assets/" + name +  ".png");
	this->trans.Scale(this->width / (float)this->sprite->width, this->height / (float)this->sprite->height);
}

void Player::rotate(float angle)
{
	orientation.rotate(angle);
	trans.Rotate(angle);
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

Player::~Player()
{
	
}

std::string Player::getStats() const
{
	return name + ": " + "Score: " + std::to_string(score) + " | " + "Velocity: " + std::to_string(velocity.mag()) + " | " + "Fuel: " + std::to_string(fuel);
}

void Player::resetToPosition(const olc::vf2d& newPos)
{
	orientation = { 0, -1 };
	
	bodyCollider->active = true;
	footCollider->active = true;

	bodyCollider->p1 = newPos;
	bodyCollider->p2 = newPos + orientation.perp() * width;
	bodyCollider->p3 = newPos + orientation.perp() * width + 0.90 * orientation * height;
	bodyCollider->p4 = newPos + 0.90 * orientation * height;

	footCollider->p1 = newPos + 0.90 * orientation * height;
	footCollider->p2 = newPos + 0.90 * orientation * height + orientation.perp() * width;
	footCollider->p3 = newPos + orientation.perp() * width + orientation * height;
	footCollider->p4 = newPos + orientation * height;

	pos = newPos;
	velocity = { 0, 0 };

	trans = olc::GFX2D::Transform2D();
	this->trans.Scale(width / (float)sprite->width, height / (float)sprite->height);

	died = false;
	landed = false;
}
