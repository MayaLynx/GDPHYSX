#pragma once
#include "../PhysicsParticle.h"
#include "ForceGenerator.h"
#include<list>

using namespace Koyu;
class ForceRegistry
{
protected:
	struct ParticleForceRegistry
	{
		PhysicsParticle* particle;
		ForceGenerator* generator;
	};

	std::list<ParticleForceRegistry> registry;

public:
	void add(PhysicsParticle* particle, ForceGenerator* generator);
	void remove(PhysicsParticle* particle, ForceGenerator* generator);
	void clear();
	void updateForces(float time);
};

