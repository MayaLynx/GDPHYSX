#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PhysicsParticle
{
protected:
	void updatePosition(float deltaTime);
	void updateVelocity(float deltaTime);


public:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	PhysicsParticle();
	void update(float time);
};