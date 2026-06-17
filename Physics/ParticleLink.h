#pragma once

#include "PhysicsParticle.h"
#include "ParticleContact.h"

using namespace Koyu;
class ParticleLink
{
    protected:
        float currentLength();

    public:
        PhysicsParticle* particles[2];
        virtual ParticleContact* getContact() { return nullptr; };
};