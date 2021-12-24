#pragma once

#include "Player.h"
#include "Collider.h"
#include "Visualizer.h"

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

Visualizer::Visualizer()
{
	
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

	startX = 0; startY = 0;
	endX = ScreenWidth(); endY = ScreenHeight();

	gm->addPlayer(500, 500, olc::RED, olc::Key::W, olc::Key::A, olc::Key::D);

	return true;
}

bool Visualizer::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::BLACK);
	
	//update
	gm->updateState(fElapsedTime);

	//render
	visualizePlayers();
	//gm->t->render(this);
	visualizeColliders(olc::GREEN);

	return true;
}