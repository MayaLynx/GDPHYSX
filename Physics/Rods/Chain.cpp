#include "Chain.h"

ParticleContact* Chain::getContact()
{
    float currLen = glm::length(particles[0]->position - anchorPoint);

    if (currLen == length)
    {
        return nullptr;
    }

    ParticleContact* ret = new ParticleContact();
    ret->particles[0] = particles[0];
    
    glm::vec3 dir = anchorPoint - particles[0]->position;
    dir = glm::normalize(dir);

    if (currLen > length)
    {
        ret->contactNormal = dir;
        ret->depth = currLen - length;
    }
    // else
    // {
    //     ret->contactNormal = dir * glm::vec3(-1);
    //     ret->depth = length - currLen;
    // }
    ret->restitution = restitution;

    return ret;
}