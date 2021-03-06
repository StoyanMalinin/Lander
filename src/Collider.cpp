#ifndef COLLIDER_CPP
#define COLLIDER_CPP

#include "Collider.h"

colliders::Collider::Collider()
{
	this->id = rnd();//we do not care that it overflows
	this->active = true;
	this->ignored.insert(id);
}

std::mt19937 colliders::Collider::rnd = std::mt19937(22);

colliders::SegmentCollider::SegmentCollider(olc::vf2d A, olc::vf2d B) : Collider()
{
	this->A = A;
	this->B = B;

	this->allSegmentColliders.push_back(this);
	this->id2Pos[this->id] = std::prev(this->allSegmentColliders.end());
}

colliders::SegmentCollider::~SegmentCollider()
{
	allSegmentColliders.erase(id2Pos[id]);
}

std::list<uint16_t> colliders::SegmentCollider::getAllCollisions()
{
	std::list<uint16_t> l;

	for (colliders::SegmentCollider* sc : colliders::SegmentCollider::allSegmentColliders)
		if (checkIgnored(sc->getId()) == false && sc->active==true && collides(*sc) == true) l.push_back(sc->getId());
	for (colliders::BoxCollider* bc : colliders::BoxCollider::allBoxColliders)
		if (checkIgnored(bc->getId()) == false && bc->active == true && collides(*bc) == true) l.push_back(bc->getId());

	return l;
}

std::list<colliders::SegmentCollider*>  colliders::SegmentCollider::allSegmentColliders = std::list<colliders::SegmentCollider*>();
std::list<colliders::BoxCollider*>  colliders::BoxCollider::allBoxColliders = std::list<colliders::BoxCollider*>();
std::unordered_map<uint16_t, std::list<colliders::BoxCollider*>::iterator> colliders::BoxCollider::id2Pos = std::unordered_map<uint16_t, std::list<colliders::BoxCollider*>::iterator>();
std::unordered_map<uint16_t, std::list<colliders::SegmentCollider*>::iterator> colliders::SegmentCollider::id2Pos = std::unordered_map<uint16_t, std::list<colliders::SegmentCollider*>::iterator>();

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

	if (math::insideQuad(A, other.p1, other.p2, other.p3, other.p4) == true) return true;
	if (math::insideQuad(B, other.p1, other.p2, other.p3, other.p4) == true) return true;

	return false;
}

void colliders::SegmentCollider::rotate(olc::vf2d origin, float angle)
{
}

void colliders::SegmentCollider::translate(olc::vf2d v)
{
	A += v;
	B += v;
}

colliders::BoxCollider::BoxCollider() : Collider() {}

colliders::BoxCollider::BoxCollider(olc::vf2d p1, olc::vf2d p2, olc::vf2d p3, olc::vf2d p4) : BoxCollider()
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->p4 = p4;

	this->allBoxColliders.push_back(this);
	this->id2Pos[this->id] = std::prev(this->allBoxColliders.end());
}

colliders::BoxCollider::~BoxCollider()
{
	allBoxColliders.erase(id2Pos[id]);
}

bool colliders::BoxCollider::collides(const BoxCollider& other) const
{
	if (math::insideQuad(p1, other.p1, other.p2, other.p3, other.p4) == true) return true;
	if (math::insideQuad(p2, other.p1, other.p2, other.p3, other.p4) == true) return true;
	if (math::insideQuad(p3, other.p1, other.p2, other.p3, other.p4) == true) return true;
	if (math::insideQuad(p4, other.p1, other.p2, other.p3, other.p4) == true) return true;

	if (math::insideQuad(other.p1, p1, p2, p3, p4) == true) return true;
	if (math::insideQuad(other.p2, p1, p2, p3, p4) == true) return true;
	if (math::insideQuad(other.p3, p1, p2, p3, p4) == true) return true;
	if (math::insideQuad(other.p4, p1, p2, p3, p4) == true) return true;

	if (math::segmentIntersection(p1, p2, other.p1, other.p2) == true) return true;
	if (math::segmentIntersection(p1, p2, other.p2, other.p3) == true) return true;
	if (math::segmentIntersection(p1, p2, other.p3, other.p4) == true) return true;
	if (math::segmentIntersection(p1, p2, other.p4, other.p1) == true) return true;

	if (math::segmentIntersection(p2, p3, other.p1, other.p2) == true) return true;
	if (math::segmentIntersection(p2, p3, other.p2, other.p3) == true) return true;
	if (math::segmentIntersection(p2, p3, other.p3, other.p4) == true) return true;
	if (math::segmentIntersection(p2, p3, other.p4, other.p1) == true) return true;

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
		if (checkIgnored(sc->getId()) == false && sc->active == true && collides(*sc) == true) l.push_back(sc->getId());
	for (colliders::BoxCollider* bc : colliders::BoxCollider::allBoxColliders)
		if (checkIgnored(bc->getId()) == false && bc->active==true && collides(*bc) == true)
		{
			collides(*bc);
			l.push_back(bc->getId());
		}

	return l;
}

void colliders::BoxCollider::rotate(olc::vf2d origin, float angle)
{
	p1 = (p1 - origin).rotated(angle) + origin;
	p2 = (p2 - origin).rotated(angle) + origin;
	p3 = (p3 - origin).rotated(angle) + origin;
	p4 = (p4 - origin).rotated(angle) + origin;
}

void colliders::BoxCollider::translate(olc::vf2d v)
{
	p1 += v;
	p2 += v;
	p3 += v;
	p4 += v;
}

#endif // !COLLIDER_CPP
