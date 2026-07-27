#include "PhysicsCube.h"

Koyu::PhysicsCube::PhysicsCube()
{
    // 1x1x1 Cube
    halfExtents = glm::vec3(0.5f);
}

Koyu::PhysicsCube::PhysicsCube(glm::vec3 _halfExtents)
{
    halfExtents = _halfExtents;
}


float Koyu::PhysicsCube::momentOfInertia()
{
    // Full length
    float l = halfExtents.x * 2.f;

    return (1.f/6.f) * mass * (l * l); // Simplified cube formula
}

void Koyu::PhysicsCube::updateAngularVelocity(float deltaTime)
{
    glm::mat3 wolrdInvTensor = getWorldInvTensor();

    glm::vec3 angularAcceleration = wolrdInvTensor * accumulatedTorque;
    angularVelocity += angularAcceleration * deltaTime;

    angularVelocity = angularVelocity * powf(angularDamping, deltaTime);
}

// Matrix of the 3 axis of MoI
glm::mat3 Koyu::PhysicsCube::getInertiaTensor()
{
    glm::vec3 size = halfExtents * 2.f;

    // Width, height, depth
    float w = size.x;
    float h = size.y;
    float d = size.z;

    float ix = (1.f/12.f) * mass * (h * h + d * d); // BB
    float iy = (1.f/12.f) * mass * (w * w + d * d); // AA
    float iz = (1.f/12.f) * mass * (w * w + h * h); // CC

    // Inertia Tensor
    // Matrix for MoI
    return glm::mat3(
        glm::vec3(ix, 0, 0),
        glm::vec3(0, iy, 0),
        glm::vec3(0, 0, iz)
    );
}

// Rotates our Intertia Tensor
glm::mat3 Koyu::PhysicsCube::getWorldInvTensor()
{
    glm::mat3 localTensor = getInertiaTensor();

    // Invert the local Tensor
    glm::mat3 localInvTensor = glm::mat3(

        glm::vec3(1.f/localTensor[0][0], 0.f, 0.f),
        glm::vec3(0.f, 1.f/localTensor[0][0], 0.f),
        glm::vec3(0.f, 0.f, 1.f/localTensor[0][0])

    );

    glm::quat orientationQuat = glm::quat(rotation);
    glm::mat3 R = glm::mat3_cast(orientationQuat);

    // Rotate the inv tensor
    // qvq
    return R * localInvTensor * glm::transpose(R);
}
