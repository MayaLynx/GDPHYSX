#pragma once

#include "../ParticleLink.h"

class Rod : public ParticleLink
{
    public:
        float length = 1;
        float restitution = 0; // Always 0 because a rod has no bounciness

        ParticleContact* getContact() override;
};