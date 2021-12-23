#ifndef COLLIDER_CPP
#define COLLIDER_CPP

#include "Collider.h"

colliders::Collider::Collider()
{
	this->id = rnd();//we do not care that it overflows
	this->ignored.insert(id);
}

std::mt19937 colliders::Collider::rnd = std::mt19937(22);

colliders::SegmentCollider::SegmentCollider(olc::vf2d A, olc::vf2d B) : Collider()
{
	this->A = A;
	this->B = B;

	this->allSegmentColliders.push_back(this);
}

std::list<uint16_t> colliders::SegmentCollider::getAllCollisions()
{
	std::list<uint16_t> l;

	for (colliders::SegmentCollider* sc : colliders::SegmentCollider::allSegmentColliders)
		if (checkIgnored(sc->getId()) == false && collides(*sc) == true) l.push_back(sc->getId());
	for (colliders::BoxCollider* bc : colliders::BoxCollider::allBoxColliders)
		if (checkIgnored(bc->getId()) == false && collides(*bc) == true) l.push_back(bc->getId());

	return l;
}

std::list<colliders::SegmentCollider*>  colliders::SegmentCollider::allSegmentColliders = std::list<colliders::SegmentCollider*>();
std::list<colliders::BoxCollider*>  colliders::BoxCollider::allBoxColliders = std::list<colliders::BoxCollider*>();

bool colliders::SegmentCollider::collides(const SegmentCollider& other) const
{
	return math::segmentIntersection(A, B, other.A, other.B);
}

bool colliders::SegmentCollider::collides(const BoxCollider& other) const
{
	if (math::segmentIntersection(A, B, other.p1, other.p2) == true) return true;
	if (math::segmentIntersection(A, B, other.p2, other.p3) == true) return true;
	if (math::segmentIntersection(A, B, other.p3, other.p4) == true) return true;
	if (math::segmentIntersection(A, B, other.p4, other.p1) == true) return true;

	//if (math::insideQuad(A, other.p1, other.p2, other.p3, other.p4) == true) return true;
	//if (math::insideQuad(B, other.p1, other.p2, other.p3, other.p4) == true) return true;

	return false;
}

colliders::BoxCollider::BoxCollider() : Collider()
{

}

colliders::BoxCollider::BoxCollider(olc::vf2d p1, olc::vf2d p2, olc::vf2d p3, olc::vf2d p4) : BoxCollider()
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->p4 = p4;

	this->allBoxColliders.push_back(this);
}

bool colliders::BoxCollider::collides(const BoxCollider& other) const
{
	return false;
}

bool colliders::BoxCollider::collides(const SegmentCollider& other) const
{
	return other.collides(*this);
}

std::list<uint16_t> colliders::BoxCollider::getAllCollisions()
{
	std::list<uint16_t> l;

	for (colliders::SegmentCollider* sc : colliders::SegmentCollider::allSegmentColliders)
	{
		if (checkIgnored(sc->getId()) == false && collides(*sc) == true)
		{
			l.push_back(sc->getId());
		}
	}
	for (colliders::BoxCollider* bc : colliders::BoxCollider::allBoxColliders)
		if (checkIgnored(bc->getId()) == false && collides(*bc) == true) l.push_back(bc->getId());

	return l;
}

void colliders::BoxCollider::rotate(olc::vf2d origin, float angle)
{

}

void colliders::BoxCollider::translate(olc::vf2d v)
{
	p1 += v;
	p2 += v;
	p3 += v;
	p4 += v;
}

#endif // !COLLIDER_CPP
