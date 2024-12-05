#include "Seek.h"
#include "VMath.h"

Seek::Seek(const Body* npc_, const Body* target_) {
    npc = npc_;
    target = target_;
    result = new SteeringOutput(); // Initialize result
}

Seek::~Seek() {
    // Destructor left empty for now
}

SteeringOutput* Seek::getSteering() {
    // Get direction to target
    result->linear = target->getPos() - npc->getPos();

    // Normalize direction and apply maximum acceleration
    result->linear = VMath::normalize(result->linear) * npc->getMaxAcceleration();
    result->angular = 0.0f;

    return result; // Return the result pointer
}

