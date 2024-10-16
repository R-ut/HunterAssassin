#include "Arrive.h"
#include "VMath.h"  // Assumes VMath handles vector operations

Arrive::Arrive(const Body* npc_, const Body* target_) {
    npc = npc_;
    target = target_;
}

Arrive::~Arrive() {}

SteeringOutput* Arrive::getSteering() {
    SteeringOutput* result = new SteeringOutput();  // Initialize steering output
    float targetSpeed;

    // Calculate direction and distance to the target
    Vec3 direction = target->getPos() - npc->getPos();
    float distance = VMath::mag(direction);  // Magnitude of the direction vector

    // If within a small threshold, stop movement and set velocity to zero
    if (distance < target->getRadius() * 0.5f) {  // Fine-tuned dead zone
        delete result;
        return nullptr;
    }

    // Determine the appropriate target speed based on the distance
    if (distance > target->getRadius()) {
        targetSpeed = npc->getMaxSpeed();  // Max speed if far
    }
    else {
        // Smooth deceleration: scale speed based on distance
        targetSpeed = npc->getMaxSpeed() * (distance / target->getRadius());
    }

    // Normalize direction and scale by target speed to get target velocity
    Vec3 targetVelocity = VMath::normalize(direction) * targetSpeed;

    // Calculate linear acceleration towards the target velocity
    result->linear = (targetVelocity - npc->getVel()) / timeToTarget;

    // Clamp the acceleration to avoid excessive changes
    if (VMath::mag(result->linear) > npc->getMaxAcceleration()) {
        result->linear = VMath::normalize(result->linear) * npc->getMaxAcceleration();
    }

    // No angular acceleration required for this behavior
    result->angular = 0.0f;

    return result;
}
