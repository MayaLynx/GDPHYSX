#pragma once

#include "PhysicsParticle.h"

using namespace Koyu;
class ParticleContact
{
    protected:
        float getSeparatingSpeed();
        void resolveVelocity(float time);

    public:
        PhysicsParticle* particles[2];
        float restitution;
        glm::vec3 contactNormal;
        void resolve(float time);
};