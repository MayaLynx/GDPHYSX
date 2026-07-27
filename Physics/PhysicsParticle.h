#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
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

		glm::vec3 accumulatedTorque;
		virtual float momentOfInertia();
		virtual void updateAngularVelocity(float deltaTime);


	public:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f);
		float mass;
		float radius;
		float restitution;
		model3D* model;
		GLuint shader;
		float damping;

		glm::vec3 angularVelocity;
		float angularDamping;

		PhysicsParticle(GLuint shader);
		PhysicsParticle(GLuint shader, std::string texturePath, bool hasAlpha);

		void addForce(glm::vec3 force);
		void resetForce();
		void addForceAtPoint(glm::vec3 force, glm::vec3 p);

		void setColor(glm::vec3 newColor);
		void setScale(glm::vec3 newScale);
		void setRadius(float newRadius);
		void destroy();
		bool IsDestroyed();
		void update(float time);
		void draw();
	};
}
