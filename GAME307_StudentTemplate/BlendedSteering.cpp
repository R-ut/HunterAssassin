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
        delete steering;
    }
    // Inline limit for linear acceleration
    float maxAcceleration = 10.0f; // Example value
    float magnitude = VMath::mag(result->linear);
    if (magnitude > maxAcceleration) {
        result->linear = VMath::normalize(result->linear) * maxAcceleration;
    }
    return result;
}