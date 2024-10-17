#include "Evade.h" 

Evade::Evade(const Body* npc_, const Body* target_, float maxPrediction_)
    : npc(npc_), target(target_), maxPrediction(maxPrediction_) {}

Evade::~Evade() {}

SteeringOutput* Evade::getSteering() {
    Vec3 direction = target->getPos() - npc->getPos();
    float distance = VMath::mag(direction);  // Get distance between NPC and target

    // Get NPC's speed
    Vec3 velocity = npc->getVel();
    float speed = VMath::mag(velocity);

    // Determine prediction time based on speed and distance
    float prediction;
    if (speed <= (distance / maxPrediction)) {
        prediction = maxPrediction;
    }
    else {
        prediction = distance / speed;
    }

    // Predict the future position of the target
    Vec3 futurePosition = target->getPos() + target->getVel() * prediction;

    // Calculate the direction to flee from the future position
    SteeringOutput* result = new SteeringOutput();
    direction = npc->getPos() - futurePosition;  // Fleeing, so reverse the direction
    result->linear = VMath::normalize(direction) * npc->getMaxAcceleration();  // Apply max acceleration

    // No angular acceleration for this behavior
    result->angular = 0.0f;

    return result;
}
