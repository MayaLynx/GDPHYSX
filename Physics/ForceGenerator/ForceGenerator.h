#pragma once
#include "../PhysicsParticle.h"

using namespace Koyu;
class ForceGenerator
{
public:
	virtual void updateForce(PhysicsParticle* p, float time)
	{
		p->addForce(glm::vec3(0.f, 0.f, 0.f));
	}
};

