#include "Rod.h"

ParticleContact* Rod::getContact()
{
    float currLen = currentLength();

    if (currLen == length)
    {
        return nullptr;
    }

    ParticleContact* ret = new ParticleContact();
    ret->particles[0] = particles[0];
    ret->particles[1] = particles[1];

    glm::vec3 dir = particles[1]->position - particles[0]->position;
    dir = glm::normalize(dir);

    if (currLen > length)
    {
        ret->contactNormal = dir;
        ret->depth = currLen - length;
    }
    else
    {
        ret->contactNormal = dir * glm::vec3(-1);
        ret->depth = length - currLen;
    }
    ret->restitution = restitution;

    return ret;
}