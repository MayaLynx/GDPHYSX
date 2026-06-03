#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Physics
{
	class PhysicsParticle
	{
	protected:
		bool isDestroyed = false;

		void updatePosition(float deltaTime);
		void updateVelocity(float deltaTime);


	public:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;

		PhysicsParticle();
		void destroy();
		bool IsDestroyed();
		void update(float time);
	};
}
