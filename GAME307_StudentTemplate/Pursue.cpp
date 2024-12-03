#include "Pursue.h"
#include "VMath.h"

Pursue::Pursue(const Body* npc_, const Body* target_, float maxPrediction_)
    : Seek(npc_, target_), maxPrediction(maxPrediction_) {}

Pursue::~Pursue() {}

SteeringOutput* Pursue::getSteering() {
    SteeringOutput* result = new SteeringOutput();

    // Calculate direction and distance to the target
    Vec3 direction = target->getPos() - npc->getPos();
    float distance = VMath::mag(direction);

    // Get the NPC's speed
    Vec3 velocity = npc->getVel();
    float speed = VMath::mag(velocity);

    // Decide on a reasonable prediction time
    float prediction;
    if (speed <= (distance / maxPrediction)) {
        prediction = maxPrediction;
    }
    else {
        prediction = distance / speed;
    }

    // Predict the future position of the target
    Vec3 futurePosition = target->getPos() + target->getVel() * prediction;

    // Implement Seek logic towards the future position
    result->linear = VMath::normalize(futurePosition - npc->getPos()) * npc->getMaxAcceleration();
    result->angular = 0.0f;

    return result;
}

