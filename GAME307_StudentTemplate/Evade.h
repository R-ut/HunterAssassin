#ifndef EVADE_H
#define EVADE_H

#include "body.h"
#include "SteeringOutput.h"
class Evade {
private:
    const Body* npc;
    const Body* target;
    float maxPrediction;

public:
    Evade(const Body* npc_, const Body* target_, float maxPrediction_);
    ~Evade();

    SteeringOutput* getSteering();
};

#endif // EVADE_H