#pragma once
#include "PhysicsParticle.h"
#include "ParticleContact.h"
#include "ContactResolver.h"
#include "ForceGenerator/ForceRegistry.h"
#include "ForceGenerator/GravityForceGenerator.h"
#include <list>

namespace Koyu
{
	class PhysicsWorld
	{
		protected:
			ContactResolver contactResolver = ContactResolver(20);
		
		private:
			void updateParticleList();
			GravityForceGenerator gravity = GravityForceGenerator(glm::vec3(0.f, -9.8f, 0.f));

		public:
			std::list<PhysicsParticle*> Particles; // Holds all the particles
			ForceRegistry forceRegistry;
			std::vector<ParticleContact*> contacts;

			void addParticle(PhysicsParticle* toAdd);
			void addContact(PhysicsParticle* p1, PhysicsParticle* p2, float restitution, glm::vec3 contactNormal);
			void update(float time);
		};
}


