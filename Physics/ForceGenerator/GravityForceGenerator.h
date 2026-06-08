#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator
{
private:
	glm::vec3 gravity = glm::vec3(0.f, -9.8f, 0.f);

public:
	GravityForceGenerator(const glm::vec3 gravity) : gravity(gravity) {}
	void updateForce(PhysicsParticle* particle, float time) override;
};

