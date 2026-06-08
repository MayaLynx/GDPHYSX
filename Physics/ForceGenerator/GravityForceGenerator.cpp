#include "GravityForceGenerator.h"

void GravityForceGenerator::updateForce(PhysicsParticle* particle, float time)
{
	if (particle->mass <= 0) return;

	glm::vec3 force = gravity * particle->mass;
	particle->addForce(force);
}
