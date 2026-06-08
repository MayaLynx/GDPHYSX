#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../model3D/model3D.h"
namespace Koyu
{
	class PhysicsParticle
	{
	protected:
		bool isDestroyed = false;

		void updatePosition(float deltaTime);
		void updateVelocity(float deltaTime);

		glm::vec3 accumulatedForce;


	public:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		float mass;
		model3D* model;
		GLuint shader;
		float damping;

		PhysicsParticle(GLuint shader);

		void addForce(glm::vec3 force);
		void resetForce();

		void setColor(glm::vec3 newColor);
		void destroy();
		bool IsDestroyed();
		void update(float time);
		void draw();
	};
}
