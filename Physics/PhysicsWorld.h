#pragma once
#include "PhysicsParticle.h"
#include "ForceGenerator/ForceRegistry.h"
#include "ForceGenerator/GravityForceGenerator.h"
#include <list>

namespace Koyu
{
	class PhysicsWorld
	{
	private:
		void updateParticleList();
		GravityForceGenerator gravity = GravityForceGenerator(glm::vec3(0.f, -9.8f, 0.f));

	public:
		std::list<PhysicsParticle*> Particles; // Holds all the particles
		ForceRegistry forceRegistry;

		void addParticle(PhysicsParticle* toAdd);
		void update(float time);
	};
}


