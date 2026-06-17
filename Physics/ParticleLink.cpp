#include "ParticleLink.h"

float ParticleLink::currentLength()
{
    glm::vec3 ret = particles[0]->position - particles[1]->position;
    return glm::length(ret);
}
