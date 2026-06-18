#include "PhysicsWorld.h"

namespace Koyu
{
	void PhysicsWorld::addParticle(PhysicsParticle* toAdd)
	{
		Particles.push_back(toAdd);
		forceRegistry.add(toAdd, &gravity);
	}

    void PhysicsWorld::addContact(PhysicsParticle *p1, PhysicsParticle *p2, float restitution, glm::vec3 contactNormal, float depth)
    {
		ParticleContact *toAdd = new ParticleContact();

		toAdd->particles[0] = p1;
		toAdd->particles[1] = p2;
		toAdd->restitution = restitution;
		toAdd->contactNormal = contactNormal;
		toAdd->depth = depth;

		contacts.push_back(toAdd);
    }

    void PhysicsWorld::generateContacts()
    {
		contacts.clear();

		getOverlaps();

		for (std::list<ParticleLink*>::iterator i = links.begin();
			i != links.end();
			i++)
			{
				ParticleContact* contact = (*i)->getContact();

				if (contact != nullptr)
				{
					contacts.push_back(contact);
				}
			}
    }

    void PhysicsWorld::getOverlaps()
    {
		for (int i = 0; i < Particles.size() - 1; i++)
		{
			std::list<PhysicsParticle*>::iterator a = std::next(Particles.begin(), i);

			for (int h = i + 1; h < Particles.size(); h++)
			{
				std::list<PhysicsParticle*>::iterator b = std::next(Particles.begin(), h);
				
				glm::vec3 mag2Vector = (*a)->position - (*b)->position;
				float mag2 = glm::dot(mag2Vector, mag2Vector);

				float rad = (*a)->radius + (*b)->radius;
				float rad2 = rad * rad;

				if (mag2 <= rad2)
				{
					glm::vec3 dir = glm::normalize(mag2Vector);

					// Get the depth of the collision
					float r = rad2 - mag2;
					float depth = sqrt(r);

					// Use the lower restitution of the two
					float restitution = fmin(
						(*a)->restitution, (*b)->restitution
					);

					// Add the contact
					addContact(*a, *b, restitution, dir, depth);
				}
			}
		}
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

		generateContacts();

		// Only call resolve contacts when there are contacts
		if (contacts.size() > 0)
		{
			contactResolver.resolveContacts(contacts, time);
		}
	}
    void PhysicsWorld::draw()
    {
		for (PhysicsParticle* p : Particles)
		{
			p->draw();
		}
    }
}