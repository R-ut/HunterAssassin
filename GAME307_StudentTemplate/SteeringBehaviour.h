#ifndef STEERING_BEHAVIOUR_H
#define STEERING_BEHAVIOUR_H

#include "Body.h" // Ensure Body is defined or included
#include "SteeringOutput.h"
#include "KinematicBody.h"
class SteeringBehaviour {
protected:
    SteeringOutput* result; // Dynamically allocated
    const Body* npc;
    KinematicBody* character;


public:
    SteeringBehaviour(KinematicBody* character_) : character(character_) {}
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