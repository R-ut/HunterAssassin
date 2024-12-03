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
    // Constructor with character initialization
    SteeringBehaviour(KinematicBody* character_)
        : result(new SteeringOutput()), npc(nullptr), character(character_) {}

    // Default constructor for flexibility
    SteeringBehaviour()
        : result(new SteeringOutput()), npc(nullptr), character(nullptr) {}
    virtual ~SteeringBehaviour() {
        if (result) {
            delete result;
            result = nullptr; // Avoid dangling pointer
        }
    }
    virtual SteeringOutput* getSteering() = 0;

    //// Setter for NPC (optional, if needed later in your logic)
    //void setNPC(const Body* npc_) {
    //    npc = npc_;
    //}

    //// Setter for Character (optional, if needed later in your logic)
    //void setCharacter(KinematicBody* character_) {
    //    character = character_;
    //}
};

#endif