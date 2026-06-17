#include "PhysicsWorld.h"

namespace Koyu
{
	void PhysicsWorld::addParticle(PhysicsParticle* toAdd)
	{
		Particles.push_back(toAdd);
		forceRegistry.add(toAdd, &gravity);
	}

    void PhysicsWorld::addContact(PhysicsParticle *p1, PhysicsParticle *p2, float restitution, glm::vec3 contactNormal)
    {
		ParticleContact *toAdd = new ParticleContact();

		toAdd->particles[0] = p1;
		toAdd->particles[1] = p2;
		toAdd->restitution = restitution;
		toAdd->contactNormal = contactNormal;

		contacts.push_back(toAdd);
    }

    void PhysicsWorld::generateContacts()
    {
		contacts.clear();

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