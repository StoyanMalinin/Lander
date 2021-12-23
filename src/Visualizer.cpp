#pragma once

#include "Collider.h"
#include "Visualizer.h"

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

Visualizer::Visualizer()
{
	sAppName = "Lander";
	gm = new GameMaster(this);
}

bool Visualizer::OnUserCreate()
{
	startX = 0; startY = 0;
	endX = ScreenWidth(); endY = ScreenHeight();

	gm->addPlayer(0, 200, olc::RED);

	return true;
}

bool Visualizer::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::BLACK);
	
	gm->updateState(fElapsedTime);

	gm->render();
	visualizeColliders(olc::GREEN);

	return true;
}