#pragma once
#include "PhysicsParticle.h"

namespace Koyu
{
    class PhysicsCube : public PhysicsParticle
    {
        protected:
            // Cubes have a different formula
            float momentOfInertia() override;
            // Prisms have a different way of getting it
            void updateAngularVelocity(float deltaTime) override;

            glm::mat3 getInertiaTensor();
            glm::mat3 getWorldInvTensor();
        
        public:
            // From the center to the edge
            glm::vec3 halfExtents;

            PhysicsCube(GLuint shader);
            PhysicsCube(GLuint shader, glm::vec3 _halfExtents);
    };
}