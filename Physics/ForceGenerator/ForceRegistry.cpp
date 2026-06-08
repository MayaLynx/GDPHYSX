#include "ForceRegistry.h"

void ForceRegistry::add(PhysicsParticle* particle, ForceGenerator* generator)
{
	ParticleForceRegistry toAdd;
	toAdd.particle = particle;
	toAdd.generator = generator;

	registry.push_back(toAdd);
}

void ForceRegistry::remove(PhysicsParticle* particle, ForceGenerator* generator)
{
	registry.remove_if(
		[particle, generator](ParticleForceRegistry reg)
		{
			return reg.particle == particle && reg.generator == generator;
		}
	);
}

void ForceRegistry::clear()
{
	registry.clear();
}

void ForceRegistry::updateForces(float time)
{
	for (std::list<ParticleForceRegistry>::iterator i = registry.begin(); i != registry.end(); i++)
	{
		i->generator->updateForce(i->particle, time);
	}
}
