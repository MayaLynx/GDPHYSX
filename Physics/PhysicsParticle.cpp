#include "PhysicsParticle.h"

namespace Koyu
{
	PhysicsParticle::PhysicsParticle(GLuint shader)
		: shader{shader}
	{
		model = new model3D("3D/sphere.obj", glm::vec3(0.f, 0.f, 0.f), shader);
		model->setScale(20.f, 20.f, 20.f);

		this->position = glm::vec3(0.f, 0.f, 0.f);
		this->velocity = glm::vec3(0.f, 0.f, 0.f);
		this->acceleration = glm::vec3(0.f, 0.f, 0.f);
		this->mass = 1.f;
		this->damping = 0.9f;
		this->accumulatedForce = glm::vec3(0.f, 0.f, 0.f);
	}

	void PhysicsParticle::setColor(glm::vec3 newColor)
	{
		model->setColor(newColor);
	}

	void PhysicsParticle::destroy()
	{
		isDestroyed = true;
	}

	bool PhysicsParticle::IsDestroyed()
	{
		return isDestroyed;
	}

	void PhysicsParticle::addForce(glm::vec3 force)
	{
		this->accumulatedForce += force;
	}

	void PhysicsParticle::resetForce()
	{
		float d_mass = glm::max(
			std::numeric_limits<float>::min(),
			mass
		);

		this->acceleration -= accumulatedForce * (1 / d_mass);
		this->accumulatedForce = glm::vec3(0.f, 0.f, 0.f);
	}

	void PhysicsParticle::updatePosition(float deltaTime)
	{
		this->position = this->position + (this->velocity * deltaTime) + ((1.0f / 2.0f) * (this->acceleration * deltaTime * deltaTime));
	}

	void PhysicsParticle::updateVelocity(float deltaTime)
	{
		float d_mass = glm::max(
			std::numeric_limits<float>::min(),
			mass
		);

		this->acceleration += accumulatedForce * (1 / d_mass);

		this->velocity = this->velocity + (this->acceleration * deltaTime);
		this->velocity = this->velocity * powf(damping, deltaTime);
	}

	void PhysicsParticle::update(float time)
	{
		this->updatePosition(time);
		this->updateVelocity(time);
		this->resetForce();

		model->updatePosition(this->position);
	}

	void PhysicsParticle::draw()
	{
		if (isDestroyed) return; // Stop rendering if destroyed
		model->draw();
	}
}
