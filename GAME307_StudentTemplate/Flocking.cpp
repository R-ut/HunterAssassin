#include "Flocking.h"
#include "VMath.h"

SteeringOutput* Flocking::getSteering() {
    SteeringOutput* steering = new SteeringOutput();

    // Compute weighted behaviors
    Vec3 separation = calculateSeparation() * separationWeight;
    Vec3 alignment = calculateAlignment() * alignmentWeight;
    Vec3 cohesion = calculateCohesion() * cohesionWeight;

    steering->linear = separation + alignment + cohesion;
    steering->angular = 0.0f;

    return steering; // Return a pointer
}

Vec3 Flocking::calculateSeparation() {
    Vec3 force(0.0f, 0.0f, 0.0f);
    for (auto neighbor : neighbors) {
        Vec3 diff = character->getPos() - neighbor->getPos();
        float distance = VMath::mag(diff);
        if (distance > 0.0f && distance < separationThreshold) {  // Parameterized threshold
            force += VMath::normalize(diff) / distance;
        }
    }
    if (VMath::mag(force) > maxSeparation) {
        force = VMath::normalize(force) * maxSeparation;
    }
    return force;
}

Vec3 Flocking::calculateAlignment() {
    Vec3 averageVelocity(0.0f, 0.0f, 0.0f);
    for (auto neighbor : neighbors) {
        averageVelocity += neighbor->getVel();
    }
    if (!neighbors.empty()) {
        averageVelocity /= static_cast<float>(neighbors.size());
    }
    Vec3 alignmentForce = averageVelocity - character->getVel();
    if (VMath::mag(alignmentForce) > maxAlignment) {
        alignmentForce = VMath::normalize(alignmentForce) * maxAlignment;
    }
    return alignmentForce;
}

Vec3 Flocking::calculateCohesion() {
    Vec3 centerOfMass(0.0f, 0.0f, 0.0f);
    for (auto neighbor : neighbors) {
        centerOfMass += neighbor->getPos();
    }
    if (!neighbors.empty()) {
        centerOfMass /= static_cast<float>(neighbors.size());
    }
    Vec3 cohesionForce = centerOfMass - character->getPos();
    if (VMath::mag(cohesionForce) > maxCohesion) {
        cohesionForce = VMath::normalize(cohesionForce) * maxCohesion;
    }
    return cohesionForce;
}