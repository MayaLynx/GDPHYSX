#include "ContactResolver.h"

void ContactResolver::resolveContacts(std::vector<ParticleContact *> contacts, float time)
{
    for (ParticleContact* p : contacts)
    {
        p->resolve(time);
    }
}
