#pragma once

#include "ParticleContact.h"

class ContactResolver
{
    protected:
        unsigned current_iterations = 0;

    public:
        unsigned max_iterations;
        ContactResolver(unsigned _maxIterations) : max_iterations(_maxIterations) {}
        void resolveContacts(std::vector<ParticleContact*> contacts, float time);
};