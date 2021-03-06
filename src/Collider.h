#ifndef COLLIDER_H
#define COLLIDER_H

#include <set>
#include <list>
#include <random>
#include <vector>
#include <unordered_set>

#include "Math.h"
#include "olcPixelGameEngine.h"

namespace colliders
{
	class SegmentCollider;
	class BoxCollider;

	class Collider
	{
	private:
		static std::mt19937 rnd;
	
	public:
		bool active;
	protected:
		uint16_t id;
		std::unordered_set<uint16_t> ignored;
	
	public:
		Collider();

		void addIgnoredId(uint16_t id)
		{
			ignored.insert(id);
		}

		uint16_t getId() { return id; }
		bool checkIgnored(uint16_t otherId) { return ignored.count(otherId); }

		virtual bool collides(const SegmentCollider& other) const { return false; }
		virtual bool collides(const BoxCollider& other) const { return false; }

		virtual void rotate(olc::vf2d origin, float angle) {}
		virtual void translate(olc::vf2d v) {}
	};

	class SegmentCollider : public Collider
	{
	public:
		olc::vf2d A, B;

	public:
		static std::list<SegmentCollider*> allSegmentColliders;
	private:
		static std::unordered_map<uint16_t, std::list<SegmentCollider*>::iterator> id2Pos;

	public:
		SegmentCollider(olc::vf2d A, olc::vf2d B);
		~SegmentCollider();

		std::list<uint16_t> getAllCollisions();

		bool collides(const SegmentCollider& other) const override;
		bool collides(const BoxCollider& other) const override;

		void rotate(olc::vf2d origin, float angle) override;
		void translate(olc::vf2d v) override;
	};

	class BoxCollider : public Collider
	{
	public:
		olc::vf2d p1, p2, p3, p4; //the orientation must be clockwise

		BoxCollider();
		BoxCollider(olc::vf2d p1, olc::vf2d p2, olc::vf2d p3, olc::vf2d p4);

		~BoxCollider();

	public:
		static std::list <BoxCollider*> allBoxColliders;
	private:
		static std::unordered_map<uint16_t, std::list<BoxCollider*>::iterator> id2Pos;

	public:
		bool collides(const BoxCollider& other) const override;
		bool collides(const SegmentCollider& other) const override;
		
		std::list<uint16_t> getAllCollisions();

	public:
		void rotate(olc::vf2d origin, float angle) override;
		void translate(olc::vf2d v) override;
	};
}

#endif

