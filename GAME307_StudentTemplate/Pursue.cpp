#include "Pursue.h"

Pursue::Pursue(const Body* npc_, const Body* target_, float maxPrediction_)
    : Seek(npc_, target_), maxPrediction(maxPrediction_)
{
}

Pursue::~Pursue() {}

SteeringOutput* Pursue::getSteering()
{
    Vec3 direction = target->getPos() - npc->getPos();

    // Calculate the distance to the target by taking it's magnitude or length for each direction
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

    // Get the current speed of the NPC
    Vec3 velocity = npc->getVel();
    float speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);

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

    // Set this predicted position as the target for Seek
    direction = futurePosition - npc->getPos();

    // Normalize the direction and apply the NPC's maximum acceleration
    SteeringOutput* result = new SteeringOutput();
    result->linear = VMath::normalize(direction) * npc->getMaxAcceleration();
    result->angular = 0.0f;

    return result;
}
