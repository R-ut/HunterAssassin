#include "BlendedSteering.h"
#include "VMath.h"

void BlendedSteering::addBehavior(std::shared_ptr<SteeringBehaviour> behavior, float weight) {
    behaviors.push_back({ behavior, weight });
}

SteeringOutput* BlendedSteering::getSteering() {
    SteeringOutput* result = new SteeringOutput();

    for (const auto& bw : behaviors) {
        SteeringOutput* steering = bw.behavior->getSteering();
        result->linear += steering->linear * bw.weight;
        result->angular += steering->angular * bw.weight;
        delete steering; // Clean up dynamically allocated memory
    }
    if (VMath::mag(result->linear) > maxAcceleration) {
        result->linear = VMath::normalize(result->linear) * maxAcceleration;
    }
    return result;
}