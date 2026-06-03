#include "PhysicsParticle.h"

namespace Physics
{
	PhysicsParticle::PhysicsParticle()
	{
		this->position = glm::vec3(0.f, 0.f, 0.f);
		this->velocity = glm::vec3(0.f, 0.f, 0.f);
		this->acceleration = glm::vec3(0.f, 0.f, 0.f);
	}

	void PhysicsParticle::destroy()
	{
		isDestroyed = true;
	}

	bool PhysicsParticle::IsDestroyed()
	{
		return isDestroyed;
	}

	void PhysicsParticle::updatePosition(float deltaTime)
	{
		this->position = this->position + (this->velocity * deltaTime) + ((1.0f / 2.0f) * (this->acceleration * deltaTime * deltaTime));
	}

	void PhysicsParticle::updateVelocity(float deltaTime)
	{
		this->velocity = this->velocity + (this->acceleration * deltaTime);
	}

	void PhysicsParticle::update(float time)
	{
		this->updatePosition(time);
		this->updateVelocity(time);
	}

}
