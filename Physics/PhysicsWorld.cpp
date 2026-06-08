#include "PhysicsWorld.h"

namespace Koyu
{
	void PhysicsWorld::addParticle(PhysicsParticle* toAdd)
	{
		Particles.push_back(toAdd);
		forceRegistry.add(toAdd, &gravity);
	}

	void PhysicsWorld::updateParticleList()
	{
		// Removes all particles in list where isDestroyed is true
		Particles.remove_if(
			[](PhysicsParticle* p)
			{
				return p->IsDestroyed();
			}
		);
		// It resizes the list afterward
	}

	void PhysicsWorld::update(float time)
	{
		// Updates the list first before doing anything
		updateParticleList();
		forceRegistry.updateForces(time);

		for (std::list<PhysicsParticle*>::iterator p = Particles.begin(); p != Particles.end(); p++)
		{
			(*p)->update(time);
		}
	}
}