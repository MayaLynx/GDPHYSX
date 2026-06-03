#pragma once
#include "PhysicsParticle.h"
#include <list>

namespace Physics
{
	class PhysicsWorld
	{
	private:
		void updateParticleList();

	public:
		std::list<PhysicsParticle*> Particles;

		void addParticle(PhysicsParticle* toAdd);
		void update(float time);
	};
}


