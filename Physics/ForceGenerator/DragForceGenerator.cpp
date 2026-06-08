#include "DragForceGenerator.h"

void DragForceGenerator::updateForce(PhysicsParticle* particle, float time)
{
	glm::vec3 force = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 currV = particle->velocity;
	float mag = glm::length(particle->velocity);

	if (mag <= 0) return;

	float dragF = (k1 * mag) + (k2 * mag);
	glm::vec3 dir = glm::normalize(currV);

	particle->addForce(dir * -dragF);
}
