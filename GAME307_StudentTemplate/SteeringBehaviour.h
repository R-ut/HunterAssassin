#ifndef STEERING_BEHAVIOUR_H
#define STEERING_BEHAVIOUR_H

#include "Body.h" // Ensure Body is defined or included
#include "SteeringOutput.h"

class SteeringBehaviour {
protected:
    SteeringOutput* result; // Dynamically allocated
    const Body* npc;

public:
    SteeringBehaviour() : result(new SteeringOutput()), npc(nullptr) {}
    virtual ~SteeringBehaviour() {
        if (result) {
            delete result;
            result = nullptr; // Avoid dangling pointer
        }
    }
    virtual SteeringOutput* getSteering() = 0;
};

#endif