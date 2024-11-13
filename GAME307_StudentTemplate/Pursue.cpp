#include "Pursue.h"

Pursue::Pursue(const Body* npc_, Vec3 target_, float maxPrediction_)
    : Seek(npc_, target_), maxPrediction(maxPrediction_)
{
}

Pursue::~Pursue() {}

SteeringOutput* Pursue::getSteering()
{
    Vec3 direction = target - npc->getPos();

    // Calculate the distance to the target by taking it's magnitude or length for each direction
    float distance = VMath::mag(direction);

    // Get the current speed of the NPC
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
    Vec3 futurePosition = target + target * prediction;

    // Implemneting the Seek Code to acellerate it in that direction.
    result->linear = VMath::normalize(futurePosition - npc->getPos()) * npc->getMaxAcceleration();
    result->angular = 0.0f;

    return result;
}
