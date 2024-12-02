#include "Seek.h"
#include "VMath.h"

Seek::Seek(const Body* npc_, const Body* target_) {
    npc = npc_;
    target = target_;
    result = new SteeringOutput(); // Initialize result
}

Seek::~Seek() {
   // delete result; // Clean up memory
}

SteeringOutput* Seek::getSteering() {
    // Get direction to target
    result->linear = target->getPos() - npc->getPos();

    // Accelerate in that direction
    result->linear = VMath::normalize(result->linear) * npc->getMaxAcceleration();
    result->angular = 0.0f;

    return result; // Return the result pointer
}

