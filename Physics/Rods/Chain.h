/* 
This is a chain because the particle attached to it cannot go farther than its length
starting from the anchor point. Unlike the bungee/spring, the chain will not stretch more than its length
and the particle will not bounce when you let it drop as far as the chain's length.
*/
#pragma once
#include "../ParticleLink.h"

class Chain : public ParticleLink
{
    public:
        float length = 1;
        float restitution = 0;
        glm::vec3 anchorPoint;

        ParticleContact* getContact() override;
};