#include "ParticleContact.h"

float ParticleContact::getSeparatingSpeed()
{
    glm::vec3 velocity = particles[0]->velocity;

    if(particles[1]) velocity -= particles[1]->velocity;

    return glm::dot(velocity, contactNormal);
}

void ParticleContact::resolveVelocity(float time)
{
    float separatingSpeed = getSeparatingSpeed();

    // Check if the objects are already moving away from each other
    if(separatingSpeed > 0)
    {
        return;
    }

    float newSS = -restitution * separatingSpeed;
    float deltaSpeed = newSS - separatingSpeed;

    float totalMass = (float)1 / particles[0]->mass;
    if(particles[1]) totalMass += (float)1 / particles[1]->mass;

    // Invalid collision if total mass is 0 or less
    if(totalMass <= 0) return;

    float impulse_mag = deltaSpeed / totalMass;
    glm::vec3 impulse = contactNormal * impulse_mag;

    glm::vec3 V_a = impulse * ((float)1 / particles[0]->mass);
    particles[0]->velocity = particles[0]->velocity + V_a;

    if(particles[1])
    {
        glm::vec3 V_b = impulse * ((float)-1 / particles[1]->mass);
        particles[1]->velocity = particles[1]->velocity + V_b;
    }
}

void ParticleContact::resolve(float time)
{
    resolveVelocity(time);
}